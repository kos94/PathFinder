#ifndef AXISWAY_H
#define AXISWAY_H

#include <vector>
#include <math.h>
#include <QDebug>

using namespace std;

struct Matrix3x3 {
    double x11, x12, x13;
    double x21, x22, x23;
    double x31, x32, x33;
};

class Vector3D {
public:
    double x,y,z;
    Vector3D() { x = y = z = 0; }
    void setVector(Vector3D v1);
    void setVector(double x1, double y1, double z1);
    void substract(Vector3D v1);
    void rotate( Matrix3x3 m );
    virtual void print() { qDebug("pt: %lf %lf %lf", x, y, z); }
};

class AcceleratorRecord : public Vector3D{
public:
    double t;
    virtual void print() { qDebug("rec: %lf %lf %lf %lf", x, y, z, t); }
    void deleteNoise(double upperValue);
};

class LinearMovement {
public:
    double v, s, a;
    LinearMovement() {
        v = s = a = 0;
    }
    void calcNextCoord( double ai, double dt );
};

#endif // AXISWAY_H
