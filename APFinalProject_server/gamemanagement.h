#ifndef GAMEMANAGEMENT_H
#define GAMEMANAGEMENT_H
#include <QString>
#include <QTcpSocket>
#include "databasemanager.h"
struct UserHistory
{
    QString Date;
    QString result1;
    QString result2;
    QString result3;
    QString FinalResult;
};

class GameManagement:public DatabaseManager
{
    QString Message;
    QMap<QString,UserHistory> GameResults;
    QMap<QString, QVector<QPair<int, int>>> GamerHands;

public:
    GameManagement();
    GameManagement(QString ReceivedData);
    void InPlacingToLocalAttributes() override;
    void Communicate(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void choosingStarter(QList<QTcpSocket*> allsockets);
    void EndOfTimeOut(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void ChooseAndRankMatching(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
};

#endif // GAMEMANAGEMENT_H
