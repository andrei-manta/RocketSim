#include "RocketSim.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RocketSim w = RocketSim();
    w.show();
    return a.exec();
}
