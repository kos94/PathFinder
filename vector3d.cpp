#include "vector3d.h"

void Vector3D::rotate( Matrix3x3 m ) {
    x = ( x * m.x11 + y * m.x12 + z * m.x13 );
    y = ( x * m.x21 + y * m.x22 + z * m.x23 );
    z = ( x * m.x31 + y * m.x32 + z * m.x33 );
}

void Vector3D::setVector(Vector3D v1) {
    x = v1.x;
    y = v1.y;
    z = v1.z;
}

void Vector3D::setVector(double x1, double y1, double z1) {
    x = x1;
    y = y1;
    z = z1;
}

void Vector3D::substract( Vector3D v1 ) {
    x -= v1.x;
    y -= v1.y;
    z -= v1.z;
}

void AcceleratorRecord::deleteNoise( double upperValue ) {
    if( fabs( x ) < upperValue ) { x = 0.0; }
    if( fabs( y ) < upperValue ) { y = 0.0; }
    if( fabs( z ) < upperValue ) { z = 0.0; }
}

void LinearMovement::calcNextCoord(double ai, double dt) {
    a = ai;
    s += v * dt + a * dt * dt / 2;
    v += a * dt;
}
