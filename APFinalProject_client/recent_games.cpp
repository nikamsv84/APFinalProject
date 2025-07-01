#include "recent_games.h"
#include "ui_recent_games.h"

recent_games::recent_games(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::recent_games)
{
    ui->setupUi(this);
}

recent_games::~recent_games()
{
    delete ui;
}
