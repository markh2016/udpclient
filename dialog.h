#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QUdpSocket>
#include <QDataStream>
#include <QtSerialPort>
#include <QMessageBox>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void getPortName() ;


private slots :

    void processPendingDatagrams() ;

private:
    Ui::Dialog *ui ;

    QUdpSocket *udpSocket;

    QSerialPort *arduino;

    static const quint16 arduino_uno_vendor_id =  9025 ; //6790 ; //9025;
    static const quint16 arduino_uno_product_id = 67 ; //29987 ; //1;
    QString arduino_port_name;
    bool arduino_is_available;

};

#endif // DIALOG_H
