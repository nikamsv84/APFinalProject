#ifndef GAMEMANAGEMENT_H
#define GAMEMANAGEMENT_H
#include <QString>
#include <QTcpSocket>
#include "databasemanager.h"
class GameManagement:public DatabaseManager
{
    QString Message;
public:
    GameManagement();
    GameManagement(QString ReceivedData);
    void InPlacingToLocalAttributes() override;
    void Communicate(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void choosingStarter(QList<QTcpSocket*> allsockets);
};

#endif // GAMEMANAGEMENT_H
