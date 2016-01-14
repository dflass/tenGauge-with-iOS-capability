#include "mainwindow.h"
#include <QApplication>
//#include <QtWidgets>



extern "C" int qtmn(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Dr. Jackson's Boat");

    w.show();

    return a.exec();


}
