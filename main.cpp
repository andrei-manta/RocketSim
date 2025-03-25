#include "RocketSim.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RocketSim w = RocketSim();
    w.move(200, 10);
    w.show();
    a.exec();
    return 0;
}
