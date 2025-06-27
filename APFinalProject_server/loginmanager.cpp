#include "loginmanager.h"

LogInManager::LogInManager(QString ReceivedData):DatabaseManager(ReceivedData) {}
void LogInManager::InPlacingToLocalAttributes()
{
    Username = userInfo["Username"];
    Password = userInfo["Password"];
    qDebug()<<"username: "<<Username;
    qDebug()<<"password: "<<Password;
}


