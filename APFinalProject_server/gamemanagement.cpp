#include "gamemanagement.h"
#include <QRandomGenerator>
#include <algorithm>
#include "mainwindow.h"
#include <random>
#include <QTimer>
#include <chrono>

using namespace std;

QVector<QPair<int, int>> GameManagement::AllCards;
QVector<QPair<int, int>> GameManagement::CardsInARound;
QVector<QPair<int, int>> GameManagement::CardsInOneHand;

QMap<QString,UserHistory> GameManagement::GameResults;
QMap<QString, QVector<QPair<int, int>>> GameManagement::GamerHands;


GameManagement::GameManagement():DatabaseManager(""){

}
GameManagement::GameManagement(QString ReceivedData):DatabaseManager(ReceivedData)
{
}

void GameManagement::ChargingCardsForFirstTime()
{
    if (AllCards.size() == 0)
    {
        for (int degree = 1; degree<=4; degree++)
        {
            for(int name = 2; name<=14; name++)
            {
                AllCards.append(qMakePair(degree, name));
            }
        }
    }
    qDebug()<<"the number of cards at first:"+QString::number(AllCards.size());
}

void GameManagement::InPlacingToLocalAttributes()
{
    Message = userInfo["Message"];
}

void GameManagement::ChargingCards()
{
    if (CardsInARound.size() == 17 || CardsInARound.size() == 0)//52-35 35 cards are used in every round.
    {
        CardsInARound = AllCards;
        qDebug()<<"cards in round at first or in the begining of a round: "+QString::number(CardsInARound.size());
        ++GameRound;
    }
}

void GameManagement::ChangeCardProcess(QTcpSocket* _socket, QList<QTcpSocket*> allsockets)
{
    qDebug()<<"we are in the changeCard process";
    QPair<int , int> cardtochange_1 = QPair<int , int> (userInfo["Degree1"].toInt(), userInfo["Name1"].toInt());
    QPair<int, int> cardtochange_2 = QPair<int , int> (userInfo["Degree2"].toInt(), userInfo["Name2"].toInt());
    qDebug()<<"card1"+QString::number(cardtochange_1.first)+QString::number(cardtochange_1.second);
    qDebug()<<"card2"+QString::number(cardtochange_2.first)+QString::number(cardtochange_2.second);


    if (_socket == allsockets[0])
    {
        int index1 = GamerHands[allsockets[0]->objectName()].indexOf(cardtochange_1);
        int index2 = GamerHands[allsockets[1]->objectName()].indexOf(cardtochange_2);
        if (index1 != -1 && index2 != -1) {
            GamerHands[allsockets[0]->objectName()][index1] = cardtochange_2;
            GamerHands[allsockets[1]->objectName()][index2] = cardtochange_1;
            qDebug()<<"card1 after changing"+QString::number(GamerHands[allsockets[0]->objectName()][index1].first)+QString::number(GamerHands[allsockets[0]->objectName()][index1].second);
            qDebug()<<"card2 after changing"+QString::number(GamerHands[allsockets[1]->objectName()][index1].first)+QString::number(GamerHands[allsockets[1]->objectName()][index1].second);

        }

    }else if (_socket == allsockets[1])
    {
        int index1 = GamerHands[allsockets[1]->objectName()].indexOf(cardtochange_1);
        int index2 = GamerHands[allsockets[0]->objectName()].indexOf(cardtochange_2);
        if (index1 != -1 && index2 != -1) {
            GamerHands[allsockets[1]->objectName()][index1] = cardtochange_2;
            GamerHands[allsockets[0]->objectName()][index2] = cardtochange_1;

            qDebug()<<"card1 after changing"+QString::number(GamerHands[allsockets[1]->objectName()][index1].first)+QString::number(GamerHands[allsockets[1]->objectName()][index1].second);
            qDebug()<<"card2 after changing"+QString::number(GamerHands[allsockets[0]->objectName()][index1].first)+QString::number(GamerHands[allsockets[0]->objectName()][index1].second);

        }
    }
}


