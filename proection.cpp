#include "proection.h"
#include "QGridLayout"
#include "proectionview.h"
Proection::Proection(PathCalculator* pc,QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *layout=new QGridLayout;
    ProectionView *pv=new ProectionView(pc);
    layout->addWidget(pv,0,0,1,1);
    setLayout(layout);
    setWindowTitle(pv->getTypeLabel());
    setMinimumSize(230,230);
}
