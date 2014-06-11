#include "setting.h"
#include "proection.h"

#define YES true
#define NO false
#define StartServerString "Start Server"

Setting::Setting(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *layout=new QGridLayout;
    int line=0;

    layout->addWidget(new QLabel("File Open"),line,0,1,1);
    ios=new QCheckBox("iOS");
    layout->addWidget(ios,line,1,1,1);
    line++;

    fileAdress=new QLineEdit;
    layout->addWidget(fileAdress,line,0,1,1);
    QPushButton *browse=new QPushButton("Browse");
    connect(browse,SIGNAL(clicked()),SLOT(getFilePath()));
    layout->addWidget(browse,line,1,1,1);
    line++;

    startServer=new QPushButton(StartServerString);
    connect(startServer,SIGNAL(clicked()),SLOT(startStopServer()));
    layout->addWidget(startServer,line,0,1,2);
    line++;

    QFrame* separator = new QFrame();
    separator->setFrameStyle(QFrame::HLine);
    layout->addWidget(separator,line,0,1,2);
    line++;

    layout->addWidget(new QLabel("Settings"),line,0,1,2);
    line++;

    layout->addWidget(new QLabel("Count of calibration frames"),line,0,1,1);
    framesCouns=new QSpinBox();
    framesCouns->setValue(3);
    layout->addWidget(framesCouns,line,1,1,1);
    line++;

    layout->addWidget(new QLabel("Low-pass filter"),line,0,1,1);
    lowPass=new QDoubleSpinBox();
    lowPass->setSingleStep(0.01);
    layout->addWidget(lowPass,line,1,1,1);
    line++;

    layout->addWidget(new QLabel("Linear filter"),line,0,1,1);
    QSpinBox *linFrame=new QSpinBox();
    linFrame->setEnabled(NO);
    layout->addWidget(linFrame,line,1,1,1);
    line++;

    layout->addWidget(new QLabel("Gyro"),line,0,1,1);
    gyro=new QCheckBox();
    gyro->setEnabled(NO);
    connect(ios,SIGNAL(stateChanged(int)),SLOT(hasGyro()));
    layout->addWidget(gyro,line,1,1,1);
    line++;

    QPushButton* start=new QPushButton("Start");
    connect(start,SIGNAL(clicked()),SLOT(start()));
    layout->addWidget(start,line,0,1,2);
    line++;

    server=new Server((QObject *)this);

    setLayout(layout);
    setWindowTitle("Settings");
    setMinimumSize(300,300);
}

void Setting::getFilePath(){
    fileAdress->setStyleSheet("");
    fileAdress->setText(QFileDialog::getOpenFileName(this,
                                                     tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)")));
    if(fileAdress->text().length()>1)
    {
        FILE* fr = fopen(fileAdress->text().toUtf8().data(), "rt");
        char t[500];
        fgets(t,sizeof(t),fr);
        int c=0;
        for(int i=0;i<strlen(t);i++)
            if(t[i]==' ')
                c++;
        qDebug(t);
        if(c>4)
            ios->setChecked(YES);
        else
            ios->setChecked(NO);
        fclose(fr);
    }
}

void Setting::hasGyro(){
    if(ios->isChecked())
    {
        gyro->setEnabled(YES);
    }
    else{
        gyro->setChecked(NO);
        gyro->setEnabled(NO);
    }
}

void Setting::start(){
    if(fileAdress->text().length()>1)
    {
        fileAdress->setStyleSheet("");
        PathCalculator* pc= new PathCalculator(fileAdress->text().toUtf8().data(),
                                               ios->isChecked(),
                                               framesCouns->value(),
                                               lowPass->value(),
                                               gyro->isChecked());
        qDebug()<<"POINTS NUM FROM MAIN: "<<pc->getRecNum();
        Scene3D *scene = new Scene3D(pc);
        scene->show();
        Proection *pr= new Proection(pc);
        pr->show();
    }
    else
    {
        fileAdress->setStyleSheet("QLineEdit{background: red;}");
    }
}

void Setting::startStopServer(){
    if(!server->isStarted()){
    QString str=server->start();
    if(str=="error")
    {
        startServer->setText(StartServerString);
        QMessageBox::warning(this,"Error","Error: cant start server.",QMessageBox::Ok);
    }
    else
        startServer->setText("Stop "+str);
    }
    else
    {
        if(server->stop())
        {
            startServer->setText(StartServerString);
            fileAdress->setText("input_temp");
        }
        else
            QMessageBox::warning(this,"Error","Error: cant stop server.",QMessageBox::Ok);
    }
}