void GameManagement::ShuffelingAndSendCard(QList<QTcpSocket*> allsockets)
{
    if (allsockets.size() < 2) return;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::shuffle(CardsInARound.begin(), CardsInARound.end(), rng);

    QList<QPair<int, int>> selectedCards = CardsInARound.mid(0, 7);

    QString sentCard = "\\CHOOSENCARD\\";
    for (const auto& card : selectedCards) {
        sentCard += "/" + QString::number(card.first) + ":" + QString::number(card.second);
    }

    for (QTcpSocket* socket : allsockets) {
        socket->write(sentCard.toUtf8());
        socket->flush();
        qDebug() << "Sent to " << socket->objectName() << ":" << sentCard;
    }

    CardsInARound.erase(CardsInARound.begin(), CardsInARound.begin() + 7);
    qDebug()<<"remaining cards: Cards in a round "+QString::number(CardsInARound.size());
}


void GameManagement::Communicate(QTcpSocket* _socket, const QList<QTcpSocket*>& allsockets) {
    if (allsockets.size() < 2) {
        qWarning() << "Not enough sockets to communicate.";
        return;
    }

    QString message = userInfo["Message"];
    if (message.isEmpty()) {
        qWarning() << "Empty message, nothing to send.";
        return;
    }

    if (_socket == allsockets[0]) {
        qDebug() << "Message from socket 0: " << message;
        allsockets[1]->write(message.toUtf8());
    } else {
        allsockets[0]->write(message.toUtf8());
        qDebug() << "Message from other socket: " << message;
    }
}


void GameManagement::choosingStarter(QList<QTcpSocket*> allsockets)
{
    int cardValue_1 = QRandomGenerator::global()->bounded(1, 14);
    int cardValue_2 = QRandomGenerator::global()->bounded(1, 14);

    int starterIndex = (cardValue_1 > cardValue_2) ? 0 : 1;
    int secondIndex = 1 - starterIndex;

    allsockets[starterIndex]->write("\\STARTER\\");
    allsockets[starterIndex]->flush();

    allsockets[secondIndex]->write("\\SECOND_PLAYER\\");
    allsockets[secondIndex]->flush();
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

void GameManagement::StoringChooses(QTcpSocket* _socket, QList<QTcpSocket*> allsockets)
{
    // GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 13));//these lines are for debug
    // GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 12));
    // GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 3));
    // GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 10));
    // GamerHands[allsockets[0]->objectName()].append(qMakePair(1, 9));

    // GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 1));
    // GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 2));
    // GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 3));
    // GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 4));
    // // GamerHands[allsockets[1]->objectName()].append(qMakePair(1, 9));


    if (_socket->objectName() == allsockets[0]->objectName())
    {
        if (GamerHands[allsockets[0]->objectName()].size() <5){

            GamerHands[allsockets[0]->objectName()].append(qMakePair(userInfo["Degree"].toInt(),userInfo["Name"].toInt()));
            qDebug()<<"the degree of another client::"+QString::number(GamerHands[allsockets[0]->objectName()][0].first);
            qDebug()<<"the Name of another client::"+QString::number(GamerHands[allsockets[0]->objectName()][0].second);
        }
        else{
            qDebug()<<"hand are full calling RankMatching needed!";

        }

    }else{
        if (GamerHands[allsockets[1]->objectName()].size() <5){

            GamerHands[allsockets[1]->objectName()].append(qMakePair(userInfo["Degree"].toInt(),userInfo["Name"].toInt()));
            qDebug()<<"the degree of other client::"+QString::number(GamerHands[allsockets[1]->objectName()][0].first);
            qDebug()<<"the Name of other client::"+QString::number(GamerHands[allsockets[1]->objectName()][0].second);
            qDebug()<<"handcounter:"+QString::number(handCounter);

        }
        else{
            qDebug()<<"hand are full calling RankMatching needed!";

        }
    }
}

void GameManagement::RankMatching(QList<QTcpSocket*> allsockets)
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
        // IsMessyHand->if ranknumbers remain 0.
    };


    QVector<QPair<int, int>> hand1 = GamerHands[allsockets[0]->objectName()];
    QVector<QPair<int, int>> hand2 = GamerHands[allsockets[1]->objectName()];

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
        sendLooserWinner(0, 1, allsockets);

        qDebug()<<GamerHands[allsockets[0]->objectName()].size();

    }else if (ranknumber_gamer1<ranknumber_gamer2){
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        sendLooserWinner(1, 0, allsockets);

    }else if(ranknumber_gamer1==ranknumber_gamer2){
        qDebug()<<"function RanksAreTheSame() should be called";
        RanksAreTheSame(allsockets, ranknumber_gamer1);
    }

}

