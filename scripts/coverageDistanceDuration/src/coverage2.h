#ifndef COVERAGE2_H
#define COVERAGE2_H

#include <QList>
#include <QImage>

class Pixel {
public:
    Pixel(bool blocked, uint thresh = 5) {
       lastVisited = 0;
       numVisited = 0;
       passable = !blocked;
       updateThreshhold = thresh;
    }
    void update(uint time) {
        if (lastVisited - 10 == time) {

        } else {
            numVisited++;
        }
        lastVisited = time;
    }

    uint lastVisited;
    uint numVisited;
    bool passable;
    uint updateThreshhold;
};

class Coverage2
{

public:
    explicit Coverage2(int height, int width, int diameterInCM);
    void updateMap(uint time, uint x, uint y);

    double getCurrentCoveragePercent();

private:
    int height, width, diameterInPX;
    QList<QList<Pixel> > map;
    QList<uint> hist;

    QImage *coverageImage;
    QImage *scenarioImage;
    QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage);

    uint passablePx;
    uint visitedPx;
    double perc;

    void calcHist();

};

#endif // COVERAGE2_H
