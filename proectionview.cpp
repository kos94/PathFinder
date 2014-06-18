#include "proectionview.h"
#include "QPainter"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QtCore/qmath.h>
#include <iostream>

using namespace cv;
using namespace std;

ProectionView::ProectionView(PathCalculator* pc,QWidget *parent) :
    QWidget(parent)
{
    PC=pc;
    RotationForRender(20);
    QStringList str;
    str<<"./8.xml"<<"./circle.xml"<<"./triangle.xml";
    cascadeCalc(str);
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
        kh=maxh-minh;
        kw=maxw-minw;
        if(kh>kw)
            k=kh;
        else
            k=kw;

        renderToFile();
        name_count++;
    }
}
int TriOrQu(String patch){
    qDebug()<<"triorqu";
    Mat image;
    image=imread(patch);
    if(!image.data)
        return 0;

    RNG rng(12345);

    cvtColor(image,image,CV_RGB2GRAY);
    vector<vector<Point> > con;
    vector <Vec4i> hierarchy;
    //blur(image,image,Size(1,1));
    Canny(image,image,50,100*2,5);
    Mat im=imread(patch);

    findContours(image,con,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    int counter=0;
    vector<double> angle;
    for (int i = 0; i < con.size(); ++i) {
        vector<Point> ic=con.at(i);

        if((ic.size()>10)&&(ic.size()<80))
        {
            double x1=0;
            double y1=0;
            double x2=100;
            double y2=100;
            for (int j = 0; j < ic.size(); ++j) {
                if(x1<ic.at(j).x)
                {
                    x1=ic.at(j).x;
                    y1=ic.at(j).y;
                }
            }
            for (int j = 0; j < ic.size(); ++j) {
                if(x2>ic.at(j).x)
                {
                    x2=ic.at(j).x;
                    y2=ic.at(j).y;
                }
            }
            double an=qAtan2(ic.at(0).y-y1,ic.at(0).x-x1)/M_PI*180;
            bool t_b=true;
            for (int j = 0; j < angle.size(); ++j) {
                double an1=angle.at(j);
                if((an+5>an1)&&(an-5<an1))
                    t_b=false;
            }
            if(t_b)
                angle.push_back(an);
            stringstream s;
            s<<i;
            string name=s.str();
            Scalar color=Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
            /*drawContours(im,con,i,color,2,8,hierarchy,0,Point());
            namedWindow(name);
            imshow(name,im);*/
            counter++;
        }
    }
    if((angle.size()>1)&&(angle.size()<5))
        return angle.size();
    return 0;
}
bool ProectionView::isEnd(int i,int step){
    int ang=30;
    if((i<step*3)||(i+step>PC->getRecNum()-1))
        return false;
    Vector3D pr=PC->getPoint(i-step*2);
    Vector3D tec=PC->getPoint(i-step);
    Vector3D tstep=PC->getPoint(i);
    double an1=qAtan2(pr.y-tec.y,pr.x-tec.x)/M_PI*180;
    double an2=qAtan2(tec.y-tstep.y,tec.x-tstep.x)/M_PI*180;
    if(!((an2+30>an1)&&(an2-30<an1)))
        return true;
    return false;
}


void ProectionView::cascadeCalc(QStringList cascade_names){
    //QString cascade_names=str;
    /** OPENCV*/
    Mat image;
    image=imread("./1.bmp");
    imwrite("./34.jpg",image);
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
        label="";
        for (int i = 0; i < cascade_names.length(); ++i) {
            int t=5;
            vector<Rect> logos;

            int f=cascade_names[i].lastIndexOf("/")+1;
            QString name=cascade_names[i].mid(f,cascade_names[i].length()-4-f);
            cascades[i].detectMultiScale(image, logos,1.1,5,0,Size(55,55),Size(60,60));
            label=QString("%1%2 : %3 \n").arg(label,name,QString::number(logos.size()));
        }
    }
    delete[] cascades;

    label="";

    int step=5;
    int stepC=0;
    int csafa=0;
    for(int i=1;i<PC->getRecNum();i++)
    {
        if((stepC>step*2)&&(isEnd(i,step)))
        {
            stepC=0;
            csafa++;
        }
        stepC++;
    }
    switch (csafa) {
    case 2:
        label+="Triangle / ";
        break;
    case 3:
        label+="Square / ";
        break;
    default:
        label+="- / ";
        break;
    }

    /** OpenCV*/
    int tri=0;
    int sq=0;
    for (int i = 0; i < 20; i++) {
        QString name=QString("./%1.bmp").arg(QString::number(i+1));

        int rez=TriOrQu(name.toStdString());
        switch (rez) {
        case 3:
            tri++;
            break;
        case 0:

            break;
        default:
            sq++;
            break;
        }
    }

    qDebug()<<"tri"<<tri<<"\n";
    qDebug()<<"sq="<<sq<<"\n";
    if(tri>=10)
        label+="Triangle";
    else if(sq>=10)
        label+="Square";
    else
        label+="-";
}




/** Drawing proection*/
void ProectionView::paintEvent(QPaintEvent* e)
{
    double kk=300/k;
    double kkh=300/kh;
    double kkw=300/kw;
    QPainter *paint = new QPainter (this);
    paint->begin (this);
    int count=PC->getRecNum();
    Vector3D **m=new Vector3D*[count];
    for(int i=0;i<count;i++)
    {
        m[i]=new Vector3D();
        m[i]->setVector((PC->getPoint(i).x-minh)*kkh,(PC->getPoint(i).y-minw)*kkw,0);
    }
    int step=5;
    int stepC=0;
    int csafa=0;
    for(int i=1;i<count;i++)
    {
        if((stepC>step*2)&&(isEnd(i,step)))
        {
            paint->setPen(QPen(QColor(rand()%200,rand()%200,rand()%200),3));
            stepC=0;
            csafa++;
        }
        paint->drawLine(QLine(m[i-1]->x,m[i-1]->y,m[i]->x,m[i]->y));
        stepC++;
    }
      paint->setPen(QColor(0,0,0));
    qDebug()<<"!!!COUNT_P = "<<csafa<<"\n";
    paint->drawText(QRect(0,250,300,100),Qt::AlignLeft,label);
    paint->end();
}
void ProectionView::renderToFile(){
    double SIZE=50;
    QImage z(SIZE,SIZE,QImage::Format_ARGB32);
    // SIZE-=50;
    z.fill(0xFFFFFF);
    QPainter p;
    p.begin(&z);
    p.setPen(Qt::black);
    double kk=SIZE/k;
    double kkh=SIZE/kh;
    double kkw=SIZE/kw;
    int count=PC->getRecNum();
    Vector3D **m=new Vector3D*[count];
    for(int i=0;i<count;i++)
    {
        m[i]=new Vector3D();
        m[i]->setVector((PC->getPoint(i).x-minh)*kkh,(PC->getPoint(i).y-minw)*kkw,0);
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
