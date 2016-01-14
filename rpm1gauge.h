#ifndef RPM1GAUGE_H
#define RPM1GAUGE_H

#include <QDialog>
#include <QTimer>
#include "qcustomplot.h"
#include "qcgaugewidget.h"
#include "mainwindow.h"

namespace Ui {
class rpm1gauge;
}

class rpm1gauge : public QDialog
{
    Q_OBJECT

public:
    explicit rpm1gauge(QString n, QcNeedleItem *need, QWidget *parent = 0);
    ~rpm1gauge();

    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    QcNeedleItem *mNeedle;
    QColor mColor;
    void setColor(QColor item);
    void setNeedle(QcNeedleItem * item);

private slots:
    void realtimeDataSlot();
    void on_pushButton_clicked();

private:
    Ui::rpm1gauge *ui;
    QString demoName;
    QTimer dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;
    int currentDemoIndex;
    QcGaugeWidget *rpm1sub;
    QcNeedleItem *rpm1subneedle;


};

#endif // RPM1GAUGE_H
