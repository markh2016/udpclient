#include "dialog.h"
#include "ui_dialog.h"



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this) ;
    udpSocket->bind(QHostAddress::LocalHost,4000);

    arduino_is_available = false;
    setFixedSize(this->width(),this->height());
    arduino = new QSerialPort;

    getPortName() ;


   /* qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    } */




    if(arduino_is_available){
        // open and configure the serialport
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::WriteOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }else{
        // give error message if not available
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
    }


    connect(udpSocket,SIGNAL(readyRead()),SLOT(processPendingDatagrams())) ;
}


void Dialog::getPortName()

{

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    qDebug()<< arduino_port_name ;


}

void Dialog::processPendingDatagrams()
{
    QByteArray datagram;
    QHostAddress sender ;
    quint16 port ;
    QString snumber ;



        do {
            datagram.resize(udpSocket->pendingDatagramSize());
            udpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &port );

        } while (udpSocket->hasPendingDatagrams());


        quint8 number ;

        QDataStream in(&datagram, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_1);
        in >> number ;

        snumber =   QString("%1").arg(number);





        ui->lcdNumber->display(number) ;

        if(arduino->isWritable()){
            arduino->write(snumber.toStdString().c_str());
            qDebug()<<"number sent is "<< (snumber.toStdString().c_str()) ;
        }else{
            qDebug() << "Couldn't write to serial!";
        }

 }






Dialog::~Dialog()
{
    if (arduino->isOpen()){
        arduino->close() ;
    }

    delete ui;
}