void GameManagement::RanksAreTheSame(QList<QTcpSocket*> allsockets, int Rank)
{
    switch (Rank) {
    case 10:
        qDebug()<<"same 10";
        SameGoldenHand(allsockets);
        break;
    case 9:
        qDebug()<<"same 9";
        SameOrderHand(allsockets);

        break;
    case 8:
        qDebug()<<"same 8";
        Same4Plus1Hand(allsockets);

        break;
    case 7:
        qDebug()<<"same 7";
        SamePenthouseHand(allsockets);

        break;
    case 6:
        qDebug()<<"same 6";
        SameMSCHand(allsockets);

        break;
    case 5:
        qDebug()<<"same 5";
        SameSeries(allsockets);

        break;
    case 4:
        qDebug()<<"same 4";
        Same3Plus2Hand(allsockets);

        break;
    case 3:
        qDebug()<<"same 3";
        SameDoublePairHand(allsockets);
        break;
    case 2:
        qDebug()<<"same 2";
        SameSinglePairHand(allsockets);
        break;

    default:
        qDebug()<<"same 0 (Messy hand)";
        SameMessyHand(allsockets);
        break;
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

    if (hand[0].second == 14 &&
        hand[1].second == 13 &&
        hand[2].second == 12 &&
        hand[3].second == 11 &&
        hand[4].second == 10)
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

    QSet<int> values;
    for (const auto& pair : hand) {
        values.insert(pair.first);
    }
    if(!(values.contains(1) && values.contains(2)&& values.contains(3) && values.contains(4)))
    {
        return 0;
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
int GameManagement::IsPenthouseHand(QVector<QPair<int, int>> hand)
{
    QMap<int, QSet<int>> numberToTypes;

    for (const auto& card : hand) {
        numberToTypes[card.second].insert(card.first);
    }

    int tripleCount = 0;
    int pairCount = 0;

    for (auto it = numberToTypes.begin(); it != numberToTypes.end(); ++it) {
        int typeCount = it.value().size();

        if (typeCount == 3)
            tripleCount++;
        else if (typeCount == 2)
            pairCount++;
    }

    if (tripleCount == 1 && pairCount == 1)
        return 7;

    return 0;
}


int GameManagement::IsMSCHand(QVector<QPair<int, int>> hand)        // 6
{
    int firstValue = hand[0].first;
    for (const auto& pair : hand) {
        if (pair.first != firstValue) {
            return 0;
        }
    }
    return 6;

}
int GameManagement::IsSeries(QVector<QPair<int, int>> hand)         // 5
{
    QSet<int> values;
    for (const auto& pair : hand) {
        values.insert(pair.first);
    }
    if(!(values.contains(1) && values.contains(2)&& values.contains(3) && values.contains(4)))
    {
        return 0;
    }

    QVector<int> seconds;
    for (const auto& pair : hand) {
        seconds.append(pair.second);
    }

    bool isAscending = is_sorted(seconds.begin(), seconds.end());

    bool isDescending = is_sorted(seconds.begin(), seconds.end(), std::greater<int>());

    if (isAscending || isDescending) {
        return 5;
    }

    return 0;

}
int GameManagement::Is3Plus2Hand(QVector<QPair<int, int>> hand)
{
    if (hand.size() != 5)
        return 0;

    QMap<int, QSet<int>> numberToTypes;
    QMap<int, int> numberCount;

    for (const auto& card : hand) {
        numberToTypes[card.second].insert(card.first);
        numberCount[card.second]++;
    }

    for (auto it = numberToTypes.begin(); it != numberToTypes.end(); ++it) {
        int typeCount = it.value().size();
        int totalCount = numberCount[it.key()];

        if (typeCount == 3 && totalCount == 3) {
            return 7;
        }
    }

    return 0;
}

int GameManagement::DoublePairHand(QVector<QPair<int, int>> hand) // 3
{
    QMap<int, QSet<int>> numberToTypes;
    QMap<int, int> numberCount;

    for (const auto& card : hand) {
        numberToTypes[card.second].insert(card.first);
        numberCount[card.second]++;
    }

    QVector<int> validPairs;

    for (auto it = numberToTypes.begin(); it != numberToTypes.end(); ++it) {
        int number = it.key();
        int typeCount = it.value().size();
        int totalCount = numberCount[number];

        if (typeCount == 2 && totalCount == 2) {
            validPairs.append(number);
        }
    }

    if (validPairs.size() == 2 && numberCount.size() == 3)
        return 3;

    return 0;
}

int GameManagement::IsSinglePairHand(QVector<QPair<int, int>> hand) // 2
{
    QMap<QPair<int, int>, int> cardCount;

    for (const auto& card : hand) {
        cardCount[card]++;
    }

    int pairCount = 0;

    for (auto it = cardCount.begin(); it != cardCount.end(); ++it) {
        if (it.value() == 2) {
            pairCount++;
        } else if (it.value() > 2) {
            return 0;
        }
    }

    if (pairCount == 1)
        return 2;

    return 0;
}



//same checker functions:
void GameManagement::SameGoldenHand(QList<QTcpSocket*> allsockets)
{
    if(GamerHands[allsockets[0]->objectName()][0].first > GamerHands[allsockets[1]->objectName()][0].first)
    {
        qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
        sendLooserWinner(0, 1, allsockets);

    }else if (GamerHands[allsockets[0]->objectName()][0].first < GamerHands[allsockets[1]->objectName()][0].first){
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        sendLooserWinner(1, 0, allsockets);
    }else{
        IfAllWasTheSame(allsockets);
    }
}

void GameManagement::SameOrderHand(QList<QTcpSocket*> allsockets)
{
    if(GamerHands[allsockets[0]->objectName()][0].second > GamerHands[allsockets[1]->objectName()][0].second)
    {
        qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
        sendLooserWinner(0, 1, allsockets);

    }else if (GamerHands[allsockets[0]->objectName()][0].second < GamerHands[allsockets[1]->objectName()][0].second){
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        sendLooserWinner(1, 0, allsockets);

    }else{
        if(GamerHands[allsockets[0]->objectName()][0].first > GamerHands[allsockets[1]->objectName()][0].first)
        {
            qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
            sendLooserWinner(0, 1, allsockets);
        }else if (GamerHands[allsockets[0]->objectName()][0].first< GamerHands[allsockets[1]->objectName()][0].first){
            qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
            sendLooserWinner(1, 0, allsockets);
        }else{
            IfAllWasTheSame(allsockets);
        }
    }

}

void GameManagement::Same4Plus1Hand(QList<QTcpSocket*> allsockets)
{
    QVector<int> seconds_player1;
    for (const auto& pair : GamerHands[allsockets[0]->objectName()]) {
        seconds_player1.append(pair.second);
    }

    QMap<int, int> frequency_player1;
    int cardsValue_player1 = -1;
    for (int val : seconds_player1) {
        frequency_player1[val]++;
        if (frequency_player1[val] == 4) {
            cardsValue_player1 = val;
        }
    }

    QVector<int> seconds_player2;
    for (const auto& pair : GamerHands[allsockets[1]->objectName()]) {
        seconds_player2.append(pair.second);
    }

    QMap<int, int> frequency_player2;
    int cardsValue_player2 = -1;
    for (int val : seconds_player2) {
        frequency_player2[val]++;
        if (frequency_player2[val] == 4) {
            cardsValue_player2 = val;
        }
    }

    if (cardsValue_player1 > cardsValue_player2) {
        qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
        sendLooserWinner(0, 1, allsockets);

    } else if (cardsValue_player2 > cardsValue_player1) {
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        sendLooserWinner(1, 0, allsockets);

    } else {
        qDebug()<<"the 4 card\'s value were the same.";
        IfAllWasTheSame(allsockets);
    }
}


void GameManagement::SamePenthouseHand(QList<QTcpSocket*> allsockets)
{
    QMap<int, QSet<int>> numberToTypes_player1;

    for (const auto& card : GamerHands[allsockets[0]->objectName()]) {
        numberToTypes_player1[card.second].insert(card.first);
    }

    int tripleNumber_player1 = -1;

    for (auto it = numberToTypes_player1.begin(); it != numberToTypes_player1.end(); ++it) {
        int typeCount = it.value().size();

        if (typeCount == 3) {
            tripleNumber_player1 = it.key();
            break;
        }
    }

    QMap<int, QSet<int>> numberToTypes_player2;

    for (const auto& card : GamerHands[allsockets[1]->objectName()]) {
        numberToTypes_player2[card.second].insert(card.first);
    }

    int tripleNumber_player2 = -1;

    for (auto it = numberToTypes_player2.begin(); it != numberToTypes_player2.end(); ++it) {
        int typeCount = it.value().size();

        if (typeCount == 3) {
            tripleNumber_player2 = it.key();
            break;
        }
    }

    if(tripleNumber_player1>tripleNumber_player2)
    {
        qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
        sendLooserWinner(0, 1, allsockets);
    }else if (tripleNumber_player1<tripleNumber_player2){
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        sendLooserWinner(1, 0, allsockets);
    }else{
        IfAllWasTheSame(allsockets);
    }


}

void GameManagement::SameMSCHand(QList<QTcpSocket*> allsockets)
{
    QVector<int> numbers_player1;
    QVector<int> numbers_player2;

    for(auto card: GamerHands[allsockets[0]->objectName()])
    {
        numbers_player1.append(card.second);
    }

    for(auto card: GamerHands[allsockets[1]->objectName()])
    {
        numbers_player1.append(card.second);
    }

    while (!numbers_player1.isEmpty() && !numbers_player2.isEmpty()) {
        int maxnum_player1 = *std::max_element(numbers_player1.begin(), numbers_player1.end());
        int maxnum_player2 = *std::max_element(numbers_player2.begin(), numbers_player2.end());

        if(maxnum_player1>maxnum_player2)
        {
            qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
            sendLooserWinner(0, 1, allsockets);
        }else if (maxnum_player1<maxnum_player2)
        {
            qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
            sendLooserWinner(1, 0, allsockets);
        }
        else if (maxnum_player1 == maxnum_player2) {
            numbers_player1.removeOne(maxnum_player1);
            numbers_player2.removeOne(maxnum_player2);
        }
    }

    IfAllWasTheSame(allsockets);

}

void GameManagement::SameSeries(QList<QTcpSocket*> allsockets)
{
    if (GamerHands[allsockets[0]->objectName()][0].second> GamerHands[allsockets[1]->objectName()][0].second)
    {
        qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
        sendLooserWinner(0, 1, allsockets);
    }else if (GamerHands[allsockets[1]->objectName()][1].second>GamerHands[allsockets[1]->objectName()][0].second)
    {
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        sendLooserWinner(1, 0, allsockets);
    }else if (GamerHands[allsockets[0]->objectName()][0].second ==  GamerHands[allsockets[1]->objectName()][0].second)
    {
        if (GamerHands[allsockets[0]->objectName()][0].first >  GamerHands[allsockets[1]->objectName()][0].first)
        {
            qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
            sendLooserWinner(0, 1, allsockets);
        }else if (GamerHands[allsockets[0]->objectName()][0].first <  GamerHands[allsockets[1]->objectName()][0].first)
        {
            qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
            sendLooserWinner(1, 0, allsockets);
        }else
        {
            qDebug()<<"the Degree and number of cards were the same";
            IfAllWasTheSame(allsockets);
        }
    }
}

void GameManagement::Same3Plus2Hand(QList<QTcpSocket*> allsockets)
{
    QMap<int, QSet<int>> numberToTypes_player1;

    for (const auto& card : GamerHands[allsockets[0]->objectName()]) {
        numberToTypes_player1[card.second].insert(card.first);
    }

    int tripleNumber_player1 = -1;

    for (auto it = numberToTypes_player1.begin(); it != numberToTypes_player1.end(); ++it) {
        int typeCount = it.value().size();

        if (typeCount == 3) {
            tripleNumber_player1 = it.key();
            break;
        }
    }

    QMap<int, QSet<int>> numberToTypes_player2;

    for (const auto& card : GamerHands[allsockets[1]->objectName()]) {
        numberToTypes_player2[card.second].insert(card.first);
    }

    int tripleNumber_player2 = -1;

    for (auto it = numberToTypes_player2.begin(); it != numberToTypes_player2.end(); ++it) {
        int typeCount = it.value().size();

        if (typeCount == 3) {
            tripleNumber_player2 = it.key();
            break;
        }
    }

    if(tripleNumber_player1>tripleNumber_player2)
    {
        qDebug()<<"gamer 1 with username: "+allsockets[0]->objectName()+"won";
        sendLooserWinner(0, 1, allsockets);
    }else if (tripleNumber_player1<tripleNumber_player2){
        qDebug()<<"gamer 2 with username: "+allsockets[1]->objectName()+"won";
        sendLooserWinner(1, 0, allsockets);
    }else{
        IfAllWasTheSame(allsockets);
    }
}

void GameManagement::SameDoublePairHand(QList<QTcpSocket*> allsockets)
{
    // --- Player 1 ---
    QMap<int, QSet<int>> numberToTypes_player1;
    QMap<int, int> numberCount_player1;

    for (const auto& card : GamerHands[allsockets[0]->objectName()]) {
        numberToTypes_player1[card.second].insert(card.first);
        numberCount_player1[card.second]++;
    }

    QVector<int> validPairs_player1;
    for (auto it = numberToTypes_player1.begin(); it != numberToTypes_player1.end(); ++it) {
        int number = it.key();
        int typeCount = it.value().size();
        int totalCount = numberCount_player1[number];

        if (typeCount == 2 && totalCount == 2) {
            validPairs_player1.append(number);
        }
    }
    std::sort(validPairs_player1.begin(), validPairs_player1.end(), std::greater<int>());

    // --- Player 2 ---
    QMap<int, QSet<int>> numberToTypes_player2;
    QMap<int, int> numberCount_player2;

    for (const auto& card : GamerHands[allsockets[1]->objectName()]) {
        numberToTypes_player2[card.second].insert(card.first);
        numberCount_player2[card.second]++;
    }

    QVector<int> validPairs_player2;
    for (auto it = numberToTypes_player2.begin(); it != numberToTypes_player2.end(); ++it) {
        int number = it.key();
        int typeCount = it.value().size();
        int totalCount = numberCount_player2[number];

        if (typeCount == 2 && totalCount == 2) {
            validPairs_player2.append(number);
        }
    }
    std::sort(validPairs_player2.begin(), validPairs_player2.end(), std::greater<int>());

    // --- Compare pairs ---
    if (validPairs_player1[0] > validPairs_player2[0]) {
        qDebug() << "Player 1 wins with higher first pair";
        sendLooserWinner(0, 1, allsockets);
        return;
    } else if (validPairs_player1[0] < validPairs_player2[0]) {
        qDebug() << "Player 2 wins with higher first pair";
        sendLooserWinner(1, 0, allsockets);
        return;
    }

    // First pairs are equal, compare second pairs
    if (validPairs_player1[1] > validPairs_player2[1]) {
        qDebug() << "Player 1 wins with higher second pair";
        sendLooserWinner(0, 1, allsockets);
        return;
    } else if (validPairs_player1[1] < validPairs_player2[1]) {
        qDebug() << "Player 2 wins with higher second pair";
        sendLooserWinner(1, 0, allsockets);
        return;
    }

    // Both pairs are equal, compare kicker
    int kicker1 = -1;
    int kicker2 = -1;

    for (const auto& card : GamerHands[allsockets[0]->objectName()]) {
        if (!validPairs_player1.contains(card.second)) {
            kicker1 = card.second;
            break;
        }
    }

    for (const auto& card : GamerHands[allsockets[1]->objectName()]) {
        if (!validPairs_player2.contains(card.second)) {
            kicker2 = card.second;
            break;
        }
    }

    if (kicker1 > kicker2) {
        qDebug() << "Player 1 wins with higher kicker";
        sendLooserWinner(0, 1, allsockets);
    } else if (kicker1 < kicker2) {
        qDebug() << "Player 2 wins with higher kicker";
        sendLooserWinner(1, 0, allsockets);
    } else {
        qDebug() << "It's a tie!";
        IfAllWasTheSame(allsockets);
    }
}


void GameManagement::SameSinglePairHand(QList<QTcpSocket*> allsockets)
{
    // --- Player 1 ---
    QMap<int, int> numberCount_player1;
    QVector<int> kickers1;
    int pair1 = -1;

    for (const auto& card : GamerHands[allsockets[0]->objectName()]) {
        numberCount_player1[card.second]++;
    }

    for (auto it = numberCount_player1.begin(); it != numberCount_player1.end(); ++it) {
        if (it.value() == 2)
            pair1 = it.key();
        else {
            for (int i = 0; i < it.value(); ++i)
                kickers1.append(it.key());
        }
    }

    // --- Player 2 ---
    QMap<int, int> numberCount_player2;
    QVector<int> kickers2;
    int pair2 = -1;

    for (const auto& card : GamerHands[allsockets[1]->objectName()]) {
        numberCount_player2[card.second]++;
    }

    for (auto it = numberCount_player2.begin(); it != numberCount_player2.end(); ++it) {
        if (it.value() == 2)
            pair2 = it.key();
        else {
            for (int i = 0; i < it.value(); ++i)
                kickers2.append(it.key());
        }
    }

    // --- Compare pairs ---
    if (pair1 > pair2) {
        qDebug() << "Player 1 wins with higher pair";
        sendLooserWinner(0, 1, allsockets);
        return;
    } else if (pair1 < pair2) {
        qDebug() << "Player 2 wins with higher pair";
        sendLooserWinner(1, 0, allsockets);
        return;
    }

    // --- Compare kickers without sorting ---
    for (int i = 0; i < 3; ++i) {
        int max1 = *std::max_element(kickers1.begin(), kickers1.end());
        int max2 = *std::max_element(kickers2.begin(), kickers2.end());

        if (max1 > max2) {
            qDebug() << "Player 1 wins with higher kicker";
            sendLooserWinner(0, 1, allsockets);
            return;
        } else if (max1 < max2) {
            qDebug() << "Player 2 wins with higher kicker";
            sendLooserWinner(1, 0, allsockets);
            return;
        }

        // Remove used max to compare next highest
        kickers1.removeOne(max1);
        kickers2.removeOne(max2);
    }

    qDebug() << "It's a tie!";
    IfAllWasTheSame(allsockets);
}

void GameManagement::SameMessyHand(QList<QTcpSocket*> allsockets)
{
    QVector<QPair<int, int>> player1_cards = GamerHands[allsockets[0]->objectName()];
    QVector<QPair<int, int>> player2_cards = GamerHands[allsockets[1]->objectName()];

    for (int i=0; i<=5; ++i)
    {
        if (player1_cards[i].second>player2_cards[i].second)
        {
            sendLooserWinner(0, 1, allsockets);
            return;

        }else if (player1_cards[i].second<player2_cards[i].second)
        {
            sendLooserWinner(1, 0, allsockets);
            return;
        }else{
            for (int i=0; i<=5; ++i)
            {
                if (player1_cards[i].first>player2_cards[i].first)
                {
                    sendLooserWinner(0, 1, allsockets);
                    return;
                }else if (player1_cards[i].first<player2_cards[i].first)
                {
                    sendLooserWinner(1, 0, allsockets);
                    return;
                }else{
                    qDebug()<<"the hands are exactly the same.";
                }
            }
        }
    }
}


void GameManagement::sendLooserWinner(int WinnerIndex, int LooserIndex, QList<QTcpSocket*> allsockets)
{
    QString todayString = QDate::currentDate().toString("yyyy-MM-dd");
    allsockets[WinnerIndex]->write("\\WINNER\\");
    allsockets[LooserIndex]->write("\\LOOSER\\");
    allsockets[WinnerIndex]->flush();
    allsockets[LooserIndex]->flush();
    //setting opponent name:
    GameResults[allsockets[WinnerIndex]->objectName()].OpponentName = allsockets[LooserIndex]->objectName();
    GameResults[allsockets[LooserIndex]->objectName()].OpponentName = allsockets[WinnerIndex]->objectName();

    //setting date:
    GameResults[allsockets[WinnerIndex]->objectName()].Date = todayString;
    GameResults[allsockets[LooserIndex]->objectName()].Date = todayString;

    if (GameResults[allsockets[WinnerIndex]->objectName()].result1.isEmpty()  && GameResults[allsockets[LooserIndex]->objectName()].result1.isEmpty())
    {
        GameResults[allsockets[WinnerIndex]->objectName()].result1 = "Winner";
        GameResults[allsockets[LooserIndex]->objectName()].result1 = "Looser";

    }else if (GameResults[allsockets[WinnerIndex]->objectName()].result2.isEmpty()  && GameResults[allsockets[LooserIndex]->objectName()].result2.isEmpty())
    {
        GameResults[allsockets[WinnerIndex]->objectName()].result2 = "Winner";
        GameResults[allsockets[LooserIndex]->objectName()].result2 = "Looser";

    }else if (GameResults[allsockets[WinnerIndex]->objectName()].result3.isEmpty()  && GameResults[allsockets[LooserIndex]->objectName()].result3.isEmpty())
    {
        GameResults[allsockets[WinnerIndex]->objectName()].result3 = "Winner";
        GameResults[allsockets[LooserIndex]->objectName()].result3 = "Looser";
    }

    GamerHands[allsockets[0]->objectName()].clear();
    GamerHands[allsockets[1]->objectName()].clear();
    CardsInARound.clear();
    CardsInOneHand.clear();
}

void GameManagement::ShowFinalWinner(QList<QTcpSocket*> allsockets)
{
    qDebug()<<"we are in showFinalWinner";
    QVector<QString> client1Results = {GameResults[allsockets[0]->objectName()].result1, GameResults[allsockets[0]->objectName()].result2, GameResults[allsockets[0]->objectName()].result3};
    QVector<QString> client2LooserResults = {GameResults[allsockets[1]->objectName()].result1, GameResults[allsockets[1]->objectName()].result2, GameResults[allsockets[1]->objectName()].result3};
    int client1totalCount = 0;
    int client2totalCount = 0;
    for (const QString& result : client1Results) {
        if (result == "Winner") {
            client1totalCount++;
        }
    }

    for (const QString& result : client2LooserResults) {
        if (result == "Winner") {
            client2totalCount++;
        }
    }

    if(client1totalCount>=2)
    {
        GameResults[allsockets[0]->objectName()].FinalResult="Winner";
        GameResults[allsockets[1]->objectName()].FinalResult="Looser";
        allsockets[0]->write("\\FINALWINNER\\");
        allsockets[1]->write("\\FINALLOOSER\\");
        qDebug()<<"the final winner: "+allsockets[0]->objectName();
        qDebug()<<"the final looser: "+allsockets[1]->objectName();
        allsockets[0]->flush();
        allsockets[1]->flush();
        SavingFinalResultsToHistory();
    }else if (client2totalCount >= 2)
    {
        GameResults[allsockets[1]->objectName()].FinalResult="Winner";
        GameResults[allsockets[0]->objectName()].FinalResult="Looser";

        allsockets[0]->write("\\FINALLOOSER\\");
        allsockets[1]->write("\\FINALWINNER\\");
        qDebug()<<"the final winner: "+allsockets[1]->objectName();
        qDebug()<<"the final looser: "+allsockets[0]->objectName();
        allsockets[0]->flush();
        allsockets[1]->flush();
        SavingFinalResultsToHistory();
    }else{
        qDebug()<<"no final result";
    }


}


void GameManagement::SavingFinalResultsToHistory()
{
    QString filePath = "game_results.dat";
    QFile file(filePath);
    if (!file.open(QIODevice::Append)) {
        qWarning() << " Cannot open file for writing:" << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);

    for (auto it = GameResults.begin(); it != GameResults.end(); ++it) {
        const QString& key = it.key();
        const UserHistory& h = it.value();

        out << key;
        out << h.OpponentName;
        out << h.Date;
        out << h.result1;
        out << h.result2;
        out << h.result3;
        out << h.FinalResult;
    }

    file.close();
    qDebug() << " GameResults saved successfully to" << filePath;
}


void GameManagement::ShowHistory(QTcpSocket* _socket)
{
    QFile file("game_results.dat");
    QString username = _socket->objectName();
    QString allHistories = "\\HISTORY\\,";

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file for reading:" << file.errorString();
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);

    int matchCount = 0;

    while (!in.atEnd()) {
        QString currentKey;
        UserHistory h;

        in >> currentKey;
        in >> h.OpponentName;
        in >> h.Date;
        in >> h.result1;
        in >> h.result2;
        in >> h.result3;
        in >> h.FinalResult;

        if (in.status() != QDataStream::Ok) {
            qWarning() << "Data stream error while reading.";
            break;
        }

        if (currentKey == username) {
            ++matchCount;
            allHistories += "#" + QString::number(matchCount);
            allHistories += ",Opponent:" + h.OpponentName;
            allHistories += ",Date:" + h.Date;
            allHistories += ",Round1:" + h.result1;
            allHistories += ",Round2:" + h.result2;
            allHistories += ",Round3:" + h.result3;
            allHistories += ",FinalResult:" + h.FinalResult;
            allHistories += "|";
        }
    }

    qDebug()<<"all matches :"+allHistories;

    file.close();

    if (matchCount == 0) {
        qWarning() << "No records found for key:" << username;
    } else {
        qDebug() << "Total records found for" << username << ":" << matchCount;
        _socket->write(allHistories.toUtf8());
        _socket->flush();
    }

}

void GameManagement::IfAllWasTheSame(QList<QTcpSocket*> allsockets)
{
    QVector<QPair<int, int>> player1_cards = GamerHands[allsockets[0]->objectName()];
    QVector<QPair<int, int>> player2_cards = GamerHands[allsockets[1]->objectName()];

    for (int i=0; i<=5; ++i)
    {
        if (player1_cards[i].first>player2_cards[i].first)
        {
            sendLooserWinner(0, 1, allsockets);
            return;

        }else if (player1_cards[i].first<player2_cards[i].first)
        {
            sendLooserWinner(1, 0, allsockets);
            return;
        }
    }
}





