#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools/qserialportdiscovery.h"
#include "obd/qelm327serial.h"

#include <QDebug>
#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SerialPortSettings settings;
    QSerialPortDiscovery serialPortDiscovery;   
    QList<QSerialPortInfo>* obdDevicesList = serialPortDiscovery.getOBDSerialPortList(settings);

    settings.setSerialPortInfo(obdDevicesList->first());

    QThread *workingThread = new QThread();
    QOBDDevice *elm327 = new QELM327Serial(settings);
    elm327->moveToThread(workingThread);    

    connect(workingThread, &QThread::started, elm327, &QELM327Serial::start);

    workingThread->start();    

    /* Adding some PID's to the polling loop */
    elm327->addPID("010C");  //Engine RPM
    elm327->addPID("010D");  //Vehicle speed

    /* Hashtable containing PID's names loaded from XML files */
    qDebug() << elm327->availablePIDs();

    delete obdDevicesList;
}

MainWindow::~MainWindow()
{
    delete ui;
}
