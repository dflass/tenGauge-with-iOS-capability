#include "mainwindow.h"
#include <QApplication>

#if defined(Q_OS_IOS)
extern "C" int qtmn(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    QApplication a(argc, argv);

    //MainWindow Initialization
    MainWindow w;
    w.setWindowTitle("Dr. Jackson's Boat");
    w.show();
    //**************************

    return a.exec();
}

