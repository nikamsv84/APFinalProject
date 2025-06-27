#include "editinfo.h"
#include <QList>
#include <QMap>
#include <QFile>
EditInfo::EditInfo(QString ReceivedData):DatabaseManager(ReceivedData){}
void EditInfo::InPlacingToLocalAttributes(){
    DataType = userInfo["DataType"];
    PreviousData= userInfo["PreviousData"];
    NewData = userInfo["NewData"];

    qDebug()<<"DataType:"+DataType;
    qDebug()<<"PreviousData: "+PreviousData;
    qDebug()<<"NewData: "+NewData;
}
void EditInfo::EditSpecificField()
{
    QFile file("signup_data.bin");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading!";
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

    bool fieldUpdated = false;

    for (auto& user : users) {
        QString targetField;

        if (DataType == "name" && user.name == PreviousData) {
            user.name = NewData; fieldUpdated = true;
        } else if (DataType == "lastname" && user.lastname == PreviousData) {
            user.lastname = NewData; fieldUpdated = true;
        } else if (DataType == "email" && user.email == PreviousData) {
            user.email = NewData; fieldUpdated = true;
        } else if (DataType == "phone" && user.phone == PreviousData) {
            user.phone = NewData; fieldUpdated = true;
        } else if (DataType == "username" && user.username == PreviousData) {

        //checking the username is unique or not?
            bool duplicate = false;
            for (const auto& u : users) {
                if (u.username == NewData) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                qDebug() << "Username already exists!";
                return;
            }
            user.username = NewData;
            fieldUpdated = true;
        } else if (DataType == "password" && user.password == PreviousData) {
            user.password = NewData; fieldUpdated = true;
        }
    }

    if (!fieldUpdated) {
        qDebug() << "No matching field found to update!";
        return;
    }

    //rewrite data.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Cannot open file for writing!";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    for (const auto& u : users) {
        out << u.name << u.lastname << u.email << u.phone << u.username << u.password;
    }

    file.close();
    qDebug() << DataType << " field updated successfully.";
}

