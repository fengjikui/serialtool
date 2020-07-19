#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    bool opened;
    QSerialPort serialport;
    QDateTime readTime;

public:
    void init();
private slots:
    void on_openButton_clicked();
    void on_cmb_PortRate_currentIndexChanged(int index);
    void on_cmb_PortName_currentTextChanged(const QString &arg1);
    void on_cmb_stopbits_currentIndexChanged(int index);
    void on_cmb_databytes_currentIndexChanged(int index);
    void on_cmb_parity_currentIndexChanged(int index);

    void recv_data();

};
#endif // WIDGET_H
