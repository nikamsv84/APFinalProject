#include "loginmanager.h"
#include <QFile>
LogInManager::LogInManager(QString ReceivedData):DatabaseManager(ReceivedData) {}
void LogInManager::InPlacingToLocalAttributes()
{
    Username = userInfo["Username"];
    Password = userInfo["Password"];
    qDebug()<<"username: "<<Username;
    qDebug()<<"password: "<<Password;
}

void LogInManager::LoginMainPage(QTcpSocket* _socket)
{
    QFile file("signup_data.bin");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open the file!";
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);

    while (!in.atEnd())
    {
        QString name, lastname, email, phonenumber, username, password;

        in >> name >> lastname >> email >> phonenumber >> username >> password;

        if (username == this->Username && password == this->Password)
        {
            qDebug() << "OKLOGIN";
            _socket->write("\\OKLOGIN\\");
            file.close();
        }
    }

    file.close();
}
