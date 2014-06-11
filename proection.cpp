#include "proection.h"
#include "QGridLayout"
#include "proectionview.h"
Proection::Proection(PathCalculator* pc,QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *layout=new QGridLayout;

    layout->addWidget(new ProectionView(pc),0,0,1,1);

    setLayout(layout);
    setWindowTitle("Proection");
    setMinimumSize(330,330);
     setMaximumSize(330,330);
}
