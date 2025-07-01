#include "game_page.h"
#include "ui_game_page.h"
#include "mainwindow.h"
#include "resume_stop.h"
#include <QPair>

int game_page::count_of_lates = 0;
int game_page::win_counts = 0;
int game_page::lose_counts = 0;

int game_page::choose_card_each_hand = 0;
 QString game_page::player_turn_status;
 int game_page::hand = 1;
 int game_page::round = 1;
bool game_page::is_stop_clicked = false;
int game_page::stop_count = 0;


  bool game_page::is_card1_clicked = false;
bool game_page::is_card2_clicked = false;
  bool game_page::is_card3_clicked = false;
bool game_page::is_card4_clicked = false;
  bool game_page::is_card5_clicked = false;
bool game_page::is_card6_clicked = false;
  bool game_page::is_card7_clicked = false;

QVector<QString> game_page::player_cards;//player cards in each round
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

    ui->round->setText(QString::number(round));
    ui->player_status_1->setText(MainWindow::username);

}

game_page::~game_page()
{
    delete ui;
}

void game_page::changing_turn_in_hand(){

    if(player_turn_status == "first"){

        player_turn_status = "second";
        ui->mine_status->setText("second player");
        ui->competitor_status->setText("first player");
        ui->Card_request->setEnabled(false);

    }
    else if(player_turn_status == "second"){

        player_turn_status = "first";
        ui->mine_status->setText("first player");
        ui->competitor_status->setText("second player");
        ui->Card_request->setEnabled(true);
    }
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


    //gold cards
    if(degree == 3 && name == 2){path ="gold 2";}
    else if(degree == 3 && name == 3){path ="gold 3";}
    else if(degree == 3 && name == 4){path ="gold 4";}
    else if(degree == 3 && name == 5){path ="gold 5";}
    else if(degree == 3 && name == 6){path ="gold 6";}
    else if(degree == 3 && name == 7){path ="gold 7";}
    else if(degree == 3 && name == 8){path ="gold 8";}
    else if(degree == 3 && name == 9){path ="gold 9";}
    else if(degree == 3 && name == 10){path ="gold 10";}
    else if(degree == 3 && name == 11){path ="gold soldier";}
    else if(degree == 3 && name == 12){path ="gold queen";}
    else if(degree == 3 && name == 13){path ="gold king";}
    else if(degree == 3 && name == 14){path ="gold bitcoin";}

    //dollar cards
    if(degree == 2 && name == 2){path ="dollar 2";}
    else if(degree == 2 && name == 3){path ="dollar 3";}
    else if(degree == 2 && name == 4){path ="dollar 4";}
    else if(degree == 2 && name == 5){path ="dollar 5";}
    else if(degree == 2 && name == 6){path ="dollar 6";}
    else if(degree == 2 && name == 7){path ="dollar 7";}
    else if(degree == 2 && name == 8){path ="dollar 8";}
    else if(degree == 2 && name == 9){path ="dollar 9";}
    else if(degree == 2 && name == 10){path ="dollar 10";}
    else if(degree == 2 && name == 11){path ="dollar soldier";}
    else if(degree == 2 && name == 12){path ="dollar queen";}
    else if(degree == 2 && name == 13){path ="dollar king";}
    else if(degree == 2 && name == 14){path ="dollar bitcoin";}

    //coin cards
    if(degree == 1 && name == 2){path ="coin 2";}
    else if(degree == 1 && name == 3){path ="coin 3";}
    else if(degree == 1 && name == 4){path ="coin 4";}
    else if(degree == 1 && name == 5){path ="coin 5";}
    else if(degree == 1 && name == 6){path ="coin 6";}
    else if(degree == 1 && name == 7){path ="coin 7";}
    else if(degree == 1 && name == 8){path ="coin 8";}
    else if(degree == 1 && name == 9){path ="coin 9";}
    else if(degree == 1 && name == 10){path ="coin 10";}
    else if(degree == 1 && name == 11){path ="coin soldier";}
    else if(degree == 1 && name == 12){path ="coin queen";}
    else if(degree == 1 && name == 13){path ="coin king";}
    else if(degree == 1 && name == 14){path ="coin bitcoin";}


    return path;
}



