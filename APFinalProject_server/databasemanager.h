#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString>
#include <QMap>
#include <QDebug>
class DatabaseManager
{
protected:
    QString ReceivedData;
    QString Specifier;

public:
    static QMap<QString , QString>userInfo;
    DatabaseManager(QString ReceivedData);
    virtual void Messagehandeler(QString Specifier);
    virtual void InPlacingToLocalAttributes()=0;
};

#endif // DATABASEMANAGER_H
