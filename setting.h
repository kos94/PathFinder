#ifndef SETTING_H
#define SETTING_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QFileDialog>
#include <QApplication>

#include "scene3D.h"
#include "PathCalculator.h"

#include "server.h"

class Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = 0);
    QLineEdit *fileAdress;
    QCheckBox *ios;
    QCheckBox *gyro;
    QSpinBox *framesCouns;
    // QSpinBox *linFrame;
    QDoubleSpinBox *lowPass;
    // QPushButton *browse;
    QPushButton *startServer;
    Server *server;
signals:

public slots:
    void getFilePath();//Open filebrowser
    void hasGyro();//Show checkbox if file has gyrodata
    void start();//Start pacthCalculator and show graph
    void startStopServer();
};

#endif // SETTING_H