void game_page::onServerMessage(const QString& msg){
    qDebug() << "Message in game_page:" << msg;

    if(msg == "\\STARTER\\"){

        ui->mine_status->setText("first player");
        player_turn_status = "first";
        ui->competitor_status->setText("second player");
        ui->Card_request->setEnabled(true);

    } else if(msg == "\\SECOND_PLAYER\\"){
        ui->mine_status->setText("second player");
        player_turn_status = "second";
        ui->competitor_status->setText("first player");
        ui->Card_request->setEnabled(false);
    }
    else if(msg == "STOP"){
        qDebug() << "communicate : " << msg ;
        resume_stop* resume_stop_pg = new resume_stop();
        resume_stop_pg->show();
    }
    else if(msg == "EXIT"){
        this->close();
    }
    else if(msg == "\\WINNER\\"){
        win_counts++;
        ui->your_wins->setText(QString::number(win_counts));

    }else if(msg == "\\LOSER\\"){
        lose_counts++;
        ui->competitor_wins->setText(QString::number(lose_counts));
    }
    else if(msg == "\\NO_CHANGING\\"){
        ui->your_changed_card->setText("NO CHANGING!");
        ui->change_cards->setEnabled(true);
        ui->ok_changing->setEnabled(true);
    }
    else if(msg == "\\OK_CHANGING\\"){

        ui->change_cards->setEnabled(true);
        ui->ok_changing->setEnabled(true);

        if(ui->your_changed_card->text() == "1"){
            QString text = ui->competitor_change_card->text();
            ui->mine_card1->setText(text);
            ui->your_changed_card->setText("");
            ui->competitor_change_card->setText("");
        }
        else if(ui->your_changed_card->text() == "2"){
            QString text = ui->competitor_change_card->text();
            ui->mine_card2->setText(text);
            ui->your_changed_card->setText("");
            ui->competitor_change_card->setText("");
        }
        else if(ui->your_changed_card->text() == "3"){
            QString text = ui->competitor_change_card->text();
            ui->mine_card3->setText(text);
            ui->your_changed_card->setText("");
            ui->competitor_change_card->setText("");
        }
        else if(ui->your_changed_card->text() == "4"){
            QString text = ui->competitor_change_card->text();
            ui->mine_card4->setText(text);
            ui->your_changed_card->setText("");
            ui->competitor_change_card->setText("");
        }
        else if(ui->your_changed_card->text() == "5"){
            QString text = ui->competitor_change_card->text();
            ui->mine_card5->setText(text);
            ui->your_changed_card->setText("");
            ui->competitor_change_card->setText("");
        }

    }
    else if(msg.contains("\\CHANGED_CARD\\")){

        if (msg.startsWith("\\CHANGED_CARD\\")) {
            QString content = msg.mid(QString("\\CHANGED_CARD\\").length());

            QStringList parts = content.split(' ');

            if (parts.size() == 2) {
                QString partOne = parts[0];
                QString partTwo = parts[1];

                ui->competitor_change_card->setText(partOne + " " + partTwo); // Correct

                qDebug() << "Part One:" << partOne;
                qDebug() << "Part Two:" << partTwo;
            } else {
                qDebug() << "The format is not correct!";
            }
        } else {
            qDebug() << "It's not started with \\CHANGED\\!";
        }

    }
    else if(msg == "card_1"){
            ui->card_1->hide();
            ui->card_1->setEnabled(false);

        }else if(msg == "card_2"){
            ui->card_2->hide();
            ui->card_2->setEnabled(false);


        }else if(msg== "card_3"){
            ui->card_3->hide();
            ui->card_3->setEnabled(false);


        }else if(msg == "card_4"){
            ui->card_4->hide();
            ui->card_4->setEnabled(false);

        }else if(msg == "card_5"){
            ui->card_5->hide();
            ui->card_5->setEnabled(false);

        }else if(msg == "card_6"){
            ui->card_6->hide();
            ui->card_6->setEnabled(false);

        }else if(msg == "card_7"){
            ui->card_7->hide();
            ui->card_7->setEnabled(false);

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
                    ui->card_1->setText(path_c1);
                    ui->card_1->setEnabled(true);
                    is_card1_clicked = false;
                    ui->card_1->show();

                    QString path_c2 = manging_card_show(parsedCards[1].first, parsedCards[1].second);
                    ui->card_2->setText(path_c2);
                    ui->card_2->setEnabled(true);
                    is_card2_clicked = false;
                    ui->card_2->show();

                    QString path_c3 = manging_card_show(parsedCards[2].first, parsedCards[2].second);
                    ui->card_3->setText(path_c3);
                    ui->card_3->setEnabled(true);
                    is_card3_clicked = false;
                    ui->card_3->show();

                    QString path_c4 = manging_card_show(parsedCards[3].first, parsedCards[3].second);
                    ui->card_4->setText(path_c4);
                    ui->card_4->setEnabled(true);
                    is_card4_clicked = false;
                    ui->card_4->show();

                    QString path_c5 = manging_card_show(parsedCards[4].first, parsedCards[4].second);
                    ui->card_5->setText(path_c5);
                    ui->card_5->setEnabled(true);
                    is_card5_clicked = false;
                    ui->card_5->show();

                    QString path_c6 = manging_card_show(parsedCards[5].first, parsedCards[5].second);
                    ui->card_6->setText(path_c6);
                    ui->card_6->setEnabled(true);
                    is_card6_clicked = false;
                    ui->card_6->show();

                    QString path_c7 = manging_card_show(parsedCards[6].first, parsedCards[6].second);
                    ui->card_7->setText(path_c7);
                    ui->card_7->setEnabled(true);
                    is_card7_clicked = false;
                    ui->card_7->show();

                }
                   }
}




