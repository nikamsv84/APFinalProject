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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow*     instance;
    static QTcpSocket*     socket;
    static QString         lastReceivedMessage;

    // utility
    static QString hashPassword(const QString& password);
    static void    sendData(const QString& input);

signals:
    void messageReceived(const QString& message);

private slots:
    void socket_connected();
    void socket_readyRead();
    void socket_bytesWritten();
    void socket_disconnected();

    void on_connect_clicked();
    void on_disconnect_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
