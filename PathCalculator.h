#ifndef PATHCALCULATOR_H
#define PATHCALCULATOR_H

#include <QObject>
#include "vector3d.h"

class PathCalculator {
private:
    char filePath[256];
    int NUM_CALIBRATION_FRAMES;
    double LOW_BORDER_VALUE;
    bool GYRO;
    bool IOS;
    static const int ROW_LENGTH = 4;
    static const int MAX_REC_NUM = 1000;
    int recNum;
    AcceleratorRecord initRecord;
    vector < AcceleratorRecord > records;
    vector < Vector3D > points;

public:
    PathCalculator(char* accelFilePath,
                   bool ios,
                   int framesNumbers,
                   double lowPass,
                   bool gyro);
    ~PathCalculator();
    int getRecNum() { return points.size(); }
    Vector3D getPoint(int index);
    void rotateOn(double degree);
    void oldRotateMetod();
    void newRotateMetod();//Grabluk

protected:
    void readData();
    void readInitialRecord(FILE* f);
    void changeData();
    void calcData();
};

#endif // PATHCALCULATOR_H
