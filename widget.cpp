#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
/*--------------------*/

/*--------------------*/

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , opened(false)
{
    ui->setupUi(this);
    init();

    connect(&serialport,&QSerialPort::readyRead,this,&Widget::recv_data);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    if(!availablePorts.empty())
    {
        QSerialPortInfo portinfo;
        foreach(portinfo, availablePorts){
            ui->cmb_PortName->addItem(portinfo.systemLocation());
            qDebug()<<portinfo.manufacturer();
            qDebug()<<portinfo.description()<<portinfo.portName()<<portinfo.isBusy();
            qDebug();
        }
        ui->cmb_PortName->setCurrentIndex(availablePorts.size()-1);     //设置为识别到的最后一个端口
        serialport.setPortName(ui->cmb_PortName->currentText());
    }

    ui->cmb_parity->setCurrentIndex(0);         // QSerialPort::NoParity
    ui->cmb_PortRate->setCurrentIndex(7);       // QSerialPort::Baud115200
    ui->cmb_stopbits->setCurrentIndex(0);       // QSerialPort::OneStop
    ui->cmb_databytes->setCurrentIndex(3);      // QSerialPort::Data8

    serialport.setParity(QSerialPort::NoParity);
    serialport.setBaudRate(QSerialPort::Baud115200);
    serialport.setStopBits(QSerialPort::OneStop);
    serialport.setDataBits(QSerialPort::Data8);
    serialport.setReadBufferSize(8*1024*64);
}


void Widget::on_openButton_clicked()
{
    // 若是尚未打开
    if(opened==false)
    {

        if(serialport.open(QIODevice::ReadWrite)==false)
        {
            QMessageBox::critical(this, tr("Error"), serialport.errorString());
        }
        else{
            opened=!opened;
            ui->openButton->setText("关闭");
            qDebug()<<serialport.readBufferSize();

        }
    }
    else
    {
        serialport.close();
        ui->openButton->setText("打开");
        opened=!opened;
    }
    ui->cmb_parity->setDisabled(opened);
    ui->cmb_PortName->setDisabled(opened);
    ui->cmb_PortRate->setDisabled(opened);
    ui->cmb_stopbits->setDisabled(opened);
    ui->cmb_databytes->setDisabled(opened);


    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    if(!availablePorts.empty())
    {
        QSerialPortInfo portinfo;
        foreach(portinfo, availablePorts){
//            ui->cmb_PortName->addItem(portinfo.systemLocation());
            qDebug()<<portinfo.manufacturer();
            qDebug()<<portinfo.description()<<portinfo.portName()<<portinfo.isBusy();
            qDebug();
        }
    }

}


void Widget::on_cmb_PortRate_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        serialport.setBaudRate(QSerialPort::Baud1200);
        break;
    case 1:
        serialport.setBaudRate(QSerialPort::Baud2400);
        break;
    case 2:
        serialport.setBaudRate(QSerialPort::Baud4800);
        break;
    case 3:
        serialport.setBaudRate(QSerialPort::Baud9600);
        break;
    case 4:
        serialport.setBaudRate(QSerialPort::Baud19200);
        break;
    case 5:
        serialport.setBaudRate(QSerialPort::Baud38400);
        break;
    case 6:
        serialport.setBaudRate(QSerialPort::Baud57600);
        break;
    case 7:
        serialport.setBaudRate(QSerialPort::Baud115200);
        break;


    }
}

void Widget::on_cmb_PortName_currentTextChanged(const QString &arg1)
{
    serialport.setPortName(arg1);
    qDebug()<<arg1;
}

void Widget::on_cmb_stopbits_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        serialport.setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        serialport.setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        serialport.setStopBits(QSerialPort::TwoStop);
        break;
    }
}

void Widget::on_cmb_databytes_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        serialport.setDataBits(QSerialPort::Data5);
        break;
    case 1:
        serialport.setDataBits(QSerialPort::Data6);
        break;
    case 2:
        serialport.setDataBits(QSerialPort::Data7);
        break;
    case 3:
        serialport.setDataBits(QSerialPort::Data8);
        break;
    }
}

void Widget::on_cmb_parity_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        serialport.setParity(QSerialPort::NoParity);
        break;
    case 1:
        serialport.setParity(QSerialPort::OddParity);
        break;
    case 2:
        serialport.setParity(QSerialPort::EvenParity);
        break;
    case 3:
        serialport.setParity(QSerialPort::SpaceParity);
        break;
    case 4:
        serialport.setParity(QSerialPort::MarkParity);
        break;
    }
}

void Widget::recv_data()
{
    QByteArray buf;
    while (serialport.waitForReadyRead(5)) {
        buf += serialport.readAll();
    }
    buf+='\n';
    ui->recv_Data->insertPlainText(QDateTime::currentDateTime().toString("yyyy-mm-dd hh:mm:ss.zzz\n"));
    ui->recv_Data->insertPlainText(buf);


    qDebug()<<buf;
}
