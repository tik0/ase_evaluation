#include <QDebug>

#include "coverage2.h"

#include <QPainter>



Coverage2::Coverage2(int height, int width, int diameterInCM) {

    this->width = width;
    this->height = height;
    this->diameterInPX = diameterInCM * 3;

    this->coverageImage = new QImage(width, height, QImage::Format_ARGB32);
    this->coverageImage->fill(Qt::transparent);

    this->scenarioImage = new QImage("scenario_with_walls.png", "png");

    passablePx = 0;
    map.clear();
    for(int x = 0; x < width; x++) {
        QList<Pixel>  a;
        //map.append(QList<Pixel> a);
        for(int y = 0; y < height; y++) {
            bool blocked = (QColor(this->scenarioImage->pixel(x, y)) == Qt::red);
            if(!blocked) passablePx++;
            Pixel p(blocked);
            a.append(p);
        }
        map.append(a);
    }
}

void Coverage2::updateMap(uint time, uint x, uint y) {
    qDebug() << Q_FUNC_INFO << " not implemented";
    /*
    foreach pixel in map
            if pixel in circle around x y
                pixel.update(time)
    */
}

QImage Coverage2::createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage)
{
    QImage imageWithOverlay = QImage(baseImage.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, baseImage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, overlayImage);

    painter.end();

    return imageWithOverlay;
}

void Coverage2::calcHist() {

    visitedPx = 0;

    hist.clear();

    foreach(QList<Pixel> l , map) {
        foreach(Pixel p , l) {
            if(!p.passable) continue;

            if(p.numVisited != 0) {
                visitedPx++;
                for(uint i = 0; i<p.numVisited;i++) {
                    if(hist.size() <= (int)i) hist.append(0);
                    hist.operator [](i)++;
                    //hist.at[i]++;
                }
            }
        }
    }
}

double Coverage2::getCurrentCoveragePercent()
{
    calcHist();
    double perc = (double) passablePx / hist.at(0);

    perc = perc * 10000;
    perc = double(qRound(perc)) / 100;

    return perc;
}

