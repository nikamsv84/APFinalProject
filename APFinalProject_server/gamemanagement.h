#ifndef GAMEMANAGEMENT_H
#define GAMEMANAGEMENT_H
#include <QString>
#include <QTcpSocket>
#include <QDate>
#include "databasemanager.h"
struct UserHistory
{
    QString OpponentName;
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
    int handCounter = 0;

public:
    static QVector<QPair<int, int>> AllCards;
    static QVector<QPair<int, int>> CardsInARound;
    static QVector<QPair<int, int>> CardsInOneHand;

    static QMap<QString,UserHistory> GameResults;
    static QMap<QString, QVector<QPair<int, int>>> GamerHands;

    GameManagement();
    GameManagement(QString ReceivedData);
    void InPlacingToLocalAttributes() override;
    void ChargingCards();
    void sendLooserWinner(int WinnerIndex, int LooserIndex, QList<QTcpSocket*> allsockets);

    // void ShuffelingAndSendCard(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void ShuffelingAndSendCard(QList<QTcpSocket*> allsockets);
    void Communicate(QTcpSocket* _socket, const QList<QTcpSocket*>& allsockets);
    void choosingStarter(QList<QTcpSocket*> allsockets);
    void StoringChooses(QTcpSocket* _socket, QList<QTcpSocket*> allsockets);
    void RankMatching(QList<QTcpSocket*> allsockets);
    void RanksAreTheSame(QList<QTcpSocket*> allsockets, int Rank);
    void ShowOpponent(QList<QTcpSocket*> allsockets);
    void ChargingCardsForFirstTime();

    void ChangeCardProcess(QTcpSocket* _socket,QList<QTcpSocket*> allsockets);

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

    //if the ranks are the same:
    void IfAllWasTheSame(QList<QTcpSocket*> allsockets);
    void SameGoldenHand(QList<QTcpSocket*> allsockets);
    void SameOrderHand(QList<QTcpSocket*> allsockets);
    void Same4Plus1Hand(QList<QTcpSocket*> allsockets);
    void SamePenthouseHand(QList<QTcpSocket*> allsockets);
    void SameMSCHand(QList<QTcpSocket*> allsockets);
    void SameSeries(QList<QTcpSocket*> allsockets);
    void Same3Plus2Hand(QList<QTcpSocket*> allsockets);
    void SameDoublePairHand(QList<QTcpSocket*> allsockets);
    void SameSinglePairHand(QList<QTcpSocket*> allsockets);
    void SameMessyHand(QList<QTcpSocket*> allsockets);

    //displaying the results:
    void ShowFinalWinner(QList<QTcpSocket*> allsockets);
    void SavingFinalResultsToHistory();
    void ShowHistory(QTcpSocket* _socket);





};

#endif // GAMEMANAGEMENT_H
