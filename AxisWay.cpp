#include "AxisWay.h"

AxisWay::AxisWay(double a0, double t0) {
    a = v = s = 0.0;
    this->a0 = a0;
    this->t0 = t0;
    qDebug("a default: %lf. a: %lf. v: %lf. s: %lf", a0, a, v, s);
    //coords.push_back( s );
    //accelerations.push_back( a );
}

void AxisWay::addNextAcceleration(double ai, double ti) {
    acceleratorRecord ar;
    ar.a = ai; ar.t = ti;
    accelerations.push_back( ar );
}

void AxisWay::calcCoords(bool IOS,double ACCELERATION_NOISE) {
    double ai, v, s, t, ti, dt;
    int recNum = accelerations.size();

    ai = v = s = 0;
    t = t0;
    for(int i=0; i<recNum; i++) {
        ti = accelerations.at(i).t;
        ai = accelerations.at(i).a - a0;
        if( fabs(ai) < ACCELERATION_NOISE ) { ai = 0.0; }

        dt = (ti - t);
        if( !IOS )
            dt /= 1000;
        s += v * dt  + ai * dt * dt / 2;
        coords.push_back( s );
        v += ai * dt;
        t = ti;
    }
}

void AxisWay::printPathCoords() {
    int n = coords.size();
    for(int i=0; i<n; i++) {
        qDebug()<<coords.at(i);
    }
}
