#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void tcpclientreaddata(void);

private slots:
    void on_psBt_connect_clicked();

    void on_psBt_send_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpclient;
};
#endif // MAINWINDOW_H

