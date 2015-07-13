#include <QDebug>

#include "coverage2.h"

#include <QPainter>
#include <iostream>
#include <QFile>

void Coverage2::printHist() {
    std::cout << "Histogramm" << std::endl;
    QString a, b;
    a.append(QString("pixel \t"));
    b.append(QString("visits\t"));
    for(int i = 0 ; i < hist.size(); i++) {
        a.append(QString::number(hist.at(i)));
        a.append(QString("\t"));
        b.append(QString::number(i+1));
        b.append(QString("\t"));
    }

    std::cout << b.toStdString() << std::endl;
    std::cout << a.toStdString() << std::endl;


}

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
        QList<Pixel *>  a;
        //map.append(QList<Pixel> a);
        for(int y = 0; y < height; y++) {
            bool blocked = (QColor(this->scenarioImage->pixel(x, y)) == Qt::red);
            if(!blocked) passablePx++;
            Pixel * p = new Pixel(blocked);
            a.append(p);
        }
        map.append(a);
    }
    //qDebug() << "Passable pixel:" << passablePx << "of" << height*width << "in map";
}

void Coverage2::updateMap(int time, int xp, int yp) {
    //qDebug() << Q_FUNC_INFO << time << xp << yp;

    updated = false;
    int r = this->diameterInPX/2;

    for(int x = xp-r ; x < xp+r; x++) {
        if(x < 0 || x >= map.size()) continue;
        for(int y = yp-r ; y < yp+r; y++) {
            QList<Pixel *> ymap = map.at(x);
            if(y < 0 || y >= ymap.size()) continue;
            int a = x - xp;
            int b = y - yp;
            if (a*a + b*b <= r*r) {
                Pixel * p = ymap.at(y);
                p->update(time);
            }
        }
    }

   /*
    foreasch pixel in map
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
    if(updated) return;

    visitedPx = 0;

    hist.clear();

    foreach(QList<Pixel *> l , map) {
        foreach(Pixel * p , l) {
            if(!p->passable) continue;

            if(p->numVisited != 0) {
                visitedPx++;
                for(int i = 0; i<p->numVisited;i++) {
                    if(hist.size() <= (int)i) hist.append(0);
                    hist.operator [](i)++;
                    //hist.at[i]++;
                }
            }
        }
    }
    //qDebug() << "hist visited:" <<  visitedPx;
    //qDebug() << "histmax" << hist.size();
    //qDebug() << "hist[0]" << hist.at(0);

    if(hist.isEmpty()) {
        hist.append(-1);
    }
    updated = true;
}

double Coverage2::getCurrentCoveragePercent()
{

    calcHist();
    double perc = (double) hist.at(0) / passablePx ;

    //qDebug() << "currrentCperc" << perc;
    perc = perc * 10000;
    perc = double(qRound(perc)) / 100;

    return perc;
}

void Coverage2::updateHeatmap()
{
    calcHist();
    this->coverageImage = new QImage(width, height, QImage::Format_ARGB32);
    this->coverageImage->fill(Qt::transparent);

    QList<QRgb> l;
    l.append(qRgb(166,247,247));
    l.append(qRgb(126, 207, 247));
    l.append(qRgb(46, 112, 219));
    l.append(qRgb(23, 35, 207));
    l.append(qRgb(114, 0, 171));

    for(int x = 0; x < map.size(); x++ ) {
        QList<Pixel *> ym = map.at(x);
        for(int y = 0; y < ym.size(); y++) {
            Pixel * p = ym.at(y);
            if(p->numVisited > 0) {
                QRgb c;
                if(p->numVisited+1 > l.size()) {
                    c = l.back();
                } else {
                    c = l.at(p->numVisited);
                }
                coverageImage->setPixel(x,y,c);
            }

        }
    }


}

void Coverage2::exportCurrentHeatmap()
{
    updateHeatmap();
    QFile file("coverage.png");
    file.open(QIODevice::WriteOnly);
    this->coverageImage->save(&file, "PNG");
}

void Coverage2::exportScenarioAndHeatmap(QString filename)
{
    updateHeatmap();
    QImage overlay = createImageWithOverlay(*this->coverageImage, *this->scenarioImage);
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    overlay.save(&file, "PNG");
}

