#include "game_page.h"
#include "ui_game_page.h"
#include "mainwindow.h"


QVector<QPair<int,int>> game_page::Cards;

game_page::game_page(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::game_page)
{
    ui->setupUi(this);

    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &game_page::onServerMessage,
                Qt::UniqueConnection);
    }

    for(int degree = 1; degree <= 4; degree++){
        for(int name = 2; name <= 14; name++){
            Cards.append(qMakePair(degree, name));
        }
    }
}

game_page::~game_page()
{
    delete ui;
}

QString game_page::manging_card_show(int degree,int name){
    QString path;
    //diamond cards
    if(degree == 4 && name == 2){path ="diamond 2";}
    else if(degree == 4 && name == 3){path ="diamond 3";}
    else if(degree == 4 && name == 4){path ="diamond 4";}
    else if(degree == 4 && name == 5){path ="diamond 5";}
    else if(degree == 4 && name == 6){path ="diamond 6";}
    else if(degree == 4 && name == 7){path ="diamond 7";}
    else if(degree == 4 && name == 8){path ="diamond 8";}
    else if(degree == 4 && name == 9){path ="diamond 9";}
    else if(degree == 4 && name == 10){path ="diamond 10";}
    else if(degree == 4 && name == 11){path ="diamond soldier";}
    else if(degree == 4 && name == 12){path ="diamond queen";}
    else if(degree == 4 && name == 13){path ="diamond king";}
    else if(degree == 4 && name == 14){path ="diamond bitcoin";}

    //dollar cards
    if(degree == 3 && name == 2){path ="dollar 2";}
    else if(degree == 3 && name == 3){path ="dollar 3";}
    else if(degree == 3 && name == 4){path ="dollar 4";}
    else if(degree == 3 && name == 5){path ="dollar 5";}
    else if(degree == 3 && name == 6){path ="dollar 6";}
    else if(degree == 3 && name == 7){path ="dollar 7";}
    else if(degree == 3 && name == 8){path ="dollar 8";}
    else if(degree == 3 && name == 9){path ="dollar 9";}
    else if(degree == 3 && name == 10){path ="dollar 10";}
    else if(degree == 3 && name == 11){path ="dollar soldier";}
    else if(degree == 3 && name == 12){path ="dollar queen";}
    else if(degree == 3 && name == 13){path ="dollar king";}
    else if(degree == 3 && name == 14){path ="dollar bitcoin";}

    //coin cards
    if(degree == 2 && name == 2){path ="coin 2";}
    else if(degree == 2 && name == 3){path ="coin 3";}
    else if(degree == 2 && name == 4){path ="coin 4";}
    else if(degree == 2 && name == 5){path ="coin 5";}
    else if(degree == 2 && name == 6){path ="coin 6";}
    else if(degree == 2 && name == 7){path ="coin 7";}
    else if(degree == 2 && name == 8){path ="coin 8";}
    else if(degree == 2 && name == 9){path ="coin 9";}
    else if(degree == 2 && name == 10){path ="coin 10";}
    else if(degree == 2 && name == 11){path ="coin soldier";}
    else if(degree == 2 && name == 12){path ="coin queen";}
    else if(degree == 2 && name == 13){path ="coin king";}
    else if(degree == 2 && name == 14){path ="coin bitcoin";}

    //gold cards
    if(degree == 1 && name == 2){path ="gold 2";}
    else if(degree == 1 && name == 3){path ="gold 3";}
    else if(degree == 1 && name == 4){path ="gold 4";}
    else if(degree == 1 && name == 5){path ="gold 5";}
    else if(degree == 1 && name == 6){path ="gold 6";}
    else if(degree == 1 && name == 7){path ="gold 7";}
    else if(degree == 1 && name == 8){path ="gold 8";}
    else if(degree == 1 && name == 9){path ="gold 9";}
    else if(degree == 1 && name == 10){path ="gold 10";}
    else if(degree == 1 && name == 11){path ="gold soldier";}
    else if(degree == 1 && name == 12){path ="gold queen";}
    else if(degree == 1 && name == 13){path ="gold king";}
    else if(degree == 1 && name == 14){path ="gold bitcoin";}

    return path;
}