void game_page::on_Card_request_clicked()
{
    MainWindow::sendData("\\CARDREQUEST\\");

}


void game_page::on_card_1_clicked()
{

    is_card1_clicked = true;
    QString card_data = ui->card_1->text();
    player_cards.append(card_data);

    changing_turn_in_hand();

    if(player_cards.size() == 5){
        player_cards.clear();
        round++;
        ui->round->setText(QString::number(round));
        ui->mine_card1->setText("");
        ui->mine_card2->setText("");
        ui->mine_card3->setText("");
        ui->mine_card4->setText("");
        ui->mine_card5->setText("");
        hand = 0;
    }

    QList<QPushButton*> cards = {
        ui->mine_card1,
        ui->mine_card2,
        ui->mine_card3,
        ui->mine_card4,
        ui->mine_card5
    };

    for (QPushButton* card : cards) {
        if (card->text().trimmed().isEmpty()) {
            card->setText(card_data);
            break;
        }
    }


    int degree = 0;
    int name = 0;

    QStringList parts = card_data.split(' ');
    if (parts.size() == 2) {
        QString _degree = parts[0];
        QString _name = parts[1];

        if (_degree == "diamond") degree = 4;
        else if (_degree == "gold") degree = 3;
        else if (_degree == "dollar") degree = 2;
        else if (_degree == "coin") degree = 1;

        if(_name == "queen"){
            name = 12;
        }
        else if(_name == "king"){
            name = 13;
        }else if(_name == "soldier"){
            name = 11;
        }else if(_name == "bitcoin"){
            name = 14;
        }
        else{
        name = _name.toInt();
        }
    }


    MainWindow::sendData(QString("\\CHOOSE\\,") + "Degree:" + QString::number(degree) + "," + "Name:" + QString::number(name));
    ui->card_1->hide();
    ui->card_1->setEnabled(false);
}


