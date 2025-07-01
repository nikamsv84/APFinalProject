#include "game_menu.h"
#include "ui_game_menu.h"
#include "mainwindow.h"
#include "game_page.h"
#include "recent_games.h"
#include "edit_info.h"

game_menu::game_menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::game_menu)
{
    ui->setupUi(this);

    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &game_menu::onServerMessage,
                Qt::UniqueConnection);
    }
}


game_menu::~game_menu()
{
    delete ui;
}

void game_menu::on_start_game_clicked()
{
    MainWindow::sendData("\\STARTGAME\\");
    ui->status->setText("Waiting for another player...");
}


void game_menu::onServerMessage(const QString& msg)
{
    qDebug() << "Received in start_game :" << msg;

    if (msg == "\\START\\")
    {
        game_page* game_page_main = new game_page();
        game_page_main->show();
        this->close();
    }
    else
    {
        ui->status->setText("There isn't another user to join game!");
    }
}



void game_menu::on_edit_info_clicked()
{
    edit_info* edit_info_pg = new edit_info();
    edit_info_pg->show();
}

void game_menu::on_exit_clicked()
{
    this->close();
}


void game_menu::on_recent_games_clicked()
{
    recent_games* recent_games_page = new recent_games();
    recent_games_page->show();
}

