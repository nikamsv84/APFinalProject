#include "game_page.h"
#include "ui_game_page.h"
#include "mainwindow.h"

game_page::game_page(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::game_page)
{
    ui->setupUi(this);

    // وصل می‌شویم به سیگنال messageReceived
    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &game_page::onServerMessage,
                Qt::UniqueConnection);
    }
}

game_page::~game_page()
{
    delete ui;
}


void game_page::onServerMessage(const QString& msg){
    qDebug() << "Received in choosingStarter :" << msg;
    if(msg == "\\STARTER\\"){
        ui->mine_status->setText("first player");
        ui->competitor_status->setText("second player");
    }else if(msg == "\\SECOND_PLAYER\\"){
        ui->mine_status->setText("second player");
        ui->competitor_status->setText("first player");
    }
}



