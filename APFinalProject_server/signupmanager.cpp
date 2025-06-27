#include "signupmanager.h"
#include <mainwindow.h>
#include <QList>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QTextStream>

SignupManager::SignupManager(QString ReceivedData):DatabaseManager(ReceivedData) {}
void SignupManager::InPlacingToLocalAttributes()
{
    name = userInfo["name"];
    lastname = userInfo["lastname"];
    email = userInfo["email"];
    phonenumber = userInfo["phonenumber"];
    username = userInfo["username"];
    password = userInfo["password"];

    qDebug()<<"name:>>"+name;
}

void SignupManager::WriteDatasToFile()
{
    QFile file("signup_data.bin");
    if (file.open(QIODevice::Append))
    {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15);

        out << name;
        out << lastname;
        out << email;
        out << phonenumber;
        out << username;
        out << password;

        file.close();
        MainWindow::sendDatatoAll("User saved in binary file.");

        qDebug() << "User saved in binary file.";
    }
    else
    {
        qDebug() << "Failed to open binary file!";
    }
}

