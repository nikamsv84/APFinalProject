#include "forgotpassword.h"
#include <QList>
#include <QMap>
#include<QDebug>
#include <QFile>
ForgotPassword::ForgotPassword(QString ReceivedData):ReceivedData(ReceivedData) {}

void ForgotPassword::Messagehandeler()
{
    ReceivedData.remove("\\FORGOTPASSWORD\\,");
    QStringList fields = ReceivedData.split(',');
    QMap<QString, QString> forgotpassword;
    for (const QString& field : fields) {
        QStringList keyValue = field.split(':');
        if (keyValue.size() == 2) {
            forgotpassword[keyValue[0]] = keyValue[1];
        }
    }

    PhoneNumber = forgotpassword["PhoneNumber"];
    NewPassword = forgotpassword["NewPassword"];

    qDebug()<<"Phonenumber:"+PhoneNumber;
    qDebug()<<"NewPassword:"+NewPassword;

}

void ForgotPassword::EditForgetPassword()
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
        return;
    }
    //rewrite
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to open file for writing!";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    for (const auto& u : users) {
        out << u.name << u.lastname << u.email << u.phone << u.username << u.password;
    }

    file.close();
    qDebug() << "Password updated successfully.";
}
