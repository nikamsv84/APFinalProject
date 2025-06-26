#ifndef SIGNUPMANAGER_H
#define SIGNUPMANAGER_H
#include <QString>
#include <QTcpSocket>
class SignupManager
{
    QString ReceivedData;
    QString name;
    QString lastname;
    QString phonenumber;
    QString email;
    QString username;
    QString password;


public:
    SignupManager(QString ReceivedData);
    void Messagehandeler();
    void WriteDatasToFile();
};

#endif // SIGNUPMANAGER_H
