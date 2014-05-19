#ifndef PATHCALCULATOR_H
#define PATHCALCULATOR_H

#include <QObject>
#include "AxisWay.h"

class PathCalculator {
private:
    char filePath[256];
    static const int NUM_CALIBRATION_FRAMES = 3;
    static const int ROW_LENGTH = 4;
    static const int MAX_REC_NUM = 1000;
    int recNum;
    AxisWay *wx, *wy, *wz;

public:
    PathCalculator(char* accelFilePath);
    ~PathCalculator();
    void readData();
    void calcData();
    inline int getRecNum() { return wx->getCoordsNum(); }
    Vector3D getPoint(int index);
};

#endif // PATHCALCULATOR_H
