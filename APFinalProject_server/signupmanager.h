#ifndef SIGNUPMANAGER_H
#define SIGNUPMANAGER_H
#include <QString>
#include <QTcpSocket>
#include "databasemanager.h"
class SignupManager:public DatabaseManager
{
    QString name;
    QString lastname;
    QString phonenumber;
    QString email;
    QString username;
    QString password;


public:
    SignupManager(QString ReceivedData);
    void WriteDatasToFile();
    void InPlacingToLocalAttributes() override;
};

#endif // SIGNUPMANAGER_H
