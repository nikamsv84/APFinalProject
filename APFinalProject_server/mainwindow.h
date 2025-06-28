#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QList>
#include <QFile>
#include <QString>
#include <QNetworkInterface>
#include <QList>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ManagingData(QTcpSocket *_socket, const char* data);
    static void sendDatatoAll(QString input);
    static QList<QTcpSocket*> clients;


public slots:
    void server_newConnection();
    void socket_connected();
    void socket_readyRead(QTcpSocket *_socket);
    void socket_bytesWritten(QTcpSocket *_socket);
    void socket_disconnected(QTcpSocket *_socket);

private slots:
    void on_connect_clicked();

    void on_disconnect_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *server = nullptr;
    int server_port;
};
#endif // MAINWINDOW_H
