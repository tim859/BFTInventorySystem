//#include "BodyFixTherapiesSystem.h"
//#include <QtWidgets/QApplication>
//#include <QFile>
//#include <QDir>
//
//int main(int argc, char* argv[])
//{
//    QApplication a(argc, argv);
//
//    // Get the directory of the application
//    QString appDir = QApplication::applicationDirPath();
//
//    // Construct the relative path to the QSS file
//    QString relativePath = appDir + "/BFTStyleSheet.qss";
//
//    // Open the stylesheet file
//    QFile file(relativePath);
//    if (file.open(QFile::ReadOnly)) {
//        QString styleSheet = QLatin1String(file.readAll());
//        a.setStyleSheet(styleSheet);
//    }
//    else {
//        std::cout << "QSS Stylesheet not found.";
//    }
//
//    BodyFixTherapiesSystem w;
//    w.resize(1600, 1000);
//    w.show();
//    return a.exec();
//}


#include "BodyFixTherapiesSystem.h"
#include <QtWidgets/QApplication>
#include <qdir.h>

int main(int argc, char* argv[])
{
    QFile file("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/BFTStyleSheet.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    QApplication a(argc, argv);
    BodyFixTherapiesSystem w;

    a.setStyleSheet(styleSheet);

    w.resize(1600, 1000);
    w.show();
    return a.exec();
}
