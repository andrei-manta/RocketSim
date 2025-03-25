#include "RocketSim.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RocketSim w = RocketSim();
    w.move(100, 100);
    w.show();
    a.exec();
    return 0;
}
