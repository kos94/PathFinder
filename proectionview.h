#ifndef PROECTIONVIEW_H
#define PROECTIONVIEW_H

#include <QWidget>
#include "PathCalculator.h"

class ProectionView : public QWidget
{
    Q_OBJECT
    PathCalculator* PC;
    double k;
    double minh,minw;
    QString label="Cascade calculation turned off";
    int name_count=1;
public:
    explicit ProectionView(PathCalculator*,QWidget *parent = 0);
        void paintEvent(QPaintEvent*);
        void renderToFile();
        void RotationForRender(int frames);
        void cascadeCalc(QStringList str);
signals:

public slots:

};

#endif // PROECTIONVIEW_H
