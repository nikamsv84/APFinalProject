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

void SignupManager::WriteDatasToFile(QTcpSocket* _socket)
{
        QFile file("signup_data.bin");
        if (!file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly)) {
            qDebug() << "Could not open file for reading.";
            return;
        }

        //checking if the username is unique or not:
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);

        while (!in.atEnd()) {
            QString existingName, existingLast, existingEmail, existingPhone, existingUser, existingPass;
            in >> existingName >> existingLast >> existingEmail >> existingPhone >> existingUser >> existingPass;

            if (existingUser == username) {
                qDebug() << "this username is already exists.";
                _socket->write("this username is already exists.");
                file.close();
                return;
            }
        }

        file.close();


        if (file.open(QIODevice::Append)) {
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_15);

            out << name << lastname << email << phonenumber << username << password;

            file.close();
            MainWindow::sendDatatoAll("User saved in binary file.");
            qDebug() << "User saved in binary file.";
        } else {
            qDebug() << "Failed to open binary file for writing!";
        }
}



