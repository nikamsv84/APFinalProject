#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H
#include <QString>
#include <databasemanager.h>
class LogInManager:public DatabaseManager
{
    QString Username;
    QString Password;
public:
    LogInManager(QString ReceivedData);
    void InPlacingToLocalAttributes() override;
};

#endif // LOGINMANAGER_H
