#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

QTcpSocket* MainWindow::socket = nullptr;
QString MainWindow::lastReceivedMessage = "hello";

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

void MainWindow::socket_connected()
{
    qDebug()<<"connected";
    ui->connecting_status->setText("connected");
    sendData("client added");//this shows that senddata is working.
}

void MainWindow::socket_readyRead()
{
    QByteArray data = socket->readAll();
    lastReceivedMessage = QString::fromUtf8(data);
    ui->data->setText("data:"+lastReceivedMessage);
    qDebug()<<"Data: " + data;
}

void MainWindow::socket_bytesWritten()
{
    qDebug()<<"Data was written!";
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
        qDebug()<<"Data was sent";
    }
}

