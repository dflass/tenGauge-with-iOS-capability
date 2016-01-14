#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QRect>

#include <QtWidgets>
#include <QtNetwork>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rpm1datavalue=0;                    //initializes data values
    fuel1datavalue=0;                   //otherwise initial position of needle is unknown
    oil1datavalue=0;                    //this will be removed upon network data aquisition
    temp1datavalue=120;
    bat1datavalue=10;

    rpm2datavalue=0;
    fuel2datavalue=0;
    oil2datavalue=0;
    temp2datavalue=120;
    bat2datavalue=10;

    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    /////               First Engine                            ///
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    /////               First Gauge: RPM                        ///
    ///////////////////////////////////////////////////////////////
    rpm1 = new QcGaugeWidget;                                 //initializes RPM1 gauge

    rpm1->addBackground(95);                                  //my pretty multicolor gradient background
    QcBackgroundItem *rpm1_bkg1 = rpm1->addBackground(92);
    rpm1_bkg1->clearrColors();
    rpm1_bkg1->addColor(float(0.1),Qt::black);                //smaller black background
    rpm1_bkg1->addColor(1.0,Qt::black);

    rpm1->addArc(55);                                         //arc, dashes and band
    rpm1->addDegrees(65)->setValueRange(0,60);
    rpm1->addColorBand(50);

    rpm1->addValues(80)->setValueRange(0,60);                 //numbers

    rpm1->addLabel(60)->setText("RPM");                       //labels on bottom
    rpm1->addLabel(75)->setText("x100");
    QcLabelItem *rpm1_lab = rpm1->addLabel(40);
    rpm1_lab->setText("0");
    rpm1_needle = rpm1->addNeedle(60);                        //needle
    rpm1_needle->setLabel(rpm1_lab);
    rpm1_needle->setColor(Qt::red);
    rpm1_needle->setValueRange(0,60);
    rpm1->addBackground(7);
    rpm1->addGlass(88);                                      //shading tint (set to white)

    QcLabelItem *rpm1_labwarn = rpm1->addLabel(40);
    rpm1_labwarn->setText("warning");
    rpm1_labwarn->setAngle(90);
    rpm1_labwarn->setColor(Qt::transparent);
    rpm1_needle->setWarnLabel(rpm1_labwarn);


    ///////////////////////////////////////////////////////////////
    /////               Second Gauge: Fuel                      ///
    ///////////////////////////////////////////////////////////////

    fuel1 = new QcGaugeWidget;
    fuel1->addBackground(95);
    QcBackgroundItem *fuel1_bkg1 = fuel1->addBackground(92);
    fuel1_bkg1->clearrColors();
    fuel1_bkg1->addColor(float(0.1),Qt::black);
    fuel1_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *fuel1_bkg2 = fuel1->addBackground(88);
    fuel1_bkg2->clearrColors();
    fuel1_bkg2->addColor(float(0.1),Qt::black);
    fuel1_bkg2->addColor(1.0,Qt::black);

    fuel1->addArc(55);
    fuel1->addDegrees(65)->setValueRange(0,100);
    fuel1->addColorBand(50);

    fuel1->addValues(80)->setValueRange(0,100);

    fuel1->addLabel(70)->setText("Fuel");
    QcLabelItem *fuel1_lab = fuel1->addLabel(40);
    fuel1_lab->setText("0");
    fuel1_needle = fuel1->addNeedle(60);
    fuel1_needle->setLabel(fuel1_lab);
    fuel1_needle->setColor(Qt::red);
    fuel1_needle->setValueRange(0,100);
    fuel1->addBackground(7);
    fuel1->addGlass(88);

    QcLabelItem *fuel1_labwarn = fuel1->addLabel(40);
    fuel1_labwarn->setText("low fuel");
    fuel1_labwarn->setAngle(90);
    fuel1_labwarn->setColor(Qt::transparent);
    fuel1_needle->setWarnLabel(fuel1_labwarn);

    ///////////////////////////////////////////////////////////////
    /////               Third Gauge: Battery                    ///
    ///////////////////////////////////////////////////////////////

    bat1 = new QcGaugeWidget;
    bat1->addBackground(5);
    QcBackgroundItem *bat1_bkg1 = bat1->addBackground(92);
    bat1_bkg1->clearrColors();
    bat1_bkg1->addColor(float(0.1),Qt::black);
    bat1_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *bat1_bkg2 = bat1->addBackground(88);
    bat1_bkg2->clearrColors();
    bat1_bkg2->addColor(float(0.1),Qt::black);
    bat1_bkg2->addColor(1.0,Qt::black);

    bat1->addArc(55);
    QcDegreesItem *bat1_deg = bat1->addDegrees(65);             //manual adjustment of degrees
    bat1_deg->setValueRange(10,18);
    bat1_deg->setStep(1);
    bat1->addColorBand(50);

    QcValuesItem *bat1_val = bat1->addValues(80);                //manual adjustment of numbers
    bat1_val->setValueRange(10,18);
    bat1_val->setStep(1);

    bat1->addLabel(60)->setText("Battery");
    bat1->addLabel(75)->setText("Volts");
    QcLabelItem *bat1_lab = bat1->addLabel(40);
    bat1_lab->setText("0");
    bat1_needle = bat1->addNeedle(60);
    bat1_needle->setLabel(bat1_lab);
    bat1_needle->setColor(Qt::red);
    bat1_needle->setValueRange(10,18);
    bat1->addBackground(7);
    bat1->addGlass(88);


    ///////////////////////////////////////////////////////////////
    /////               Forth Gauge: Temp                        ///
    ///////////////////////////////////////////////////////////////

    temp1 = new QcGaugeWidget;
    temp1->addBackground(5);
    QcBackgroundItem *temp1_bkg1 = temp1->addBackground(92);
    temp1_bkg1->clearrColors();
    temp1_bkg1->addColor(float(0.1),Qt::black);
    temp1_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *temp1_bkg2 = temp1->addBackground(88);
    temp1_bkg2->clearrColors();
    temp1_bkg2->addColor(float(0.1),Qt::black);
    temp1_bkg2->addColor(1.0,Qt::black);

    temp1->addArc(55);
    temp1->addDegrees(65)->setValueRange(120,220);
    temp1->addColorBand(50);

    temp1->addValues(80)->setValueRange(120,220);

    temp1->addLabel(60)->setText("Temp");
    temp1->addLabel(75)->setText("F");
    QcLabelItem *temp1_lab = temp1->addLabel(40);
    temp1_lab->setText("0");
    temp1_needle = temp1->addNeedle(60);
    temp1_needle->setLabel(temp1_lab);
    temp1_needle->setColor(Qt::red);
    temp1_needle->setValueRange(120,220);
    temp1->addBackground(7);
    temp1->addGlass(88);


    ///////////////////////////////////////////////////////////////
    /////               Fifth Gauge: Oil                        ///
    ///////////////////////////////////////////////////////////////

    oil1 = new QcGaugeWidget;
    oil1->addBackground(5);
    QcBackgroundItem *oil1_bkg1 = oil1->addBackground(92);
    oil1_bkg1->clearrColors();
    oil1_bkg1->addColor(float(0.1),Qt::black);
    oil1_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *oil1_bkg2 = oil1->addBackground(88);
    oil1_bkg2->clearrColors();
    oil1_bkg2->addColor(float(0.1),Qt::black);
    oil1_bkg2->addColor(1.0,Qt::black);

    oil1->addArc(55);
    oil1->addDegrees(65)->setValueRange(0,80);
    oil1->addColorBand(50);

    oil1->addValues(80)->setValueRange(0,80);

    oil1->addLabel(60)->setText("Oil");
    oil1->addLabel(75)->setText("PSI");
    QcLabelItem *oil1_lab = oil1->addLabel(40);
    oil1_lab->setText("0");
    oil1_needle = oil1->addNeedle(60);
    oil1_needle->setLabel(oil1_lab);
    oil1_needle->setColor(Qt::red);
    oil1_needle->setValueRange(0,80);
    oil1->addBackground(7);
    oil1->addGlass(88);

    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    /////               Second Engine                           ///
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    /////               First Gauge: RPM 2                       ///
    ///////////////////////////////////////////////////////////////

    rpm2 = new QcGaugeWidget;

    rpm2->addBackground(5);
    QcBackgroundItem *rpm2_bkg1 = rpm2->addBackground(92);
    rpm2_bkg1->clearrColors();
    rpm2_bkg1->addColor(float(0.1),Qt::black);
    rpm2_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *rpm2_bkg2 = rpm2->addBackground(88);
    rpm2_bkg2->clearrColors();
    rpm2_bkg2->addColor(float(0.1),Qt::black);
    rpm2_bkg2->addColor(1.0,Qt::black);

    rpm2->addArc(55);
    rpm2->addDegrees(65)->setValueRange(0,60);
    rpm2->addColorBand(50);

    rpm2->addValues(80)->setValueRange(0,60);

    rpm2->addLabel(70)->setText("RPM");
    QcLabelItem *rpm2_lab = rpm2->addLabel(40);
    rpm2_lab->setText("0");
    rpm2_needle = rpm2->addNeedle(60);
    rpm2_needle->setLabel(rpm2_lab);
    rpm2_needle->setColor(Qt::red);
    rpm2_needle->setValueRange(0,60);
    rpm2->addBackground(7);
    rpm2->addGlass(88);

    ///////////////////////////////////////////////////////////////
    /////               Second Gauge: Fuel2                      ///
    ///////////////////////////////////////////////////////////////

    fuel2 = new QcGaugeWidget;
    fuel2->addBackground(5);
    QcBackgroundItem *fuel2_bkg1 = fuel2->addBackground(92);
    fuel2_bkg1->clearrColors();
    fuel2_bkg1->addColor(float(0.1),Qt::black);
    fuel2_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *fuel2_bkg2 = fuel2->addBackground(88);
    fuel2_bkg2->clearrColors();
    fuel2_bkg2->addColor(float(0.1),Qt::black);
    fuel2_bkg2->addColor(1.0,Qt::black);

    fuel2->addArc(55);
    fuel2->addDegrees(65)->setValueRange(0,100);
    fuel2->addColorBand(50);

    fuel2->addValues(80)->setValueRange(0,100);

    fuel2->addLabel(70)->setText("Fuel");
    QcLabelItem *fuel2_lab = fuel2->addLabel(40);
    fuel2_lab->setText("0");
    fuel2_needle = fuel2->addNeedle(60);
    fuel2_needle->setLabel(fuel2_lab);
    fuel2_needle->setColor(Qt::red);
    fuel2_needle->setValueRange(0,100);
    fuel2->addBackground(7);
    fuel2->addGlass(88);

    ///////////////////////////////////////////////////////////////
    /////               Third Gauge: Battery 2                  ///
    ///////////////////////////////////////////////////////////////

    bat2 = new QcGaugeWidget;
    bat2->addBackground(5);
    QcBackgroundItem *bat2_bkg1 = bat2->addBackground(92);
    bat2_bkg1->clearrColors();
    bat2_bkg1->addColor(float(0.1),Qt::black);
    bat2_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *bat2_bkg2 = bat2->addBackground(88);
    bat2_bkg2->clearrColors();
    bat2_bkg2->addColor(float(0.1),Qt::black);
    bat2_bkg2->addColor(1.0,Qt::black);

    bat2->addArc(55);
    QcDegreesItem *bat2_deg = bat2->addDegrees(65);
    bat2_deg->setValueRange(10,18);
    bat2_deg->setStep(1);
    bat2->addColorBand(50);

    QcValuesItem *bat2_val = bat2->addValues(80);
    bat2_val->setValueRange(10,18);
    bat2_val->setStep(1);

    bat2->addLabel(70)->setText("Battery");
    QcLabelItem *bat2_lab = bat2->addLabel(40);
    bat2_lab->setText("0");
    bat2_needle = bat2->addNeedle(60);
    bat2_needle->setLabel(bat2_lab);
    bat2_needle->setColor(Qt::red);
    bat2_needle->setValueRange(10,18);
    bat2->addBackground(7);
    bat2->addGlass(88);


    ///////////////////////////////////////////////////////////////
    /////               Forth Gauge: Temp                        ///
    ///////////////////////////////////////////////////////////////

    temp2 = new QcGaugeWidget;
    temp2->addBackground(5);
    QcBackgroundItem *temp2_bkg1 = temp2->addBackground(92);
    temp2_bkg1->clearrColors();
    temp2_bkg1->addColor(float(0.1),Qt::black);
    temp2_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *temp2_bkg2 = temp2->addBackground(88);
    temp2_bkg2->clearrColors();
    temp2_bkg2->addColor(float(0.1),Qt::black);
    temp2_bkg2->addColor(1.0,Qt::black);

    temp2->addArc(55);
    temp2->addDegrees(65)->setValueRange(120,220);
    temp2->addColorBand(50);

    temp2->addValues(80)->setValueRange(120,220);

    temp2->addLabel(70)->setText("Temp");
    QcLabelItem *temp2_lab = temp2->addLabel(40);
    temp2_lab->setText("0");
    temp2_needle = temp2->addNeedle(60);
    temp2_needle->setLabel(temp2_lab);
    temp2_needle->setColor(Qt::red);
    temp2_needle->setValueRange(120,220);
    temp2->addBackground(7);
    temp2->addGlass(88);


    ///////////////////////////////////////////////////////////////
    /////               Fifth Gauge: Oil                        ///
    ///////////////////////////////////////////////////////////////

    oil2 = new QcGaugeWidget;
    oil2->addBackground(5);
    QcBackgroundItem *oil2_bkg1 = oil2->addBackground(92);
    oil2_bkg1->clearrColors();
    oil2_bkg1->addColor(float(0.1),Qt::black);
    oil2_bkg1->addColor(1.0,Qt::black);

    QcBackgroundItem *oil2_bkg2 = oil2->addBackground(88);
    oil2_bkg2->clearrColors();
    oil2_bkg2->addColor(float(0.1),Qt::black);
    oil2_bkg2->addColor(1.0,Qt::black);

    oil2->addArc(55);
    oil2->addDegrees(65)->setValueRange(0,80);
    oil2->addColorBand(50);

    oil2->addValues(80)->setValueRange(0,80);

    oil2->addLabel(70)->setText("Oil");
    QcLabelItem *oil2_lab = oil2->addLabel(40);
    oil2_lab->setText("0");
    oil2_needle = oil2->addNeedle(60);
    oil2_needle->setLabel(oil2_lab);
    oil2_needle->setColor(Qt::red);
    oil2_needle->setValueRange(0,80);
    oil2->addBackground(7);
    oil2->addGlass(88);


    ///////////////////////////////////////////////////////////////
    /////                   Layout                              ///
    ///////////////////////////////////////////////////////////////

    ui->verticalLayout->addWidget(rpm1);                    //adds gauges to the user interface
    ui->verticalLayout->addWidget(fuel1);
    ui->verticalLayout_2->addWidget(oil1);
    ui->verticalLayout_2->addWidget(temp1);
    ui->verticalLayout_3->addWidget(bat1);
    ui->verticalLayout_4->addWidget(rpm2);
    ui->verticalLayout_4->addWidget(fuel2);
    ui->verticalLayout_5->addWidget(oil2);
    ui->verticalLayout_5->addWidget(temp2);
    ui->verticalLayout_6->addWidget(bat2);

    tcpSocket = new QTcpSocket(this);
    count=0;                                                //used for data storage

    connect(&testtimer, SIGNAL(timeout()), this, SLOT(requestData()));        //on timeout, run testingtime

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    testtimer.start(50);                                    //after 50 ms, timeout and reset to 50
}

