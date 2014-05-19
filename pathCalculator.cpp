#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "PathCalculator.h"

void Vector3D::rotate( double x11,double x12,double x13,
        double x21,double x22,double x23,
        double x31,double x32,double x33) {
    x = ( x * x11 + y * x12 + z * x13 );
    y = ( x * x21 + y * x22 + z * x23 );
    z = ( x * x31 + y * x32 + z * x33 );
}

PathCalculator::PathCalculator(char* accelFilePath,
                                   bool ios,
                                   int framesNumbers,
                                   double lowPass,
                                   bool gyro) {
    NUM_CALIBRATION_FRAMES = framesNumbers;
    IOS = ios;
    GYRO = gyro;
    LOW_PASS_VALUE = lowPass;
    strcpy(filePath, accelFilePath);
    readData();
    calcData();
}

PathCalculator::~PathCalculator() {
    delete wx;
    delete wy;
    delete wz;
}

void PathCalculator::readData() {
    Vector3D a1;
    double ax0, ay0, az0;
    double x11, x12, x13, x21, x22, x23, x31, x32, x33;
    double tPrev, t;

    FILE* fr = fopen(filePath, "rt");
    assert( fr != NULL );
    qDebug("OPENED");
    ax0 = ay0 = az0 = 0.0;
    if( NUM_CALIBRATION_FRAMES ) {
        for(int i=0; i<NUM_CALIBRATION_FRAMES; i++) {
            if(IOS)
            fscanf(fr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
                   &a1.x, &a1.y, &a1.z, &x11, &x12, &x13, &x21, &x22, &x23, &x31, &x32, &x33, &tPrev);
            else
                fscanf(fr, "%lf %lf %lf %lf ", &a1.x, &a1.y, &a1.z, &tPrev);
            if(GYRO)
                a1.rotate(x11, x12, x13, x21, x22, x23, x31, x32, x33);
            ax0 += a1.x;
            ay0 += a1.y;
            az0 += a1.z;
        }
        ax0 /= NUM_CALIBRATION_FRAMES;
        ay0 /= NUM_CALIBRATION_FRAMES;
        az0 /= NUM_CALIBRATION_FRAMES;
    }

    wx = new AxisWay( ax0, tPrev );
    wy = new AxisWay( ay0, tPrev );
    wz = new AxisWay( az0, tPrev );

    int counter = 0;
    int temp;
    if(IOS)
        temp = fscanf(fr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
                      &a1.x, &a1.y, &a1.z, &x11, &x12, &x13, &x21, &x22, &x23, &x31, &x32, &x33, &t);
    else
        temp = fscanf(fr, "%lf %lf %lf %lf ", &a1.x, &a1.y, &a1.z, &t);
    while( temp != EOF ) {

        wx->addNextAcceleration(a1.x, t);
        wy->addNextAcceleration(a1.y, t);
        wz->addNextAcceleration(a1.z, t);
        counter++;
        if( counter > MAX_REC_NUM ) break;
        if(IOS)
            temp = fscanf(fr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
                          &a1.x, &a1.y, &a1.z, &x11, &x12, &x13, &x21, &x22, &x23, &x31, &x32, &x33, &t);
        else
            temp = fscanf(fr, "%lf %lf %lf %lf ", &a1.x, &a1.y, &a1.z, &t);
    }
    fclose(fr);
}

void PathCalculator::calcData() {
    wx->calcCoords(IOS,LOW_PASS_VALUE);
    wy->calcCoords(IOS,LOW_PASS_VALUE);
    wz->calcCoords(IOS,LOW_PASS_VALUE);
}

Vector3D PathCalculator::getPoint(int index) {
    Vector3D p;
    p.x = wx->getCoord(index);
    p.y = wy->getCoord(index);
    p.z = wz->getCoord(index);
    return p;
}
