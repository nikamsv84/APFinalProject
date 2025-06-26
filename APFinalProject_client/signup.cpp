#include "signup.h"
#include "ui_signup.h"
#include "mainwindow.h"
#include "game_menu.h"


signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    ui->setupUi(this);
}

signup::~signup()
{
    delete ui;
}


void signup::on_signup_2_clicked()
{
    QString hashed_password = MainWindow::hashPassword(ui->password->text());

    MainWindow::sendData("\\SIGNUP\\,name:" + ui->name->text()
                         + ",lastname:" + ui->lastname->text()
                         + ",email:" + ui->email->text()
                         + ",phonenumber:" + ui->phonenumber->text()
                         + ",username:" + ui->username->text()
                         + ",password:" + hashed_password);

    QString receivedData = MainWindow::socket_readyRead();
    // ui->status->setText(receivedData);
    qDebug()<< receivedData;
        // game_menu* game_menu_pg = new game_menu();
        // game_menu_pg->show();

}
