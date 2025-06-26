#include "forgot_password.h"
#include "ui_forgot_password.h"
#include "mainwindow.h"

forgot_password::forgot_password(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::forgot_password)
{
    ui->setupUi(this);
}

forgot_password::~forgot_password()
{
    delete ui;
}

void forgot_password::on_pushButton_clicked()
{
    QString hashed_password = MainWindow::hashPassword(ui->new_password->text());

    MainWindow::sendData("\\FORGOTPASSWORD\\,username:" + ui->username->text()
                         + ",new_password:" + hashed_password
                         + ",phonenumber:" + ui->phone_number->text());


    QString receivedData = MainWindow::socket_readyRead();
    qDebug()<< receivedData;

}

