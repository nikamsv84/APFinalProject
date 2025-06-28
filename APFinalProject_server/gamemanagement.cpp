#include "gamemanagement.h"
#include <QRandomGenerator>
#include <algorithm>
#include "mainwindow.h"
#include <random>
#include <chrono>

using namespace std;

GameManagement::GameManagement():DatabaseManager(""){
    for (int degree = 1; degree<=4; degree++)
    {
        for(int name = 1; name<=14; name++)
        {
            AllCards.append(qMakePair(degree, name));
        }
    }
}
GameManagement::GameManagement(QString ReceivedData):DatabaseManager(ReceivedData)
{
    for (int degree = 1; degree<=4; degree++)
    {
        for(int name = 1; name<=14; name++)
        {
            AllCards.append(qMakePair(degree, name));
        }
    }
}
void GameManagement::InPlacingToLocalAttributes()
{
    Message = userInfo["Message"];
}

void GameManagement::ChargingCards()
{
    if (CardsInARound.size() == 17)//52-35 35 cards are used in every round.
    {
        CardsInARound = AllCards;
        ++GameRound;
    }
}

void GameManagement::ShuffelingAndSendCard(QTcpSocket* _socket, QList<QTcpSocket*> allsockets)
{
    int handCounter = 1;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::shuffle(CardsInARound.begin(), CardsInARound.end(), rng);
    if (handCounter%2 == 1)
    {
        CardsInOneHand = CardsInARound.mid(0, 7);
        CardsInARound.erase(CardsInARound.begin(), CardsInARound.begin() + 7);
        QString sentCard = "\\CHOOSENCARD\\";
        for (int i=0; i<7; i++)
        {
            sentCard += "/"+ QString::number(CardsInOneHand[i].first)+":"+QString::number(CardsInOneHand[i].second);
        }

        _socket->write(sentCard.toUtf8());

    }else if(handCounter%2 == 0)
    {
        QPair<int, int> LastOpponentChose;

        if (_socket->objectName() == allsockets[0]->objectName()) {
            LastOpponentChose = GamerHands[allsockets[1]->objectName()].back();
        } else {
            LastOpponentChose = GamerHands[allsockets[0]->objectName()].back();
        }

        QString sentCard = "\\CHOOSENCARD\\";
        for (const auto& card : CardsInOneHand) {
            if (card != LastOpponentChose) {
                sentCard += "/" + QString::number(card.first) + ":" + QString::number(card.second);
            }
        }

        _socket->write(sentCard.toUtf8());

        CardsInOneHand.clear();
    }

    ++handCounter;
    if (handCounter>10)
    {
        handCounter = 1;
    }
}


void GameManagement::Communicate(QTcpSocket* _socket, QList<QTcpSocket*> allsockets)
{
    //this function should be used after stringhandling
    if (_socket->objectName() == allsockets[0]->objectName())
    {
        qDebug()<<"Message: "<<userInfo["Message"];
        allsockets[1]->write(userInfo["Message"].toUtf8());
    }else
    {
        allsockets[0]->write(userInfo["Message"].toUtf8());
    }
}

void GameManagement::choosingStarter(QList<QTcpSocket*> allsockets)
{
    int cardValue_1 = QRandomGenerator::global()->bounded(1, 14);//related to socket1
    int cardValue_2 = QRandomGenerator::global()->bounded(1, 14);//related to socket2
    if (cardValue_1>cardValue_2)
    {
        allsockets[0]->write("\\START\\");
        allsockets[0]->flush();
        allsockets[0]->waitForBytesWritten();
    }else{
        allsockets[1]->write("\\START\\");
        allsockets[1]->flush();
        allsockets[1]->waitForBytesWritten();
    }
}

void GameManagement::EndOfTimeOut(QTcpSocket* _socket, QList<QTcpSocket*> allsockets)
{
    //it should be used after messagehandling function.
    if(_socket->objectName() == allsockets[0]->objectName())
    {
        MainWindow::sendDatatoAll("\\WINNER\\,Winner: "+allsockets[1]->objectName());
    }else{
        MainWindow::sendDatatoAll("\\WINNER\\,Winner:"+allsockets[0]->objectName());

    }
}

void GameManagement::ShowOpponent(QList<QTcpSocket*> allsockets)
{
    //it sends the username of the players for each other.
    if (allsockets.size() == 2)
    {
        allsockets[0]->write("\\OPPONENT\\,Name:"+allsockets[1]->objectName().toUtf8());
        allsockets[1]->write("\\OPPONENT\\,Name:"+allsockets[0]->objectName().toUtf8());
    }else{
        MainWindow::sendDatatoAll("\\ERROROPPONENT\\");
    }


}

void GameManagement::ChooseAndRankMatching(QTcpSocket* _socket, QList<QTcpSocket*> allsockets)
{
    GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 13));
    GamerHands[allsockets[0]->objectName()].append(qMakePair(2, 12));
    GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 11));
    GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 10));
    GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 9));

    GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 13));
    GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 12));
    GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 11));
    GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 10));
    // GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 9));


    if (_socket->objectName() == allsockets[0]->objectName())
    {
        if (GamerHands[allsockets[0]->objectName()].size() <5){

            GamerHands[allsockets[0]->objectName()].append(qMakePair(userInfo["Degree"].toInt(),userInfo["Name"].toInt()));
            qDebug()<<"the degree ::"+QString::number(GamerHands[allsockets[0]->objectName()][0].first);
            qDebug()<<"the Name ::"+QString::number(GamerHands[allsockets[0]->objectName()][0].second);
            if (GamerHands[allsockets[1]->objectName()].size() ==5 && GamerHands[allsockets[0]->objectName()].size() ==5)
            {
                qDebug()<<"rankmaching should be called";
                RankMatching(GamerHands, _socket,allsockets);
            }


        }
        else{

            qDebug()<<"hand are full";

        }


    }else{
        if (GamerHands[allsockets[1]->objectName()].size() <5){

            GamerHands[allsockets[1]->objectName()].append(qMakePair(userInfo["Degree"].toInt(),userInfo["Name"].toInt()));
            qDebug()<<"the degree ::"+QString::number(GamerHands[allsockets[0]->objectName()][0].first);
            qDebug()<<"the Name ::"+QString::number(GamerHands[allsockets[0]->objectName()][0].second);
            if (GamerHands[allsockets[0]->objectName()].size() ==5 && GamerHands[allsockets[1]->objectName()].size() ==5)
            {
                qDebug()<<"rankmaching should be called";
                RankMatching(GamerHands, _socket,allsockets);
            }


        }
        else{

            qDebug()<<"hand are full";

        }
    }
}

