#include "proectionview.h"
#include "QPainter"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

ProectionView::ProectionView(PathCalculator* pc,QWidget *parent) :
    QWidget(parent)
{
    PC=pc;
    RotationForRender(20);
    QStringList str;
    str<<"./8.xml"<<"./circle.xml"<<"./clock.xml"<<"./square.xml"<<"./triangle.xml";
    //cascadeCalc(str);
}
void ProectionView::RotationForRender(int frames){
    /** Rotation trajectory for image render*/
    for(int i=0;i<frames;i++)
    {
        PC->rotateOn(360/frames);
        double maxh=PC->getPoint(0).x;
        minh=PC->getPoint(0).x;
        double maxw=PC->getPoint(0).y;
        minw=PC->getPoint(0).y;
        int count=PC->getRecNum();
        for(int i=0;i<count;i++)
        {
            if(PC->getPoint(i).x>maxh)
                maxh=PC->getPoint(i).x;
            if(PC->getPoint(i).x<minh)
                minh=PC->getPoint(i).x;
            if(PC->getPoint(i).y>maxw)
                maxw=PC->getPoint(i).y;
            if(PC->getPoint(i).y<minw)
                minw=PC->getPoint(i).y;
        }
        k=maxh-minh;
        double temp=maxw-minw;
        if(temp>k)
            k=temp;

        renderToFile();
        name_count++;
    }
}
void ProectionView::cascadeCalc(QStringList cascade_names){
    //QString cascade_names=str;
    /** OPENCV*/
    Mat image;
    image=imread("./1.bmp");

    CascadeClassifier *cascades=new CascadeClassifier[cascade_names.length()];
    bool error=false;
    for (int i = 0; i < cascade_names.length(); ++i) {
        cascades[i].load(cascade_names[i].toUtf8().constData());
        if(cascades[i].empty())
        {
            label="Error : Cascades not found";
            error=true;
        }
    }

    if(!image.data)
    {
        label="Error : Image Import";
    }
    else if(!error)
    {
        for (int i = 0; i < cascade_names.length(); ++i) {
            vector<Rect> logos;
            cascades[i].detectMultiScale(image,logos,1.1,3,CV_HAAR_SCALE_IMAGE,Size(30,30));
            int f=cascade_names[i].lastIndexOf("/")+1;
            QString name=cascade_names[i].mid(f,cascade_names[i].length()-4-f);
            label=QString("%1%2 : %3 \n").arg(label,name,QString::number(logos.size()));
        }

    }
    delete[] cascades;
}




/** Drawing proection*/
void ProectionView::paintEvent(QPaintEvent* e)
{
    double kk=300/k;
    QPainter *paint = new QPainter (this);
    paint->begin (this);
    int count=PC->getRecNum();
    Vector3D **m=new Vector3D*[count];
    for(int i=0;i<count;i++)
    {
        m[i]=new Vector3D();
        m[i]->setVector((PC->getPoint(i).x-minh)*kk,(PC->getPoint(i).y-minw)*kk,0);
    }
    for(int i=1;i<count;i++)
    {
        paint->drawLine(QLine(m[i-1]->x,m[i-1]->y,m[i]->x,m[i]->y));
    }
    paint->drawText(QRect(0,250,300,100),Qt::AlignLeft,label);
    paint->end();
}
void ProectionView::renderToFile(){
    QImage z(100,100,QImage::Format_ARGB32);
    z.fill(0xFFFFFF);
    QPainter p;
    p.begin(&z);
    p.setPen(Qt::black);
    double kk=100/k;
    int count=PC->getRecNum();
    Vector3D **m=new Vector3D*[count];
    for(int i=0;i<count;i++)
    {
        m[i]=new Vector3D();
        m[i]->setVector((PC->getPoint(i).x-minh)*kk,(PC->getPoint(i).y-minw)*kk,0);
    }
    for(int i=1;i<count;i++)
    {
        p.drawLine(m[i-1]->x,m[i-1]->y,m[i]->x,m[i]->y);
    }
    p.end();
    QString name;
    name.setNum(name_count);
    z.save(name+".bmp");
}
