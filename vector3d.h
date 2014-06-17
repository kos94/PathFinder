#ifndef AXISWAY_H
#define AXISWAY_H

#include <vector>
#include <math.h>
#include <QDebug>

using namespace std;

class Matrix3x3 {
public:
    double x11, x12, x13;
    double x21, x22, x23;
    double x31, x32, x33;
    Matrix3x3(double X11, double X12, double X13,
              double X21, double X22, double X23,
              double X31, double X32, double X33){
        Init(X11, X12, X13,
             X21, X22, X23,
             X31, X32, X33);

    }
    Matrix3x3(double ox,double oy,double oz){
       double ch = cos(oy);//heading
       double sh = sin(oy);
       double ca = cos(oz);//altitude
       double sa = sin(oz);
       double cb = cos(ox);//bank
       double sb = sin(ox);
       Init(ch * ca,sh*sb - ch*sa*cb,ch*sa*sb + sh*cb,
                 sa,ca*cb,-ca*sb,-sh*ca,sh*sa*cb + ch*sb,-sh*sa*sb + ch*cb);
    }
    Matrix3x3(){
        x11=0;
        x12=0;
        x12=0;
        x21=0;
        x22=0;
        x22=0;
        x31=0;
        x32=0;
        x32=0;
    }

private:void Init(double X11, double X12, double X13,
                  double X21, double X22, double X23,
                  double X31, double X32, double X33){
        x11=X11;        x12=X12;        x13=X13;
        x21=X21;        x22=X22;        x23=X23;
        x31=X31;        x32=X32;        x33=X33;
    }
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
