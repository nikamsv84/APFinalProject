#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcome.h"
#include <QCryptographicHash>
#include <QDebug>

// مقداردهی استاتیک‌ها
MainWindow*     MainWindow::instance              = nullptr;
QTcpSocket*     MainWindow::socket                = nullptr;
QString         MainWindow::lastReceivedMessage   = QString();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // رجیستر کردن این نمونه به عنوان instance
    MainWindow::instance = this;
}

MainWindow::~MainWindow()
{
    if(socket) {
        delete socket;
        socket = nullptr;
    }
    delete ui;
}

QString MainWindow::hashPassword(const QString& password)
{
    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(),
                                                 QCryptographicHash::Sha256);
    return hashed.toHex();
}

void MainWindow::sendData(const QString& input)
{
    if(socket && socket->isOpen()) {
        socket->write(input.toUtf8());
    }
}

void MainWindow::socket_connected()
{
    qDebug() << "connected";
    ui->connecting_status->setText("connected");

    // صفحه خوش‌آمدگویی
    welcome* welcomePage = new welcome();
    sendData("client added");
    welcomePage->show();
}

void MainWindow::socket_readyRead()
{
    QByteArray data = socket->readAll();
    lastReceivedMessage = QString::fromUtf8(data);
    qDebug() << "MainWindow received:" << lastReceivedMessage;

    // انتشار سیگنال برای همه اسلات‌ها
    emit messageReceived(lastReceivedMessage);
}

void MainWindow::socket_bytesWritten()
{
    Q_UNUSED(socket)
    // qDebug() << "Bytes were written";
}

void MainWindow::socket_disconnected()
{
    qDebug() << "Disconnected!";
    ui->connecting_status->setText("Disconnected");
}

void MainWindow::on_connect_clicked()
{
    if (socket) {
        ui->connecting_status->setText("Already connected");
        return;
    }

    ui->connecting_status->setText("connecting...");
    socket = new QTcpSocket(this);

    QString ip   = ui->ip->text();
    int     port = ui->port->text().toInt();
    socket->connectToHost(ip, port);

    connect(socket, &QTcpSocket::connected,    this, &MainWindow::socket_connected);
    connect(socket, &QTcpSocket::readyRead,    this, &MainWindow::socket_readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &MainWindow::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);
}

void MainWindow::on_disconnect_clicked()
{
    if (!socket) {
        qDebug() << "Not connected yet!";
        return;
    }
    socket->disconnectFromHost();
    delete socket;
    socket = nullptr;
    qDebug() << "Disconnected";
}
