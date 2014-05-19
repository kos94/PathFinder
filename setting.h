#ifndef SETTING_H
#define SETTING_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>

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
signals:

public slots:
    void getFilePath();
    void hasGiro();
    void start();
};

#endif // SETTING_H
