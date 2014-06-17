#include <stdio.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigenvalues>
#include "PathCalculator.h"


void PathCalculator::rotateOn(double degree)
{
    int pNum=getRecNum();
    Matrix3x3 *m=new Matrix3x3(0,0,(degree*M_PI)/180);
    for(int i=0;i<pNum;i++)
        this->points[i].rotate(*m);
}

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
    std::cout<<"------------------------";
    for (int i = 0; i < getRecNum(); ++i) {
        std::cout << this->getPoint(i).x << ", " << this->getPoint(i).y << ", " <<this->getPoint(i).z<< std::endl;
    }
    std::cout<<"------------------------";




    /** --------------------------------------- */
   // oldRotateMetod();
    newRotateMetod();
}

void PathCalculator::newRotateMetod(){
    using Eigen::MatrixXf;
    using Eigen::Vector3f;
    using Eigen::SelfAdjointEigenSolver;

    int len=getRecNum();
    Vector3f *data=new Vector3f[len];
    for (int i = 0; i < len; ++i) {
        Vector3D temp=this->getPoint(i);
        data[i]<<temp.x,temp.y,temp.z;
    }

    // compute the mean of the data
    Vector3f mean = Vector3f::Zero();
    for(int i = 0; i < len; ++i)
        mean += data[i];
    mean /= len;

    // compute the covariance matrix
    MatrixXf covMat = MatrixXf::Zero(3, 3);
    for(int i = 0; i < len; ++i)
    {
        Vector3f diff = (data[i] - mean).conjugate();
        covMat += diff * diff.adjoint();
    }
    SelfAdjointEigenSolver<MatrixXf> eig(covMat);
    Vector3f ei = eig.eigenvectors().col(2);


    std::cout << "x(t) = " << mean.x() << " + " << ei.x() << " * t" << std::endl;
    double x =mean.x()+ei.x()*20;
    std::cout << "y(t) = " << mean.y() << " + " << ei.y() << " * t" << std::endl;
    double y =mean.y()+ei.y()*20;
    std::cout << "z(t) = " << mean.z() << " + " << ei.z() << " * t" << std::endl;
    double z =mean.z()+ei.z()*20;

    double anB = atan(x / y);
    double arg = z / (cos(anB) * y + sin(anB) * x);
    double anA = atan( arg );
    Matrix3x3 *m=new Matrix3x3(0,-anA+M_PI_2,anB+M_PI_2);
    for(int i=0;i<len;i++)
        this->points[i].rotate(*m);
}

void PathCalculator::oldRotateMetod()
{
    int pNum=getRecNum();
    Vector3D lastP=this->points[pNum-1];

    double anB = atan(lastP.x / lastP.y);
    double arg = lastP.z / (cos(anB) * lastP.y + sin(anB) * lastP.x);
    double anA = atan( arg );
    Matrix3x3 *m=new Matrix3x3(0,-anA+M_PI_2,anB+M_PI_2);
    for(int i=0;i<pNum;i++)
        this->points[i].rotate(*m);
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
    double t, ti, dt;
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
