#include "forgot_password.h"
#include "ui_forgot_password.h"
#include "mainwindow.h"
#include "game_menu.h"
#include "signin.h"

forgot_password::forgot_password(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::forgot_password)
{
    ui->setupUi(this);

    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &forgot_password::onServerMessage,
                Qt::UniqueConnection);
    }
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


    QString receivedData = MainWindow::lastReceivedMessage;
    qDebug()<< receivedData;

}

void forgot_password::onServerMessage(const QString& msg){
    qDebug() << "Received in forgot_password:" << msg;
    if (msg == "\\OK_CHANGED\\")
    {
        ui->status->setText("The password changed successfully!");
        signin* signin_pg = new signin();
        signin_pg->show();
        this->close();

    }else{
        ui->status->setText("The username and phone_number are not related together!");
    }
    // this->close();
}