void game_page::on_card_2_clicked()
{
    is_card2_clicked = true;
    QString card_data = ui->card_2->text();
    player_cards.append(card_data);

    changing_turn_in_hand();

    if(player_cards.size() == 5){
        player_cards.clear();
        round++;
        ui->round->setText(QString::number(round));
        ui->mine_card1->setText("");
        ui->mine_card2->setText("");
        ui->mine_card3->setText("");
        ui->mine_card4->setText("");
        ui->mine_card5->setText("");
        hand = 0;
    }

    QList<QPushButton*> cards = {
        ui->mine_card1,
        ui->mine_card2,
        ui->mine_card3,
        ui->mine_card4,
        ui->mine_card5
    };

    for (QPushButton* card : cards) {
        if (card->text().trimmed().isEmpty()) {
            card->setText(card_data);
            break;
        }
    }


    int degree = 0;
    int name = 0;

    QStringList parts = card_data.split(' ');
    if (parts.size() == 2) {
        QString _degree = parts[0];
        QString _name = parts[1];

        if (_degree == "diamond") degree = 4;
        else if (_degree == "gold") degree = 3;
        else if (_degree == "dollar") degree = 2;
        else if (_degree == "coin") degree = 1;

        if(_name == "queen"){
            name = 12;
        }
        else if(_name == "king"){
            name = 13;
        }else if(_name == "soldier"){
            name = 11;
        }else if(_name == "bitcoin"){
            name = 14;
        }
        else{
            name = _name.toInt();
        }
    }

    MainWindow::sendData(QString("\\CHOOSE\\,") + "Degree:" + QString::number(degree) + "," + "Name:" + QString::number(name));
    ui->card_2->hide();
    ui->card_2->setEnabled(false);

}


void game_page::on_card_3_clicked()
{
    is_card3_clicked = true;
    QString card_data = ui->card_3->text();
    player_cards.append(card_data);

    changing_turn_in_hand();

    if(player_cards.size() == 5){
        player_cards.clear();
        round++;
        ui->round->setText(QString::number(round));
        ui->mine_card1->setText("");
        ui->mine_card2->setText("");
        ui->mine_card3->setText("");
        ui->mine_card4->setText("");
        ui->mine_card5->setText("");
        hand = 0;
    }

    QList<QPushButton*> cards = {
        ui->mine_card1,
        ui->mine_card2,
        ui->mine_card3,
        ui->mine_card4,
        ui->mine_card5
    };

    for (QPushButton* card : cards) {
        if (card->text().trimmed().isEmpty()) {
            card->setText(card_data);
            break;
        }
    }

    int degree = 0;
    int name = 0;

    QStringList parts = card_data.split(' ');
    if (parts.size() == 2) {
        QString _degree = parts[0];
        QString _name = parts[1];

        if (_degree == "diamond") degree = 4;
        else if (_degree == "gold") degree = 3;
        else if (_degree == "dollar") degree = 2;
        else if (_degree == "coin") degree = 1;

        if(_name == "queen"){
            name = 12;
        }
        else if(_name == "king"){
            name = 13;
        }else if(_name == "soldier"){
            name = 11;
        }else if(_name == "bitcoin"){
            name = 14;
        }
        else{
            name = _name.toInt();
        }
    }

    MainWindow::sendData(QString("\\CHOOSE\\,") + "Degree:" + QString::number(degree) + "," + "Name:" + QString::number(name));
    ui->card_3->hide();
    ui->card_3->setEnabled(false);

}


void game_page::on_card_4_clicked()
{
    is_card4_clicked = true;
    QString card_data = ui->card_4->text();
    player_cards.append(card_data);

    changing_turn_in_hand();

    if(player_cards.size() == 5){
        player_cards.clear();
        round++;
        ui->round->setText(QString::number(round));
        ui->mine_card1->setText("");
        ui->mine_card2->setText("");
        ui->mine_card3->setText("");
        ui->mine_card4->setText("");
        ui->mine_card5->setText("");
        hand = 0;
    }

    QList<QPushButton*> cards = {
        ui->mine_card1,
        ui->mine_card2,
        ui->mine_card3,
        ui->mine_card4,
        ui->mine_card5
    };

    for (QPushButton* card : cards) {
        if (card->text().trimmed().isEmpty()) {
            card->setText(card_data);
            break;
        }
    }


    int degree = 0;
    int name = 0;

    QStringList parts = card_data.split(' ');
    if (parts.size() == 2) {
        QString _degree = parts[0];
        QString _name = parts[1];

        if (_degree == "diamond") degree = 4;
        else if (_degree == "gold") degree = 3;
        else if (_degree == "dollar") degree = 2;
        else if (_degree == "coin") degree = 1;

        if(_name == "queen"){
            name = 12;
        }
        else if(_name == "king"){
            name = 13;
        }else if(_name == "soldier"){
            name = 11;
        }else if(_name == "bitcoin"){
            name = 14;
        }
        else{
            name = _name.toInt();
        }
    }


    MainWindow::sendData(QString("\\CHOOSE\\,") + "Degree:" + QString::number(degree) + "," + "Name:" + QString::number(name));
    ui->card_4->hide();
    ui->card_4->setEnabled(false);

}


