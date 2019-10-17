#include "globals.h"
#include <QApplication>
#include "deliveryboy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeliveryBoy boy;
    boy.FetchWord("test", "en", "ru");
    a.setQuitOnLastWindowClosed(false);
    QObject::connect(&a, &QApplication::aboutToQuit, [](){Globals::AboutToQuit();});
    Globals::Initialize();
    return a.exec();
}
