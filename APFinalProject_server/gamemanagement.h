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
    int GameRound = 0;
    QVector<QPair<int, int>> AllCards;
    QVector<QPair<int, int>> CardsInARound;
    QVector<QPair<int, int>> CardsInOneHand;

    QMap<QString,UserHistory> GameResults;
    QMap<QString, QVector<QPair<int, int>>> GamerHands;



public:
    GameManagement();
    GameManagement(QString ReceivedData);
    void InPlacingToLocalAttributes() override;
    void ChargingCards();
    void ShuffelingAndSendCard(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void Communicate(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void choosingStarter(QList<QTcpSocket*> allsockets);
    void EndOfTimeOut(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void ChooseAndRankMatching(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void RankMatching(QMap<QString, QVector<QPair<int, int>>> hands, QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void ShowOpponent(QList<QTcpSocket*> allsockets);

    //we assigned a number to every hand , the below methodes return the number of hand if the hand is golden or ... otherwise they return 0.
    static int IsGoldenHand(QVector<QPair<int, int>> hand);     // 10
    static int IsOrderHand(QVector<QPair<int, int>> hand);      // 9
    static int Is4Plus1Hand(QVector<QPair<int, int>> hand);     // 8
    static int IsPenthouseHand(QVector<QPair<int, int>> hand);  // 7
    static int IsMSCHand(QVector<QPair<int, int>> hand);        // 6
    static int IsSeries(QVector<QPair<int, int>> hand);         // 5
    static int Is3Plus2Hand(QVector<QPair<int, int>> hand);     // 4
    static int DoublePairHand(QVector<QPair<int, int>> hand);   // 3
    static int IsSinglePairHand(QVector<QPair<int, int>> hand); // 2





};

#endif // GAMEMANAGEMENT_H
