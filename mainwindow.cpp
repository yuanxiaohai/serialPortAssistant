#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serialPort(new QSerialPort(this))
{
    ui->setupUi(this);
    initSerialPort();
    
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
}

MainWindow::~MainWindow()
{
    if(serialPort->isOpen())
        serialPort->close();
    delete ui;
}

void MainWindow::initSerialPort()
{
    // 获取可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portComboBox->addItem(info.portName());
    }
    
    // 波特率
    ui->baudComboBox->addItem("9600");
    ui->baudComboBox->addItem("19200");
    ui->baudComboBox->addItem("38400");
    ui->baudComboBox->addItem("115200");
}

void MainWindow::on_openSerialButton_clicked()
{
    if(!serialPort->isOpen()) {
        serialPort->setPortName(ui->portComboBox->currentText());
        serialPort->setBaudRate(ui->baudComboBox->currentText().toInt());
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        
        if(serialPort->open(QIODevice::ReadWrite)) {
            ui->openSerialButton->setText("关闭串口");
            ui->portComboBox->setEnabled(false);
            ui->baudComboBox->setEnabled(false);
        } else {
            QMessageBox::critical(this, "错误", "无法打开串口！");
        }
    } else {
        serialPort->close();
        ui->openSerialButton->setText("打开串口");
        ui->portComboBox->setEnabled(true);
        ui->baudComboBox->setEnabled(true);
    }
}

void MainWindow::on_sendButton_clicked()
{
    if(serialPort->isOpen()) {
        serialPort->write(ui->sendTextEdit->toPlainText().toUtf8());
    } else {
        QMessageBox::warning(this, "警告", "请先打开串口！");
    }
}

void MainWindow::readData()
{
    QByteArray data = serialPort->readAll();
    ui->receiveTextEdit->append(QString::fromUtf8(data));
}
