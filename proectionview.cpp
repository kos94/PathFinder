#include "proectionview.h"
#include "QPainter"

ProectionView::ProectionView(PathCalculator* pc,QWidget *parent) :
    QWidget(parent)
{
    PC=pc;
    int frames=20;
    for(int i=0;i<frames;i++)
    {
    pc->rotateOn(360/frames);
    double maxh=pc->getPoint(0).x;
     minh=pc->getPoint(0).x;
    double maxw=pc->getPoint(0).y;
     minw=pc->getPoint(0).y;
    int count=pc->getRecNum();
    for(int i=0;i<count;i++)
    {
        if(pc->getPoint(i).x>maxh)
            maxh=pc->getPoint(i).x;
        if(pc->getPoint(i).x<minh)
            minh=pc->getPoint(i).x;
        if(pc->getPoint(i).y>maxw)
            maxw=pc->getPoint(i).y;
        if(pc->getPoint(i).y<minw)
            minw=pc->getPoint(i).y;
    }
    k=maxh-minh;
    double temp=maxw-minw;
    if(temp>k)
        k=temp;

    renderToFile();
    name_count++;
    }
}
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
     paint->end ();
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
