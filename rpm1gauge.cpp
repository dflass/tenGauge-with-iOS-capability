#include "rpm1gauge.h"
#include "ui_rpm1gauge.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "qcgaugewidget.h"
#include "mainwindow.h"


rpm1gauge::rpm1gauge(QString n, QcNeedleItem *need,QWidget *parent) : QDialog(parent), ui(new Ui::rpm1gauge)
{                                                                   //constructor with needle and string input
    mNeedle = need;
    mColor = Qt::blue;
    ui->setupUi(this);

    rpm1sub = new QcGaugeWidget;                                    //new gauge initialized
    rpm1sub->addBackground(95);
    QcBackgroundItem *rpm1subback = rpm1sub->addBackground(92);
    rpm1subback->clearrColors();
    rpm1subback->addColor(float(0.1),Qt::black);
    rpm1sub->addArc(55);

    rpm1sub->addDegrees(65)->setValueRange(mNeedle->mMinValue,mNeedle->mMaxValue);      //values set based on needle input max/min
    rpm1sub->addColorBand(50);
    rpm1sub->addValues(80)->setValueRange(mNeedle->mMinValue,mNeedle->mMaxValue);
    rpm1sub->addLabel(70)->setText(n);


    rpm1subneedle = rpm1sub->addNeedle(60);                                             //sets needle values
    rpm1subneedle->setValueRange(mNeedle->mMinValue,mNeedle->mMaxValue);
    rpm1subneedle->setColor(Qt::red);

    QcLabelItem *rpm1_lab = rpm1sub->addLabel(40);
    rpm1_lab->setText("0");
    rpm1subneedle->setLabel(rpm1_lab);

    setupRealtimeDataDemo(ui->customPlot);                                              //run data display

}



void rpm1gauge::setNeedle(QcNeedleItem *n){                     //might not end up working, should probably just pass in variables
    mNeedle = n;
}

void rpm1gauge::setColor(QColor n){
    mColor  = n;
}

rpm1gauge::~rpm1gauge()
{
    delete ui;
}

void rpm1gauge::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  customPlot->addGraph();                                               //line graph
  customPlot->graph(0)->setPen(QPen(mColor));
  customPlot->addGraph();                                               //dot for new data
  customPlot->graph(1)->setPen(QPen(mColor));
  customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssStar);


  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);             //time on x-axis

  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(30);                                   //30 second interval ticks
  customPlot->axisRect()->setupFullAxesBox();

  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));


  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));           //runs rtds function every 300 ms

  dataTimer.start(1000);
}

void rpm1gauge::realtimeDataSlot()
{
  int sum=0;
  QList<QPair<double, double> > templist = mNeedle->returnData();                //data has value and timee
  for(int i=0; i<templist.length();i++){
      QPair<double,double> ifthisworks = templist.at(i);
      ui->customPlot->graph(0)->addData(ifthisworks.first, ifthisworks.second);     //adds data to graph
      sum+=ifthisworks.second;
  }

  //
  QPair<double, double> temppair= templist.at(templist.length()-1);             //most recent data
  double x = temppair.second;
  double timedata = temppair.first;
  ui->lcdNumber->display(x);                                                    //display current value
  ui->lcdNumber->setPalette(Qt::black);
  //
  rpm1subneedle->setCurrentValue(x);                                            //sets needle
  ui->customPlot->graph(1)->clearData();
  ui->customPlot->graph(1)->addData(timedata, x);
  ui->customPlot->graph(0)->removeDataBefore(timedata-300);                     //removes old data out of range
  ui->customPlot->graph(1)->removeDataBefore(timedata-300);
  ui->customPlot->graph(0)->rescaleValueAxis();                                 //scales axis
  ui->customPlot->graph(1)->rescaleValueAxis(true);


  ui->customPlot->xAxis->setRange(timedata+0.25, 300, Qt::AlignRight);          //scales x axis
  ui->customPlot->yAxis->setRange(mNeedle->mMinValue,mNeedle->mMaxValue * 1.05);
  ui->customPlot->replot();

  ui->verticalLayout_2->addWidget(rpm1sub);
}



void rpm1gauge::on_pushButton_clicked()
{
    delete ui;
}
