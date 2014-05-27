#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "PathCalculator.h"



PathCalculator::PathCalculator(char* accelFilePath,
                                   bool ios,
                                   int framesNumbers,
                                   double lowPass,
                                   bool gyro) {
    NUM_CALIBRATION_FRAMES = framesNumbers;
    IOS = ios;
    GYRO = gyro;
    LOW_BORDER_VALUE = lowPass;
    strcpy(filePath, accelFilePath);
    readData();
    //changeData();
    calcData();
}

PathCalculator::~PathCalculator() {
    //DO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void PathCalculator::readData() {
    AcceleratorRecord rec;
    Matrix3x3 m;

    qDebug(">%s<", filePath);
    FILE* fr = fopen(filePath, "rt");
    assert( fr != NULL );
    qDebug("OPENED");

    readInitialRecord( fr );

    int counter = 0;
    int temp;

    while( counter <= MAX_REC_NUM ) {
        if(IOS) {
            temp = fscanf(fr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
                   &rec.x, &rec.y, &rec.z, &m.x11, &m.x12, &m.x13,
                   &m.x21, &m.x22, &m.x23, &m.x31, &m.x32, &m.x33, &rec.t);
        } else {
            temp = fscanf(fr, "%lf %lf %lf %lf ", &rec.x, &rec.y, &rec.z, &rec.t);
        }
        if( temp == EOF ) { break; }

        records.push_back( rec );
        rec.print();
        counter++;
    }

    fclose( fr );
}

void PathCalculator::readInitialRecord(FILE* f) {
    AcceleratorRecord rec;
    Matrix3x3 m;
    AcceleratorRecord initialRecord;
    double tPrev;

    if( !NUM_CALIBRATION_FRAMES ) return;
    for(int i=0; i<NUM_CALIBRATION_FRAMES; i++) {
        if(IOS) {
            fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
                   &rec.x, &rec.y, &rec.z, &m.x11, &m.x12, &m.x13,
                   &m.x21, &m.x22, &m.x23, &m.x31, &m.x32, &m.x33, &tPrev);
        } else {
            fscanf(f, "%lf %lf %lf %lf ", &rec.x, &rec.y, &rec.z, &tPrev);
        }

        if(GYRO) {
            rec.rotate( m );
        }

        qDebug("> %lf %lf %lf %lf ", rec.x, rec.y, rec.z, tPrev);
        initialRecord.x += rec.x;
        initialRecord.y += rec.y;
        initialRecord.z += rec.z;
    }

    initialRecord.x /= NUM_CALIBRATION_FRAMES;
    initialRecord.y /= NUM_CALIBRATION_FRAMES;
    initialRecord.z /= NUM_CALIBRATION_FRAMES;
    initialRecord.t = tPrev;

    initRecord = initialRecord;
    qDebug("INITIAL RECORD ADDED: %lf %lf %lf",
           initRecord.x, initRecord.y, initRecord.z);
}


void PathCalculator::changeData() {
    /* НЕ РАБОТАЕТ НОРМАЛЬНО. если ставить его, то нужно убирать
     * cur -> substract( initRecord ); в calcData*/
    AcceleratorRecord* rec;
    Vector3D gravity;
    Vector3D newRec;
    const double alpha = 0.8;
    int recNum = records.size();
    gravity.setVector( initRecord );
    for(int i=0; i<recNum; i++) {
        rec = & (records.at(i));

        gravity.x = alpha * gravity.x + (1 - alpha) * rec -> x;
        gravity.y = alpha * gravity.y + (1 - alpha) * rec -> y;
        gravity.z = alpha * gravity.z + (1 - alpha) * rec -> z;

        rec -> x -= gravity.x;
        rec -> y -= gravity.y;
        rec -> z -= gravity.z;
    }
}

void PathCalculator::calcData() {
    Vector3D point;
    AcceleratorRecord* cur;
    LinearMovement wx, wy, wz;
    double ai, v, s, t, ti, dt;
    int recNum = records.size();

    t = initRecord.t;
    for(int i=0; i<recNum; i++) {
        cur = &( records.at(i) );
        ti = cur -> t;

        cur -> substract( initRecord );
        cur -> deleteNoise( LOW_BORDER_VALUE );

        dt = (ti - t);
        if( !IOS ) {
            dt /= 1000;
        }

        wx.calcNextCoord( cur -> x, dt );
        wy.calcNextCoord( cur -> y, dt );
        wz.calcNextCoord( cur -> z, dt );

        point.setVector(wx.s, wy.s, wz.s);
        points.push_back( point );

        t = ti;
    }
}

Vector3D PathCalculator::getPoint(int index) {
    assert( 0 <= index && index < points.size() );
    return points.at( index );
}
