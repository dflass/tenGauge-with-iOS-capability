
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rpm1gauge.h"
#include <QMainWindow>
#include <QTimer>
#include <QTcpSocket>

class rpm1gauge;
class QTcpSocket;
class QNetworkSession;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();


    QcGaugeWidget * rpm1;
    QcNeedleItem *rpm1_needle;

    QList<double> * time;
    QList<double> * rpm1data;




private slots:

    void mousePressEvent(QMouseEvent *event);

    void testingtiming();

    void requestData();
    void readData();


private:
    Ui::MainWindow *ui;



    QcGaugeWidget * fuel1;
    QcNeedleItem *fuel1_needle;

    QcGaugeWidget * bat1;
    QcNeedleItem *bat1_needle;

    QcGaugeWidget * temp1;
    QcNeedleItem *temp1_needle;

    QcGaugeWidget * oil1;
    QcNeedleItem *oil1_needle;

    QcGaugeWidget * rpm2;
    QcNeedleItem *rpm2_needle;

    QcGaugeWidget * fuel2;
    QcNeedleItem *fuel2_needle;

    QcGaugeWidget * bat2;
    QcNeedleItem *bat2_needle;

    QcGaugeWidget * temp2;
    QcNeedleItem *temp2_needle;

    QcGaugeWidget * oil2;
    QcNeedleItem *oil2_needle;

    QTimer testtimer;
    QTimer testtimer2;

    int rpm1datavalue;
    int fuel1datavalue;
    int oil1datavalue;
    int temp1datavalue;
    int bat1datavalue;

    int rpm2datavalue;
    int fuel2datavalue;
    int oil2datavalue;
    int temp2datavalue;
    int bat2datavalue;

    rpm1gauge *rpm1graph;
    rpm1gauge *fuel1graph;
    rpm1gauge *oil1graph;
    rpm1gauge *temp1graph;
    rpm1gauge *bat1graph;

    rpm1gauge *rpm2graph;
    rpm1gauge *fuel2graph;
    rpm1gauge *oil2graph;
    rpm1gauge *temp2graph;
    rpm1gauge *bat2graph;

    int count;

    QTcpSocket *tcpSocket;
    QString currentData;
    quint16 blockSize;
};

#endif // MAINWINDOW_H
