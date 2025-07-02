#include "resume_stop.h"
#include "ui_resume_stop.h"
#include "mainwindow.h"
#include "game_page.h"


resume_stop::resume_stop(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::resume_stop)
{
    ui->setupUi(this);

    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &resume_stop::onServerMessage,
                Qt::UniqueConnection);
    }

    startCountdown();
}

resume_stop::~resume_stop()
{
    delete ui;
}


void resume_stop::startCountdown() {
    static int counter = 20;
    static QTimer *timer = nullptr;

    if (!timer) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            qDebug() << "remained time!:" << counter;
            ui->Timer->setText(QString::number(counter));

            counter--;

            if (counter == 0 && game_page::is_stop_clicked) {
                MainWindow::sendData("\\COMMUNICATE\\,Message:FinishedStop");
                ui->player_winning_status->setText("You failed!");
                timer->stop();
                qDebug() << "finished time!";
                counter = 20;
                return;
            }

            if (counter < 0) {
                timer->stop();
                qDebug() << "finished time!";
                counter = 20;
            }
        });
    }

    if (!timer->isActive()) {
        counter = 20;
        timer->start(1000);
    }
}


void resume_stop::onServerMessage(const QString& msg)
{
    if(msg == "FinishedStop"){
        ui->player_winning_status->setText("You Won!");
    }
    else if(msg == "RESUME"){
        this->close();
    }
}


void resume_stop::on_resume_clicked()
{
    if(game_page::is_stop_clicked){
    game_page::is_stop_clicked = false;
    MainWindow::sendData("\\COMMUNICATE\\,Message:RESUME");
    this->close();
    }
    else{
        ui->resume->setEnabled(false);
    }
}