void game_page::on_card_5_clicked()
{
    is_card5_clicked = true;
    QString card_data = ui->card_5->text();
    player_cards.append(card_data);

    changing_turn_in_hand();

    if(player_cards.size() == 5){
        player_cards.clear();
        round++;
        ui->round->setText(QString::number(round));
        ui->mine_card1->setText("");
        ui->mine_card2->setText("");
        ui->mine_card3->setText("");
        ui->mine_card4->setText("");
        ui->mine_card5->setText("");
        hand = 0;
    }

    QList<QPushButton*> cards = {
        ui->mine_card1,
        ui->mine_card2,
        ui->mine_card3,
        ui->mine_card4,
        ui->mine_card5
    };

    for (QPushButton* card : cards) {
        if (card->text().trimmed().isEmpty()) {
            card->setText(card_data);
            break;
        }
    }


    int degree = 0;
    int name = 0;

    QStringList parts = card_data.split(' ');
    if (parts.size() == 2) {
        QString _degree = parts[0];
        QString _name = parts[1];

        if (_degree == "diamond") degree = 4;
        else if (_degree == "gold") degree = 3;
        else if (_degree == "dollar") degree = 2;
        else if (_degree == "coin") degree = 1;

        if(_name == "queen"){
            name = 12;
        }
        else if(_name == "king"){
            name = 13;
        }else if(_name == "soldier"){
            name = 11;
        }else if(_name == "bitcoin"){
            name = 14;
        }
        else{
            name = _name.toInt();
        }
    }

    MainWindow::sendData(QString("\\CHOOSE\\,") + "Degree:" + QString::number(degree) + "," + "Name:" + QString::number(name));
    ui->card_5->hide();
    ui->card_5->setEnabled(false);

}


void game_page::on_card_6_clicked()
{
    is_card6_clicked = true;
    QString card_data = ui->card_6->text();
    player_cards.append(card_data);

    changing_turn_in_hand();

    if(player_cards.size() == 5){
        player_cards.clear();
        round++;
        ui->round->setText(QString::number(round));
        ui->mine_card1->setText("");
        ui->mine_card2->setText("");
        ui->mine_card3->setText("");
        ui->mine_card4->setText("");
        ui->mine_card5->setText("");
        hand = 0;
    }

    QList<QPushButton*> cards = {
        ui->mine_card1,
        ui->mine_card2,
        ui->mine_card3,
        ui->mine_card4,
        ui->mine_card5
    };

    for (QPushButton* card : cards) {
        if (card->text().trimmed().isEmpty()) {
            card->setText(card_data);
            break;
        }
    }

    int degree = 0;
    int name = 0;

    QStringList parts = card_data.split(' ');
    if (parts.size() == 2) {
        QString _degree = parts[0];
        QString _name = parts[1];

        if (_degree == "diamond") degree = 4;
        else if (_degree == "gold") degree = 3;
        else if (_degree == "dollar") degree = 2;
        else if (_degree == "coin") degree = 1;

        if(_name == "queen"){
            name = 12;
        }
        else if(_name == "king"){
            name = 13;
        }else if(_name == "soldier"){
            name = 11;
        }else if(_name == "bitcoin"){
            name = 14;
        }
        else{
            name = _name.toInt();
        }
    }


    MainWindow::sendData(QString("\\CHOOSE\\,") + "Degree:" + QString::number(degree) + "," + "Name:" + QString::number(name));
    ui->card_6->hide();
    ui->card_6->setEnabled(false);

}


