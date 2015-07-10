#include <QApplication>
#include <QImage>
#include <QColor>
#include <QPixmap>
#include <QLabel>
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <math.h>
#include <stdio.h>

struct DataPoint {
    int x, y; // in pixel
    float rotation; // in radians
};

std::vector<DataPoint> read_tracking_data(const char *filename)
{
    std::vector<DataPoint> data;

    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Failed opening %s\n", filename);
        exit(1);
    }

    float timestamp = 0;
    int n = 0, id = 0, rot = 0, x = 0, y = 0;

    while (EOF != fscanf(f, "%f %d %d %d %d %d", &timestamp, &n, &id, &rot, &x, &y)) {
        DataPoint d;
        d.x = x;
        d.y = y;
        d.rotation = rot;
        data.push_back(d);
    }
    fclose(f);

    return data;
}

// https://rittwik.wordpress.com/c/algorithm/bresenhams-line-algorithm/
QList<QPoint> BresenhamLineAlgorithm(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx, sy;
    if (x0 < x1) sx = 1; else sx = -1;
    if (y0 < y1) sy = 1; else sy = -1;
    int err = dx - dy;

    QList<QPoint> pointsList;
    while((x0 != x1) || (y0 != y1))
    {
        pointsList<< QPoint(x0,y0);
        int e2 = 2*err;
        if(e2 > -dy)
        {
            err = err - dy;
            x0 = x0 + sx;
        }

        if(e2 <  dx)
        {
            err = err + dx;
            y0 = y0 + sy;
        }
    }

    return pointsList;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // validate command line arguments
    QCommandLineParser parser;
    parser.addPositionalArgument("stitched_data", "File containing stitched tracking data.");

    QCommandLineOption cutoffOption("cutoff",
                                    "Replace values bigger than <max> with <max>. <max> must be positive integer. If <max> is zero cutoff is ignored. This is the default.",
                                    "max",
                                    "0");
    parser.addOption(cutoffOption);

    QCommandLineOption histOption("hist-output", "Output visits per field to <outfile>. May be used with octave to build a histogram.", "outfile");
    parser.addOption(histOption);

    QCommandLineOption radiusOption("robot-radius", "Radius of the robot in pixel (default: 50).", "r", "50");
    parser.addOption(radiusOption);

    QCommandLineOption connectPointsOption("connect-points", "Connect consecutive tracking points using Bresenham's algorithm. Otherwise the mask is only applied to single data points.");
    parser.addOption(connectPointsOption);

    QCommandLineOption saveImageOption("save-image", "Saves the the heatmap under <image>. Fileformat is guessed, see http://doc.qt.io/qt-5/qimage.html#save", "image");
    parser.addOption(saveImageOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    if (args.size() != 1)
    {
        fprintf(stderr, "Missing <stitched_data> or too many arguments.\n\n");
        fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    }

    // check option "cutoff"
    int cutoff = 0;
    if (parser.isSet(cutoffOption))
    {
        const QString cutoffOptionValue = parser.value(cutoffOption);
        bool ok;
        cutoff = cutoffOptionValue.toInt(&ok);
        if (!ok || cutoff < 0)
        {
            fprintf(stderr, "--cutoff's value <max> must be positive integer or zero.\n\n");
            fputs(qPrintable(parser.helpText()), stderr);
            return 1;
        }
    }

    // check option "hist-output"
    bool write_histfile = parser.isSet(histOption);
    QFile outfile;
    QTextStream outstream;
    if (write_histfile)
    {
        const QString outfilePath = parser.value(histOption);
        outfile.setFileName(outfilePath);
        if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            fprintf(stderr, "Could not open <outfile>.");
            return 1;
        }
        outstream.setDevice(&outfile);
    }

    // robot radius
    int robot_radius = 50;
    if (parser.isSet(radiusOption))
    {
        const QString radiusOptionValue = parser.value(radiusOption);
        bool ok;
        robot_radius = radiusOptionValue.toInt(&ok);
        if (!ok || robot_radius <= 0)
        {
            fprintf(stderr, "--robot-radius's value <r> must be positive integer.\n\n");
            fputs(qPrintable(parser.helpText()), stderr);
            return 1;
        }
    }

    // connect points?
    bool connect_points = parser.isSet(connectPointsOption);

    // save image?
    bool save_image = parser.isSet(saveImageOption);
    QString image_filename;
    if (save_image)
    {
        image_filename = parser.value(saveImageOption);
    }

    // read tracking data
    std::vector<DataPoint> trackingData = read_tracking_data(qPrintable(args.at(0)));
    int data_len = trackingData.size();

    // create mask
    // assuming a circular robot
    int center = robot_radius;
    int mask[2*robot_radius][2*robot_radius];
    for (int x = 0; x < 2*robot_radius; ++x) {
        for (int y = 0; y < 2*robot_radius; ++y) {
            if ((center - x)*(center - x) + (center -y )*(center - y) < robot_radius * robot_radius) {
                mask[x][y] = 1;
            } else {
                mask[x][y] = 0;
            }
        }
    }

    // apply mask
    // initialize array that hold number of visits
    int width = 2000, height = 2000;
    std::vector< std::vector<int> > nb_of_visits(width, std::vector<int>(height));
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
                nb_of_visits[x][y] = 0;

    // foreach pair of consecutive data point calculate line and apply mask
    for (int i = 1; i < data_len; ++i) {
        // previous point
        int x0 = trackingData[i - 1].x;
        int y0 = trackingData[i - 1].y;

        // current point
        int x1 = trackingData[i].x;
        int y1 = trackingData[i].y;

        QList<QPoint> linepoints;
        if (connect_points)
        {
            linepoints = BresenhamLineAlgorithm(x0, y0, x1, y1);
        } else {
            linepoints << QPoint(x0, y0);
        }

        QList<QPoint>::iterator iter;
        for (iter = linepoints.begin(); iter != linepoints.end(); ++iter) {
            for (int x = 0; x < 2*robot_radius; ++x) {
                for (int y = 0; y < 2*robot_radius; ++y) {
                    if (mask[x][y]) {
                        // respect offset
                        int relative_x = x - center;
                        int relative_y = y - center;

                        int world_x = iter->x() + relative_x;
                        int world_y = iter->y() + relative_y;

                        // array index check
                        if (world_x >= 0 && world_x < width && world_y > 0 && world_y < height) {
                            nb_of_visits[world_x][world_y]++;
                        }
                    }
                }
            }
        }
    }

    // normalize values to range [0.0; 1.0]
    std::vector< std::vector<float> > heatmap(width, std::vector<float>(height));

    // find min, max and average
    int max_visits = 0, min_visits = INT_MAX;
    float average = 0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int visits = nb_of_visits[x][y];

            // output number of visits to be used by octave
            if (write_histfile)
            {
                outstream << visits << "\n";
            }

            // update min and max visits
            if (visits > max_visits)
                max_visits = visits;

            if (visits < min_visits)
                min_visits = visits;

            average += visits;
        }
    }

    average /= (width * height);
    printf("max visits: %d\n", max_visits);
    printf("min visits: %d\n", min_visits);
    printf("average: %f\n", average);

    // calculate variance
    float variance = 0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
             int visits = nb_of_visits[x][y];
             variance += pow(visits - average, 2);
        }
    }
    variance /= (width * height);
    printf("variance: %f\n", variance);

    // apply scaling with respect to cutoff
    if (cutoff > 0)
    {
        // adjust max_visits to cutoff
        if (max_visits > cutoff)
            max_visits = cutoff;
    }

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int visits = nb_of_visits[x][y];

            // cutoff data
            if (cutoff > 0 && visits > cutoff)
                visits = cutoff;

            // normalize to [0; 1]
            heatmap[x][y] = (visits - min_visits) / (float)max_visits;
        }
    }

    // create image
    QImage image(width, height, QImage::Format_RGB32);
    image.fill(0x000000);

    // apply color map (here: blue intensity map)
    QColor color;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            color.setHsv((1 - heatmap[x][y]) * 240, 255, 255);
            image.setPixel(x, y, color.rgb());
        }
    }

    // scale down
    QImage small = image.scaled(600, 600, Qt::KeepAspectRatio);

    // show image
    QLabel label;
    label.setPixmap(QPixmap::fromImage(small));
    label.show();

    // close files
    if (write_histfile)
    {
        outfile.close();
    }

    // write image
    if (save_image)
    {
        if (!image.save(image_filename))
        {
            fprintf(stderr, "Failed saved image to %s\n", qPrintable(image_filename));
        }
    }

    return app.exec();
}
