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
