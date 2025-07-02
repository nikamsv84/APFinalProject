#include "forgotpassword.h"
#include <QList>
#include <QMap>
#include<QDebug>
#include <QFile>
ForgotPassword::ForgotPassword(QString ReceivedData):DatabaseManager(ReceivedData) {}
void ForgotPassword::InPlacingToLocalAttributes()
{
    PhoneNumber = userInfo["phonenumber"];
    NewPassword = userInfo["new_password"];
    qDebug()<<"phonenumber: "<<PhoneNumber;
    qDebug()<<"new_password: "<<NewPassword;
}

void ForgotPassword::EditForgetPassword( QTcpSocket* _socket)
{
    QFile file("signup_data.bin");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading!";
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);

    struct UserData {
        QString name, lastname, email, phone, username, password;
    };

    QVector<UserData> users;

    while (!in.atEnd()) {
        UserData u;
        in >> u.name >> u.lastname >> u.email >> u.phone >> u.username >> u.password;
        users.append(u);
    }

    file.close();

    bool userFound = false;
    //searching with phonenumber and changing the password;
    for (auto& user : users) {
        if (user.phone == PhoneNumber) {
            user.password = NewPassword;
            userFound = true;
            break;
        }
    }

    if (!userFound) {
        qDebug() << "No user found with this phone number.";
        _socket->write("\\ERRORFORGOTPASSWORD\\");

        return;
    }
    //rewrite
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to open file for writing!";
        _socket->write("\\ERRORFORGOTPASSWORD\\");

        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    for (const auto& u : users) {
        out << u.name << u.lastname << u.email << u.phone << u.username << u.password;
    }

    file.close();
    _socket->write("\\OKFORGOTPASSWORD\\");

    qDebug() << "Password updated successfully.";
}
