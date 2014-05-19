#ifndef AXISWAY_H
#define AXISWAY_H

#include <vector>
#include <math.h>
#include <QDebug>

using namespace std;

class Vector3D {
public:
    double x,y,z;
    void rotate( double x11,double x12,double x13,
            double x21,double x22,double x23,
            double x31,double x32,double x33);
};

struct acceleratorRecord {
    double a;
    double t;
};

class AxisWay {
private:
    static const float ACCELERATION_NOISE = 0.0;
    vector < acceleratorRecord > accelerations;
    vector < double > coords;
    double a0, a, v, s, t0;

public:
    AxisWay(double a0, double t0);
    void setDefaultA(double aDef) { a0 = aDef; }
    void addNextAcceleration(double ai, double ti);
    void calcCoords();
    void printPathCoords();
    double getCoord(int index) { return coords.at(index); }
    double getCoordsNum() { return coords.size(); }
};

#endif // AXISWAY_H
