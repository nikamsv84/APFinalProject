#include "game_menu.h"
#include "ui_game_menu.h"
#include "mainwindow.h"
#include "edit_info.h"

game_menu::game_menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::game_menu)
{
    ui->setupUi(this);
}

game_menu::~game_menu()
{
    delete ui;
}

void game_menu::on_edit_info_clicked()
{
    edit_info* edit_info_pg = new edit_info();
    edit_info_pg->show();
}

