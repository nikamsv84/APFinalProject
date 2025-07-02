#include "signin.h"
#include "ui_signin.h"
#include "game_menu.h"
#include "mainwindow.h"
#include "forgot_password.h"
#include <QDebug>

signin::signin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signin)
{
    ui->setupUi(this);

    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &signin::onServerMessage,
                Qt::UniqueConnection);
    }
}

signin::~signin()
{
    delete ui;
}

void signin::on_signin_2_clicked()
{
    QString hashed_password = MainWindow::hashPassword(ui->password->text());

    MainWindow::username = ui->username->text();

    QString payload = QString("\\LOGIN\\,Username:%1,Password:%2")
                          .arg(ui->username->text())
                          .arg(hashed_password);
    MainWindow::sendData(payload);

}

void signin::on_forgot_password_clicked()
{
    forgot_password* forgot_page = new forgot_password();
    forgot_page->show();
}

void signin::onServerMessage(const QString& msg)
{
    qDebug() << "Received in signin:" << msg;
    if(msg == "\\OKLOGIN\\")
    {
        game_menu* menu = new game_menu();
        menu->show();
        this->close();
    }
    else if (msg == "\\ERRORLOGIN\\"){
        ui->status->setText("incorrect username or password");
    }

}
