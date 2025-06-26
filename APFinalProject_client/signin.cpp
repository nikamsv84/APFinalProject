#include "signin.h"
#include "ui_signin.h"
#include "game_menu.h"
#include "mainwindow.h"


signin::signin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signin)
{
    ui->setupUi(this);
}

signin::~signin()
{
    delete ui;
}

void signin::on_signin_2_clicked()
{
    QString hashed_password = MainWindow::hashPassword(ui->password->text());
    //verifing the datas with functions written in client project...after that->
    MainWindow::sendData("\\SIGNIN\\,username:"+ui->username->text()
                         +",password:"+hashed_password);//this should be sent hashed.
    // QString receivedData = MainWindow::socket_readyRead();
    // ui->signupStatus->setText(receivedData);

    game_menu* game_menu_pg = new game_menu();
    game_menu_pg->show();
}
