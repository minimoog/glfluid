#include "myopenglwindow.h"
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    MyOpenGLWindow window;

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    //format.setMajorVersion(3);
    //format.setMinorVersion(2);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapInterval(1);
    window.setFormat(format);

    window.show();

    return a.exec();
}
