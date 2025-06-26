#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcome.h"
#include <QDebug>


#include <QCryptographicHash>
#include <QString>


QTcpSocket* MainWindow::socket = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    if(socket)
    {
        delete socket;
    }

    delete ui;
}



QString MainWindow::hashPassword(const QString& password) {
    // Using SHA-256 for hashing
    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    // Convert hashed data to hexadecimal string
    return hashed.toHex();
}


void MainWindow::socket_connected()
{
    qDebug()<<"connected";
    ui->connecting_status->setText("connected");
    welcome* welcomePage = new welcome();
    sendData("client added");//this shows that senddata is working.
    welcomePage->show();
}

QByteArray MainWindow::socket_readyRead()
{
    QByteArray data = socket->readAll();
    // qDebug()<<"Data: " + data;
    return data;
}

void MainWindow::socket_bytesWritten()
{
    // qDebug()<<"Data was written!";
}

void MainWindow::socket_disconnected()
{
    qDebug()<<"Disconnected!";
    ui->connecting_status->setText("Disconnected");
}

void MainWindow::on_connect_clicked()
{
    if (socket)
    {
        qDebug()<<"Already connected!";
        ui->connecting_status->setText("Already connected");
    } else
    {
        qDebug()<<"Connecting...";
        ui->connecting_status->setText("connecting...");

        socket = new QTcpSocket();
        QString ip = ui->ip->text();
        socket->connectToHost(ip, ui->port->text().toInt());


        connect(socket, &QTcpSocket::connected, this, &MainWindow::socket_connected);
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_readyRead);
        connect(socket, &QTcpSocket::bytesWritten, this, &MainWindow::socket_bytesWritten);
        connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);
    }

}


void MainWindow::on_disconnect_clicked()
{
    if (!socket)
    {
        qDebug()<<"Not connect yet!";
    } else
    {
        delete socket;
        socket = nullptr;
        qDebug()<<"Disconnected";
    }

}


void MainWindow::sendData(QString input){
    if (socket)
    {
        socket->write(input.toUtf8());
        // qDebug()<<"Data was sent";
    }
}
