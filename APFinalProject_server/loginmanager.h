#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H
#include <QString>
#include <databasemanager.h>
#include <QTcpSocket>

class LogInManager:public DatabaseManager
{
    QString Username;
    QString Password;
public:
    LogInManager(QString ReceivedData);
    void InPlacingToLocalAttributes() override;
    void LoginMainPage(QTcpSocket* _socket);
};

#endif // LOGINMANAGER_H
