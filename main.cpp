#include "globals.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QObject::connect(&a, &QApplication::aboutToQuit, [](){Globals::AboutToQuit();});
    Globals::Initialize();
    return a.exec();
}
