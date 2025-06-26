#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupmanager.h"

QList<QTcpSocket*> MainWindow::clients;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setText("1500");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::server_newConnection()
{
    QTcpSocket * new_client = server->nextPendingConnection();
    new_client->setObjectName("Client " + QString::number(clients.size() + 1));
    clients.append(new_client);
    connect(new_client, &QTcpSocket::connected, this, &MainWindow::socket_connected);
    connect(new_client, &QIODevice::readyRead, this, [this, new_client](){ socket_readyRead(new_client); });
    connect(new_client, &QIODevice::bytesWritten, this, [this, new_client](){ socket_bytesWritten(new_client); });
    connect(new_client, &QAbstractSocket::disconnected, this, [this, new_client](){ socket_disconnected(new_client); });
}

void MainWindow::socket_connected()
{
    ui->output->append("Connected!");
}

void MainWindow::socket_readyRead(QTcpSocket *_socket)
{
    QByteArray data = _socket->readAll();
    ui->output->append("" + _socket->objectName() + ": " + data);
    ManagingData(_socket, (const char *)data);
    // QString message_back_to_client = "Got your message!" + _socket->readAll() + "";
    // _socket->write(message_back_to_client.toUtf8());
    // _socket->flush();
    // _socket->waitForBytesWritten(3000);
}

void MainWindow::socket_bytesWritten(QTcpSocket *_socket)
{
    ui->output->append("Data was written!");
}

void MainWindow::socket_disconnected(QTcpSocket *_socket)
{
    ui->output->append(_socket->objectName() + " disconnected!");
    clients.removeOne(_socket);

    for (int var = 0; var < clients.size(); ++var)
    {
        clients[var]->setObjectName("Client " + QString::number(var + 1));
    }

    _socket->deleteLater();
}

void MainWindow::on_connect_clicked()
{
    if (server && server_port == ui->port->text().toInt())
    {
        ui->output->append("Already connected!");
    } else
    {
        ui->output->append("Connecting...");

        server = new QTcpServer();
        server_port = ui->port->text().toInt();
        server->listen(QHostAddress::Any, server_port);

        if (server->isListening())
        {
            connect(server, &QTcpServer::newConnection, this, &MainWindow::server_newConnection);
            ui->output->append("Connected!");
        }

    }

}


void MainWindow::on_disconnect_clicked()
{
    if (!server)
    {
        ui->output->append("Not connected yet!");
    } else
    {
        for (auto& _socket : clients) {
            if (!_socket)
            {
                delete _socket;
            }

            _socket = nullptr;
        }

        delete server;
        server = nullptr;
        ui->output->append("Disconnected!");
    }

}


void MainWindow::sendDatatoAll(QString input)
{
    for (auto& _socket : clients) {
        if (_socket)
        {
            _socket->write(input.toUtf8());
            _socket->flush();
            _socket->waitForBytesWritten();
            qDebug()<<"Data was sent";
        }

    }

}

void MainWindow::ManagingData(QTcpSocket *_socket, const char* data)
{
    qDebug()<<data;
    char* specifier_SIGNUP = strstr(data, "\\SIGNUP\\");
    char* specifier_SIGNIN = strstr(data, "\\SIGNIN\\");
    char* specifier_STARTGAME = strstr(data, "\\STARTGAME\\");

    if (specifier_SIGNUP)
    {
        sendDatatoAll("the datas are related to signin info.");
        QString stringData = QString::fromUtf8(data);
        SignupManager signupproccess(stringData);
        signupproccess.Messagehandeler();
        signupproccess.WriteDatasToFile();
        ui->output->append("the datas are related to signupinfo.");
    }
    else if (specifier_STARTGAME)
    {
        qDebug()<<"the datas are related to startgame";
        if (clients.size() == 2)
        {
            qDebug() <<"starting the game";
            _socket->write("\\OK\\");
            _socket->flush();
            _socket->waitForBytesWritten();
            qDebug()<<"Data was sent";

        }else{
            qDebug()<<"the number of clients are not valid";
        }
    }

}


