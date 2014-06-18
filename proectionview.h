#ifndef PROECTIONVIEW_H
#define PROECTIONVIEW_H

#include <QWidget>
#include "PathCalculator.h"

class ProectionView : public QWidget
{
    Q_OBJECT
    PathCalculator* PC;
    double k;
    double kw,kh;
    double minh,minw;
    QString label="Cascade calculation turned off";
    int name_count=1;
    QString type="-";
public:
    explicit ProectionView(PathCalculator*,QWidget *parent = 0);
        void paintEvent(QPaintEvent*);
        void renderToFile();
        void RotationForRender(int frames);
        void cascadeCalc(QStringList str);
        bool isEnd(int i,int step);
        bool isCircle();
        void setTypeLabel(QString newtype);
        QString getTypeLabel();
signals:

public slots:

};

#endif // PROECTIONVIEW_H
