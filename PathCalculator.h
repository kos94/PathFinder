#ifndef PATHCALCULATOR_H
#define PATHCALCULATOR_H

#include <QObject>
#include "AxisWay.h"

class PathCalculator {
private:
    char filePath[256];
    int NUM_CALIBRATION_FRAMES;
    double LOW_PASS_VALUE;
    bool GYRO;
    bool IOS;
    static const int ROW_LENGTH = 4;
    static const int MAX_REC_NUM = 1000;
    int recNum;
    AxisWay *wx, *wy, *wz;

public:
    PathCalculator(char* accelFilePath,
                   bool ios,
                   int framesNumbers,
                   double lowPass,
                   bool gyro);
    ~PathCalculator();
    void readData();
    void calcData();
    inline int getRecNum() { return wx->getCoordsNum(); }
    Vector3D getPoint(int index);
};

#endif // PATHCALCULATOR_H