void game_page::on_card_7_clicked()
{
    is_card7_clicked = true;
    QString card_data = ui->card_7->text();
    player_cards.append(card_data);

    changing_turn_in_hand();

    if(player_cards.size() == 5){
        player_cards.clear();
        round++;
        ui->round->setText(QString::number(round));
        ui->mine_card1->setText("");
        ui->mine_card2->setText("");
        ui->mine_card3->setText("");
        ui->mine_card4->setText("");
        ui->mine_card5->setText("");
        hand = 0;
    }

    QList<QPushButton*> cards = {
        ui->mine_card1,
        ui->mine_card2,
        ui->mine_card3,
        ui->mine_card4,
        ui->mine_card5
    };

    for (QPushButton* card : cards) {
        if (card->text().trimmed().isEmpty()) {
            card->setText(card_data);
            break;
        }
    }

    int degree = 0;
    int name = 0;

    QStringList parts = card_data.split(' ');
    if (parts.size() == 2) {
        QString _degree = parts[0];
        QString _name = parts[1];

        if (_degree == "diamond") degree = 4;
        else if (_degree == "gold") degree = 3;
        else if (_degree == "dollar") degree = 2;
        else if (_degree == "coin") degree = 1;

        if(_name == "queen"){
            name = 12;
        }
        else if(_name == "king"){
            name = 13;
        }else if(_name == "soldier"){
            name = 11;
        }else if(_name == "bitcoin"){
            name = 14;
        }
        else{
            name = _name.toInt();
        }
    }

MainWindow::sendData(QString("\\CHOOSE\\,") + "Degree:" + QString::number(degree) + "," + "Name:" + QString::number(name));
    ui->card_7->hide();
    ui->card_7->setEnabled(false);

}



void game_page::on_send_changes_clicked()
{
    if(game_page::is_card1_clicked){
        MainWindow::sendData("\\COMMUNICATE\\,Message:card_1");
        is_card1_clicked = false;

    }else if(game_page::is_card2_clicked){
        MainWindow::sendData("\\COMMUNICATE\\,Message:card_2");
        is_card2_clicked = false;


    }else if(game_page::is_card3_clicked){
        MainWindow::sendData("\\COMMUNICATE\\,Message:card_3");
        is_card3_clicked = false;


    }else if(game_page::is_card4_clicked){
        MainWindow::sendData("\\COMMUNICATE\\,Message:card_4");
        is_card4_clicked = false;


    }else if(game_page::is_card5_clicked){
        MainWindow::sendData("\\COMMUNICATE\\,Message:card_5");
        is_card5_clicked = false;


    }else if(game_page::is_card6_clicked){
        MainWindow::sendData("\\COMMUNICATE\\,Message:card_6");
        is_card6_clicked = false;


    }else if(game_page::is_card7_clicked){
        MainWindow::sendData("\\COMMUNICATE\\,Message:card_7");
        is_card7_clicked = false;


    }

}



void game_page::on_exit_clicked()
{
    MainWindow::sendData("\\COMMUNICATE\\,Message:EXIT");
    this->close();
}


void game_page::on_stop_resume_clicked()
{
    if(stop_count == 2){
        ui->stop_resume->setEnabled(false);
    }
    else{
        MainWindow::sendData("\\COMMUNICATE\\,Message:STOP");
        stop_count++;
        is_stop_clicked  = true;
    resume_stop* resume_stop_pg = new resume_stop();
    resume_stop_pg->show();
    }
}




void game_page::on_show_result_of_round_clicked()
{
    MainWindow::sendData("\\SHOWROUNDWINNER\\");
}


void game_page::on_change_card_request_clicked()
{
    if(ui->your_changed_card->text() == "1")
    {
        MainWindow::sendData(QString("\\COMMUNICATE\\,Message:") + "\\CHANGED_CARD\\" + ui->mine_card1->text());
    }
    else if(ui->your_changed_card->text() == "2")
    {
        MainWindow::sendData(QString("\\COMMUNICATE\\,Message:") + "\\CHANGED_CARD\\" + ui->mine_card2->text());
    }
    else if(ui->your_changed_card->text() == "3")
    {
        MainWindow::sendData(QString("\\COMMUNICATE\\,Message:") + "\\CHANGED_CARD\\" + ui->mine_card3->text());
    }
    else if(ui->your_changed_card->text() == "4")
    {
        MainWindow::sendData(QString("\\COMMUNICATE\\,Message:") + "\\CHANGED_CARD\\" + ui->mine_card4->text());
    }
    else if(ui->your_changed_card->text() == "5")
    {
        MainWindow::sendData(QString("\\COMMUNICATE\\,Message:") + "\\CHANGED_CARD\\" + ui->mine_card5->text());
    }

    if(ui->your_changed_card->text() != "" && ui->your_changed_card->text() != "NO CHANGING!"
        && ui->competitor_change_card->text() != ""){
        ui->change_cards->setEnabled(false);
        ui->ok_changing->setEnabled(false);
    }

}


