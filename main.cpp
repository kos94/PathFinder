#include "scene3D.h"
#include <unistd.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    //рыбка input-145413.agt
    char* filePath = "../Загрузки/android/!accelerometer/vertical/clock.txt";
    PathCalculator* pc = new PathCalculator( filePath );
    qDebug()<<"POINTS NUM FROM MAIN: "<<pc->getRecNum();

    QApplication a(argc, argv);
    Scene3D *scene = new Scene3D(pc);
    scene->show();
    a.exec();
   // unlink( filePath );
    return 0;
}
