#include "BodyFixTherapiesSystem.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BodyFixTherapiesSystem w;
    w.resize(1600, 1000);
    w.show();
    return a.exec();
}
