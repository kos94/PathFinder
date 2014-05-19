#include "scene3D.h"
#include "setting.h"
#include <unistd.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    Setting settings;
    settings.setGeometry( 100, 100, 200, 120 );
    app.setActiveWindow(&settings);
    settings.show();
    return app.exec();
}