void game_page::on_change_cards_clicked()
{
    QString card_data1 = ui->your_changed_card->text();
    int degree1 = 0;
    int name1 = 0;

    QStringList parts1 = card_data1.split(' ');
    if (parts1.size() == 2) {
        QString _degree1 = parts1[0];
        QString _name1 = parts1[1];

        if (_degree1 == "diamond") degree1 = 4;
        else if (_degree1 == "gold") degree1 = 3;
        else if (_degree1 == "dollar") degree1 = 2;
        else if (_degree1 == "coin") degree1 = 1;

        if(_name1 == "queen"){
            name1 = 12;
        }
        else if(_name1 == "king"){
            name1 = 13;
        }else if(_name1 == "soldier"){
            name1 = 11;
        }else if(_name1 == "bitcoin"){
            name1 = 14;
        }
        else{
            name1 = _name1.toInt();
        }
    }

    QString card_data2 = ui->competitor_change_card->text();
    int degree2 = 0;
    int name2 = 0;

    QStringList parts2 = card_data2.split(' ');
    if (parts2.size() == 2) {
        QString _degree2 = parts2[0];
        QString _name2 = parts2[1];

        if (_degree2 == "diamond") degree2 = 4;
        else if (_degree2 == "gold") degree2 = 3;
        else if (_degree2 == "dollar") degree2 = 2;
        else if (_degree2 == "coin") degree2 = 1;

        if(_name2 == "queen"){
            name2 = 12;
        }
        else if(_name2 == "king"){
            name2 = 13;
        }else if(_name2 == "soldier"){
            name2 = 11;
        }else if(_name2 == "bitcoin"){
            name2 = 14;
        }
        else{
            name2 = _name2.toInt();
        }
    }

    MainWindow::sendData(QString("\\CHANGE_CARD\\,") + "Degree1:" + QString::number(degree1) + "," + "Name1:" + QString::number(name1)+"Degree2:" + QString::number(degree2) + "," + "Name2:" + QString::number(name2));

    if(ui->your_changed_card->text() == "1"){
        QString text = ui->competitor_change_card->text();
        ui->mine_card1->setText(text);
        ui->your_changed_card->setText("");
        ui->competitor_change_card->setText("");
    }
    else if(ui->your_changed_card->text() == "2"){
        QString text = ui->competitor_change_card->text();
        ui->mine_card2->setText(text);
        ui->your_changed_card->setText("");
        ui->competitor_change_card->setText("");
    }
    else if(ui->your_changed_card->text() == "3"){
        QString text = ui->competitor_change_card->text();
        ui->mine_card3->setText(text);
        ui->your_changed_card->setText("");
        ui->competitor_change_card->setText("");
    }
    else if(ui->your_changed_card->text() == "4"){
        QString text = ui->competitor_change_card->text();
        ui->mine_card4->setText(text);
        ui->your_changed_card->setText("");
        ui->competitor_change_card->setText("");
    }
    else if(ui->your_changed_card->text() == "5"){
        QString text = ui->competitor_change_card->text();
        ui->mine_card5->setText(text);
        ui->your_changed_card->setText("");
        ui->competitor_change_card->setText("");
    }

}




void game_page::on_donot_change_clicked()
{
    MainWindow::sendData(QString("\\COMMUNICATE\\,Message:") + "\\NO_CHANGING\\");
}


void game_page::on_ok_changing_clicked()
{
    MainWindow::sendData(QString("\\COMMUNICATE\\,Message:") + "\\OK_CHANGING\\");
}

