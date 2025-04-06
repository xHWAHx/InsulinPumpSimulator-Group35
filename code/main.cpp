#include <QApplication>
#include "device.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Device *device = new Device();
    device->show();

    return a.exec();

    delete device;
}
