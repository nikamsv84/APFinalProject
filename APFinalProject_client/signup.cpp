#include "signup.h"
#include "ui_signup.h"
#include "mainwindow.h"
#include "game_menu.h"
#include <QDebug>

signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    ui->setupUi(this);

    // وصل می‌شویم به سیگنال messageReceived
    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &signup::onServerMessage,
                Qt::UniqueConnection);
    }
}

signup::~signup()
{
    delete ui;
}

void signup::on_signup_2_clicked()
{
    QString hashed_password = MainWindow::hashPassword(ui->password->text());

    QString payload = QString("\\SIGNUP\\,name:%1,lastname:%2,email:%3,"
                              "phonenumber:%4,username:%5,password:%6")
                          .arg(ui->name->text())
                          .arg(ui->lastname->text())
                          .arg(ui->email->text())
                          .arg(ui->phonenumber->text())
                          .arg(ui->username->text())
                          .arg(hashed_password);

    MainWindow::sendData(payload);

}

void signup::onServerMessage(const QString& msg)
{
    qDebug() << "Received in signup:" << msg;
    if (msg == "\\EXISTS\\")
    {
        ui->status->setText("the username is already exists");
    }else{
        game_menu* game_menu_pg = new game_menu();
        game_menu_pg->show();
    }
    // this->close();
}



