#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QNetworkProxy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //创建tcp连接对象
    tcpclient = new QTcpSocket(this);
    connect(tcpclient,&QTcpSocket::readyRead,this,&MainWindow::tcpclientreaddata);
    //数据库连接对象
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    //数据库ip
    db.setHostName("127.0.0.1");
    //数据库端口
    db.setPort(5432);
    //数据库名称
    db.setDatabaseName("test");
    //数据库用户名
    db.setUserName("postgres");
    //数据库密码
    db.setPassword("postgres");
    if(!db.open())
    {
        qDebug()<<"connect db faild";
    }
    else
    {
        qDebug()<<"connect db ok";
    }
    //插入数据
    //    QSqlQuery query(db);
    //    QString cmd = "insert into test values(1,'haoyuxin','haoyuxin123')";
    ////    QString cmd = "delete from test where id=1";
    //    qDebug()<<"sql:"<<cmd;
    //    if(!query.exec(cmd)){
    //        qDebug()<<"insert error!";
    //    }else{
    //        qDebug()<<"insert to db!";
    //    }
    //查询数据
    QSqlQuery query(db);
    QString cmd ="select * from test;";
    if(!query.exec(cmd)){
        qDebug()<<"select error!";
    }else{
        while(query.next()){
            qDebug()<<"id:"<<query.value(0).toString();
            qDebug()<<"username:"<<query.value(1).toString();
            qDebug()<<"password:"<<query.value(2).toString();
        }
    }

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
        qDebug() << "Connection success!";
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

