#include "globals.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Globals::Initialize();
    return a.exec();
}