void MainWindow::requestData()
{
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost("10.121.220.244"/*"localhost"*/, 1455);                               //specific network
}

void MainWindow::readData()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }
    if (tcpSocket->bytesAvailable() < blockSize)
        return;
    QString data;
    in >> data;

    qDebug() << data;

    QRegExp rx("[| ]");// match a comma or a space
    QStringList list = data.split(rx, QString::SkipEmptyParts);
    QList <int> nums;
    for(int i = 0; i < list.size(); i++)
    {
        nums.append(list.at(i).toInt());
    }

    rpm1datavalue=nums.at(0);
    fuel1datavalue=nums.at(1);
    oil1datavalue=nums.at(2);
    temp1datavalue=nums.at(3);
    bat1datavalue=nums.at(4);
    rpm2datavalue=nums.at(5);
    fuel2datavalue=nums.at(6);
    oil2datavalue=nums.at(7);
    temp2datavalue=nums.at(8);
    bat2datavalue=nums.at(9);

    testingtiming();
}

MainWindow::~MainWindow()
{    delete ui;}


///////////////////////////////
/// When a gauge is clicked ///
///////////////////////////////
void MainWindow::mousePressEvent(QMouseEvent* e)
{    QRect rectrpm1 = rpm1->frameGeometry();                //sets rectangle for each gauge
     QRect rectfuel1 = fuel1->frameGeometry();
     QRect rectoil1 = oil1->frameGeometry();
     QRect recttemp1 = temp1->frameGeometry();
     QRect rectbat1 = bat1->frameGeometry();

     QRect rectrpm2 = rpm2->frameGeometry();
     QRect rectfuel2 = fuel2->frameGeometry();
     QRect rectoil2 = oil2->frameGeometry();
     QRect recttemp2 = temp2->frameGeometry();
     QRect rectbat2 = bat2->frameGeometry();

     QString rpmstring = "Rpm";                                      //sets label
     QString fuelstring = "Fuel";
     QString oilstring = "Oil";
     QString tempstring = "Temperature";
     QString batstring = "Battery";



     if(rectrpm1.contains(e->pos())){
     //     Modal                                       //I might need this code later when building to iPad. Don't erase please
     //rpm1gauge inst;                                  //ios might not like modalless
     //inst.setModal(true);                             //I will also probably impliment this when networking is added
     //inst.exec();
     //inst.setNeedle(rpm1_needle);

     //     Modalless
     rpm1graph = new rpm1gauge(rpmstring, rpm1_needle, this);       //creates graph window from needle and string
     rpm1graph->setWindowTitle("RPM Engine 1");
     rpm1graph->show();}

     if(rectfuel1.contains(e->pos())){
     fuel1graph = new rpm1gauge(fuelstring, fuel1_needle, this);
     fuel1graph->setWindowTitle("Fuel Engine 1");
     fuel1graph->setColor(Qt::red);
     fuel1graph->show();}

     if(rectoil1.contains(e->pos())){
     oil1graph = new rpm1gauge(oilstring, oil1_needle, this);
     oil1graph->setWindowTitle("Oil Engine 1");
     oil1graph->setColor(Qt::yellow);
     oil1graph->show();}

     if(recttemp1.contains(e->pos())){
     temp1graph = new rpm1gauge(tempstring, temp1_needle, this);
     temp1graph->setWindowTitle("Temperature Engine 1");
     temp1graph->setColor(Qt::green);
     temp1graph->show();}

     if(rectbat1.contains(e->pos())){
     bat1graph = new rpm1gauge(batstring, bat1_needle, this);
     bat1graph->setWindowTitle("Battery Engine 1");
     bat1graph->setColor(Qt::green);
     bat1graph->show();}

     if(rectrpm2.contains(e->pos())){
     rpm2graph = new rpm1gauge(rpmstring, rpm2_needle, this);
     rpm2graph->setWindowTitle("RPM Engine 2");
     rpm2graph->show();}

     if(rectfuel2.contains(e->pos())){
     fuel2graph = new rpm1gauge(fuelstring, fuel2_needle, this);
     fuel2graph->setWindowTitle("Fuel Engine 2");
     fuel2graph->setColor(Qt::red);
     fuel2graph->show();}

     if(rectoil2.contains(e->pos())){
     oil2graph = new rpm1gauge(oilstring, oil2_needle, this);
     oil2graph->setWindowTitle("Oil Engine 2");
     oil2graph->setColor(Qt::yellow);
     oil2graph->show();}

     if(recttemp2.contains(e->pos())){
     temp2graph = new rpm1gauge(tempstring, temp2_needle, this);
     temp2graph->setWindowTitle("Temperature Engine 2");
     temp2graph->setColor(Qt::green);
     temp2graph->show();}

     if(rectbat2.contains(e->pos())){
     bat2graph = new rpm1gauge(batstring, bat2_needle, this);
     bat2graph->setWindowTitle("Battery Engine 2");
     bat2graph->setColor(Qt::green);
     bat2graph->show();}


}
void MainWindow::testingtiming()                                //runs every 50 ms
{
    count += 1;
    rpm1_needle->setCurrentValue(rpm1datavalue);                //sets homepage gauge values
    fuel1_needle->setCurrentValue(fuel1datavalue);
    oil1_needle->setCurrentValue(oil1datavalue);
    temp1_needle->setCurrentValue(temp1datavalue);
    bat1_needle->setCurrentValue(bat1datavalue);

    rpm2_needle->setCurrentValue(rpm2datavalue);
    fuel2_needle->setCurrentValue(fuel2datavalue);
    oil2_needle->setCurrentValue(oil2datavalue);
    temp2_needle->setCurrentValue(temp2datavalue);
    bat2_needle->setCurrentValue(bat2datavalue);

    if(rpm1datavalue<50){                                       //warnings
            rpm1_needle->setColor(Qt::red);
            rpm1_needle->dropWarnFlag();}
    else{   rpm1_needle->setColor(Qt::cyan);
            rpm1_needle->setWarnFlag();}

    if(fuel1datavalue<10){
            fuel1_needle->setColor(Qt::cyan);
            fuel1_needle->setWarnFlag();}
    else{   fuel1_needle->setColor(Qt::red);
            fuel1_needle->dropWarnFlag();}



    if (count==2){                                             //graphing data
    rpm1_needle->addData();                                    //updated every 100 ms
    fuel1_needle->addData();
    oil1_needle->addData();
    temp1_needle->addData();
    bat1_needle->addData();
    rpm2_needle->addData();
    fuel2_needle->addData();
    oil2_needle->addData();
    temp2_needle->addData();
    bat2_needle->addData();
    count=0;
    }
}
