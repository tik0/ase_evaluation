#ifndef COVERAGE2_H
#define COVERAGE2_H

#include <QList>
#include <QImage>

class Pixel {
public:
    Pixel(bool blocked, int thresh = 10) {
       lastVisited = -thresh;
       numVisited = 0;
       passable = !blocked;
       updateThreshhold = thresh;
    }
    void update(int time) {
        if (time - lastVisited  <= updateThreshhold ) {

        } else {
            numVisited++;
        }
        lastVisited = time;
    }

    int lastVisited;
    int numVisited;
    bool passable;
    int updateThreshhold;
};

class Coverage2
{

public:
    explicit Coverage2(int height, int width, int diameterInCM);
    void updateMap(int time, int x, int y);

    double getCurrentCoveragePercent();
    void printHist();

private:
    int height, width, diameterInPX;
    QList<QList<Pixel * > > map;
    QList<int> hist;

    QImage *coverageImage;
    QImage *scenarioImage;
    QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage);

    int passablePx;
    int visitedPx;
    double perc;

    void calcHist();

};

#endif // COVERAGE2_H
