#include "BodyFixTherapiesSystem.h"
#include <QtWidgets/QApplication>
#include <qdir.h>

int main(int argc, char *argv[])
{
    QFile file("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/SpyBot.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    QApplication a(argc, argv);
    BodyFixTherapiesSystem w;

    a.setStyleSheet(styleSheet);

    w.resize(1600, 1000);
    w.show();
    return a.exec();
}