void GameManagement::RankMatching(QMap<QString, QVector<QPair<int, int>>> hands, QTcpSocket* _socket, QList<QTcpSocket*> allsockets)
{
    QList<int (*)(QVector<QPair<int, int>>)> rankCheckers = {
        IsGoldenHand,     // 10
        IsOrderHand,      // 9
        Is4Plus1Hand,     // 8
        IsPenthouseHand,  // 7
        IsMSCHand,        // 6
        IsSeries,         // 5
        Is3Plus2Hand,     // 4
        DoublePairHand,   // 3
        IsSinglePairHand  // 2
    };


    QVector<QPair<int, int>> hand1 = hands[allsockets[0]->objectName()];
    QVector<QPair<int, int>> hand2 = hands[allsockets[1]->objectName()];

    int ranknumber_gamer1 = 0;
    int ranknumber_gamer2 = 0;

    for (int i = 0; i < rankCheckers.size(); ++i) {
        int result = rankCheckers[i](hand1);
        if (result > 0) {
            ranknumber_gamer1 = result;
            break;
        }
    }

    for (int i = 0; i < rankCheckers.size(); ++i) {
        int result = rankCheckers[i](hand2);
        if (result > 0) {
            ranknumber_gamer2 = result;
            break;
        }
    }

    qDebug()<<ranknumber_gamer1;
    qDebug()<<ranknumber_gamer2;

    if (ranknumber_gamer1>ranknumber_gamer2)
    {
        qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
        MainWindow::sendDatatoAll("\\WINNER\\,Winner:"+allsockets[0]->objectName());
        GamerHands[allsockets[0]->objectName()].clear();
        GamerHands[allsockets[1]->objectName()].clear();
        qDebug()<<GamerHands[allsockets[0]->objectName()].size();

    }else if (ranknumber_gamer1<ranknumber_gamer2){
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        MainWindow::sendDatatoAll("\\WINNER\\,Winner:"+allsockets[1]->objectName());
        GamerHands[allsockets[0]->objectName()].clear();
        GamerHands[allsockets[1]->objectName()].clear();

    }else if(ranknumber_gamer1==ranknumber_gamer2){
        qDebug()<<"function RanksAreTheSame() should be called";
        //after that these two lines should be called:
        // GamerHands[allsockets[0]->objectName()].clear();
        // GamerHands[allsockets[1]->objectName()].clear();
    }

}



int GameManagement::IsGoldenHand(QVector<QPair<int, int>> hand) //10
{

    int firstValue = hand[0].first;
    for (const auto& pair : hand) {
        if (pair.first != firstValue) {
            return 0;
        }
    }

    if (hand[0].second == 13 &&
        hand[1].second == 12 &&
        hand[2].second == 11 &&
        hand[3].second == 10 &&
        hand[4].second == 9)
    {
        return 10;
    }

    return 0;
}


int GameManagement::IsOrderHand(QVector<QPair<int, int>> hand) // 9
{

    int firstValue = hand[0].first;
    for (const auto& pair : hand) {
        if (pair.first != firstValue) {
            return 0;
        }
    }

    QVector<int> seconds;
    for (const auto& pair : hand) {
        seconds.append(pair.second);
    }

    bool isAscending = is_sorted(seconds.begin(), seconds.end());

    bool isDescending = is_sorted(seconds.begin(), seconds.end(), std::greater<int>());

    if (isAscending || isDescending) {
        return 9;
    }

    return 0;
}

int GameManagement::Is4Plus1Hand(QVector<QPair<int, int>> hand)     // 8
{
    //Degreeses should be different from each other.

    QSet<int> seen;
    for (const auto& pair : hand) {
        if (seen.contains(pair.first)) {
            return 0;
        }
        seen.insert(pair.first);
    }

    QVector<int> seconds;
    for (const auto& pair : hand) {
        seconds.append(pair.second);
    }

    QMap<int, int> frequency;
    for (int val : seconds) {
        frequency[val]++;
        if (frequency[val] == 4) {
            return 8;
        }
    }
    return 0;
}
int GameManagement::IsPenthouseHand(QVector<QPair<int, int>> hand)  // 7
{
    return 0;

}
int GameManagement::IsMSCHand(QVector<QPair<int, int>> hand)        // 6
{
    return 0;

}
int GameManagement::IsSeries(QVector<QPair<int, int>> hand)         // 5
{
    return 0;

}
int GameManagement::Is3Plus2Hand(QVector<QPair<int, int>> hand)     // 4
{
    return 0;

}
int GameManagement::DoublePairHand(QVector<QPair<int, int>> hand)    // 3
{
    return 0;

}
int GameManagement::IsSinglePairHand(QVector<QPair<int, int>> hand) // 2
{
    return 0;

}


