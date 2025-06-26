#include "signupmanager.h"
#include <mainwindow.h>
#include <QList>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QTextStream>

SignupManager::SignupManager(QString ReceivedData):ReceivedData(ReceivedData) {}
void SignupManager::Messagehandeler()
{
    ReceivedData.remove("\\SIGNUP\\,");
    QStringList fields = ReceivedData.split(',');
    QMap<QString, QString> signupInfo;
    for (const QString& field : fields) {
        QStringList keyValue = field.split(':');
        if (keyValue.size() == 2) {
            signupInfo[keyValue[0]] = keyValue[1];
        }
    }

    name = signupInfo["name"];
    lastname = signupInfo["lastname"];
    email = signupInfo["email"];
    phonenumber = signupInfo["phonenumber"];
    username = signupInfo["username"];
    password = signupInfo["password"];

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