void game_page::onServerMessage(const QString& msg){
    qDebug() << "Message in game_page:" << msg;

    if(msg == "\\STARTER\\"){
        ui->mine_status->setText("first player");
        ui->competitor_status->setText("second player");
    } else if(msg == "\\SECOND_PLAYER\\"){
        ui->mine_status->setText("second player");
        ui->competitor_status->setText("first player");
    }
    else{
                QVector<QPair<int, int>> parsedCards;

                QStringList cardList = msg.split("/", Qt::SkipEmptyParts);

                for (const QString& card : cardList) {
                    QStringList parts = card.split(":");
                    if (parts.size() == 2) {
                        bool ok1, ok2;
                        int first = parts[0].toInt(&ok1);
                        int second = parts[1].toInt(&ok2);
                        if (ok1 && ok2) {
                            parsedCards.append(qMakePair(first, second));
                        }
                    }
                }

                if(parsedCards.size() == 7){
                    QString path_c1 = manging_card_show(parsedCards[0].first, parsedCards[0].second);
                    // ui->card_1->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c1));
                    ui->card_1->setText(path_c1);

                    QString path_c2 = manging_card_show(parsedCards[1].first, parsedCards[1].second);
                    // ui->card_2->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c2));
                    ui->card_2->setText(path_c2);

                    QString path_c3 = manging_card_show(parsedCards[2].first, parsedCards[2].second);
                    // ui->card_3->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c3));
                    ui->card_3->setText(path_c3);

                    QString path_c4 = manging_card_show(parsedCards[3].first, parsedCards[3].second);
                    // ui->card_4->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c4));
                    ui->card_4->setText(path_c4);

                    QString path_c5 = manging_card_show(parsedCards[4].first, parsedCards[4].second);
                    // ui->card_5->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c5));
                    ui->card_5->setText(path_c5);

                    QString path_c6 = manging_card_show(parsedCards[5].first, parsedCards[5].second);
                    // ui->card_6->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c6));
                    ui->card_6->setText(path_c6);

                    QString path_c7 = manging_card_show(parsedCards[6].first, parsedCards[6].second);
                    // ui->card_7->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c7));
                    ui->card_7->setText(path_c7);

                }
                else if(parsedCards.size() == 6){
                    QString path_c1 = manging_card_show(parsedCards[0].first, parsedCards[0].second);
                    // ui->card_1->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c1));
                    ui->card_1->setText(path_c1);

                    QString path_c2 = manging_card_show(parsedCards[1].first, parsedCards[1].second);
                    // ui->card_2->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c2));
                    ui->card_2->setText(path_c2);

                    QString path_c3 = manging_card_show(parsedCards[2].first, parsedCards[2].second);
                    // ui->card_3->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c3));
                    ui->card_3->setText(path_c3);

                    QString path_c4 = manging_card_show(parsedCards[3].first, parsedCards[3].second);
                    // ui->card_4->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c4));
                    ui->card_4->setText(path_c4);

                    QString path_c5 = manging_card_show(parsedCards[4].first, parsedCards[4].second);
                    // ui->card_5->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c5));
                    ui->card_5->setText(path_c5);

                    QString path_c6 = manging_card_show(parsedCards[5].first, parsedCards[5].second);
                    // ui->card_6->setStyleSheet(QString("background-image: url(%1); background-repeat: no-repeat; background-position: center;").arg(path_c6));
                    ui->card_6->setText(path_c6);
                }

            }
}



void game_page::on_Card_request_clicked()
{
    MainWindow::sendData("\\CARDREQUEST\\");

}


void game_page::on_card_1_clicked()
{
    QString card_data = ui->card_1->text();
    MainWindow::sendData("\\CHOOSE\\"+card_data);
}


void game_page::on_card_2_clicked()
{
    QString card_data = ui->card_2->text();
    MainWindow::sendData("\\CHOOSE\\"+card_data);
}


void game_page::on_card_3_clicked()
{
    QString card_data = ui->card_3->text();
    MainWindow::sendData("\\CHOOSE\\"+card_data);
}


void game_page::on_card_4_clicked()
{
    QString card_data = ui->card_4->text();
    MainWindow::sendData("\\CHOOSE\\"+card_data);
}


void game_page::on_card_5_clicked()
{
    QString card_data = ui->card_5->text();
    MainWindow::sendData("\\CHOOSE\\"+card_data);
}


void game_page::on_card_6_clicked()
{
    QString card_data = ui->card_6->text();
    MainWindow::sendData("\\CHOOSE\\"+card_data);
}


void game_page::on_card_7_clicked()
{
    QString card_data = ui->card_7->text();
    MainWindow::sendData("\\CHOOSE\\"+card_data);
}

