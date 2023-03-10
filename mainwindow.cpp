#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkProxy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpclient = new QTcpSocket(this);
    connect(tcpclient,&QTcpSocket::readyRead,this,&MainWindow::tcpclientreaddata);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcpclientreaddata()
{
    QByteArray buffer = tcpclient->readAll();
    //此处为解决单片机发来数据中中文乱码问题
    QString recstr = QString::fromLocal8Bit(buffer);
    ui->txEt_data->append("接收数据:"+recstr);
    qDebug()<<recstr;
}


void MainWindow::on_psBt_connect_clicked()
{
    //未连接状态  去链接
    if(tcpclient->state() != QAbstractSocket::ConnectedState) {
        //服务器IP
        QString serverip = ui->lnEt_ip->text();
        //服务器PORT
        ushort serverport = ui->lnEt_port->text().toUShort();
        qDebug() << "连接服务器ip:" << serverip << ",端口：" << serverport;
        //连接服务器
        tcpclient->setProxy(QNetworkProxy::NoProxy);
        tcpclient->connectToHost(serverip,serverport);
        //等待连接成功
        if(!tcpclient->waitForConnected(30000)) {
           qDebug() << "Connection failed!";
           return;
        }
        ui->psBt_connect->setText("已连接");
    }
    //已连接状态 去断开
    else {
        //断开服务器
        tcpclient->disconnectFromHost();
        ui->psBt_connect->setText("连接");
    }
}

void MainWindow::on_psBt_send_clicked()
{
    if(tcpclient->state() == QAbstractSocket::ConnectedState) {
        //获取发送的数据
        QString sendstr = ui->txEt_sendata->toPlainText();
        //发送数据
        tcpclient->write(sendstr.toLocal8Bit());
        //打印发送的数据
        ui->txEt_data->append("发送数据:"+sendstr);
    }
}

