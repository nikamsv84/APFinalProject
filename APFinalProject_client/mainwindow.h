#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static QTcpSocket *socket;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void sendData(QString input);
    static QString lastReceivedMessage;

public slots:
    void socket_connected();
    void socket_bytesWritten();
    void socket_disconnected();
    void socket_readyRead();


private slots:
    void on_connect_clicked();

    void on_disconnect_clicked();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
