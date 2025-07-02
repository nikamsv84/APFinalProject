#include "recent_games.h"
#include "ui_recent_games.h"
#include "mainwindow.h"
#include "QString"

recent_games::recent_games(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::recent_games)
{
    ui->setupUi(this);

    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &recent_games::onServerMessage,
                Qt::UniqueConnection);
    }

}

recent_games::~recent_games()
{
    delete ui;
}


void recent_games::onServerMessage(const QString& msg){

    if(msg.contains("\\HISTORY\\")){

        // QString allHistories = "\\HISTORY\\,#1,Opponent:Ali,Date:2025-07-01,Round1:Win,Round2:Lose,Round3:Win,FinalResult:Win|#2,Opponent:Reza,...";

        QString data = msg.section(",", 1);

            QStringList matches = data.split("|", Qt::SkipEmptyParts);

        for (const QString& match : matches) {
            QStringList fields = match.split(",", Qt::SkipEmptyParts);
            QString matchNumber;
            QString opponent;
            QString date;
            QString round1, round2, round3;
            QString finalResult;

            for (const QString& field : fields) {
                if (field.startsWith("#")) {
                    matchNumber = field.mid(1);
                } else if (field.startsWith("Opponent:")) {
                    opponent = field.section(":", 1);
                } else if (field.startsWith("Date:")) {
                    date = field.section(":", 1);
                } else if (field.startsWith("Round1:")) {
                    round1 = field.section(":", 1);
                } else if (field.startsWith("Round2:")) {
                    round2 = field.section(":", 1);
                } else if (field.startsWith("Round3:")) {
                    round3 = field.section(":", 1);
                } else if (field.startsWith("FinalResult:")) {
                    finalResult = field.section(":", 1);
                }
            }

            if(matchNumber == "1"){
                ui->match_number1->setText(matchNumber);
                ui->opponent_1->setText(opponent);
                ui->date_1->setText(date);
                ui->round1_1->setText(round1);
                ui->round2_1->setText(round2);
                ui->round3_1->setText(round3);
                ui->final_result_1->setText(finalResult);
            }else if(matchNumber == "2"){

                ui->match_number1_2->setText(matchNumber);
                ui->opponent_2->setText(opponent);
                ui->date_2->setText(date);
                ui->round1_2->setText(round1);
                ui->round2_2->setText(round2);
                ui->round3_2->setText(round3);
                ui->final_result_2->setText(finalResult);

            }else if(matchNumber == "3"){
                ui->match_number1_3->setText(matchNumber);
                ui->opponent_3->setText(opponent);
                ui->date_3->setText(date);
                ui->round1_3->setText(round1);
                ui->round2_3->setText(round2);
                ui->round3_3->setText(round3);
                ui->final_result_3->setText(finalResult);
            }

        }

    }
}
