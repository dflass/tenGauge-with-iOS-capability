#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T15:30:14
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tenGauge
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rpm1gauge.cpp \
    qcustomplot.cpp

    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcgaugewidget.h \
    rpm1gauge.h \
    qcustomplot.h

    qcustomplot.h

FORMS    += mainwindow.ui \
    rpm1gauge.ui
