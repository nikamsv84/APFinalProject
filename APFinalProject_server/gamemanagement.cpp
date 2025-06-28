#include "gamemanagement.h"
#include <QRandomGenerator>
#include "mainwindow.h"

GameManagement::GameManagement():DatabaseManager(""){}
GameManagement::GameManagement(QString ReceivedData):DatabaseManager(ReceivedData)
{
}
void GameManagement::InPlacingToLocalAttributes()
{
    Message = userInfo["Message"];
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

        allsockets[1]->write("\\SECOND_PLAYER\\");
        allsockets[1]->flush();
        allsockets[1]->waitForBytesWritten();
    }else{
        allsockets[1]->write("\\START\\");
        allsockets[1]->flush();
        allsockets[1]->waitForBytesWritten();

        allsockets[0]->write("\\SECOND_PLAYER\\");
        allsockets[0]->flush();
        allsockets[0]->waitForBytesWritten();
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
    }else{
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";

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


int GameManagement::IsOrderHand(QVector<QPair<int, int>> hand)      // 9
{
    return 0;
}
int GameManagement::Is4Plus1Hand(QVector<QPair<int, int>> hand)     // 8
{
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


