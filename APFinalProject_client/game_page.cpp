#include "game_page.h"
#include "ui_game_page.h"

game_page::game_page(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::game_page)
{
    ui->setupUi(this);
}

game_page::~game_page()
{
    delete ui;
}
