#include "edit_info.h"
#include "ui_edit_info.h"
#include "mainwindow.h"
#include "game_menu.h"

edit_info::edit_info(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::edit_info)
{
    ui->setupUi(this);

    // وصل می‌شویم به سیگنال messageReceived
    if (MainWindow::instance) {
        connect(MainWindow::instance,
                &MainWindow::messageReceived,
                this,
                &edit_info::onServerMessage,
                Qt::UniqueConnection);
    }
}

edit_info::~edit_info()
{
    delete ui;
}

void edit_info::on_edit_clicked()
{
    QString data_type;
    if(ui->username->isChecked()){
        data_type = "username";
    }else if(ui->email->isChecked()){
        data_type = "email";
    }
    else if(ui->phone_num->isChecked()){
        data_type = "phone";
    }
    else if(ui->name->isChecked()){
        data_type = "name";
    }
    else if(ui->last_name->isChecked()){
        data_type = "lastname";
    }
    else{
        qDebug()<<"Invalid operation selection!";
    }

  MainWindow::sendData("\\EDITINFO\\,Datatype:"+data_type
                     +",PreviousData:"+ui->PreviousData->text()+",NewData:"+ui->NewData->text());


  QString receivedData = MainWindow::lastReceivedMessage;
  qDebug()<< receivedData;

}

void edit_info::onServerMessage(const QString& msg){
    qDebug() << "Received in edit_info:" << msg;
    if (msg == "\\OK_EDITED\\")
    {
        ui->status->setText("The information changed successfully!");
        game_menu* game_menu_pg = new game_menu();
        game_menu_pg->show();
        this->close();

    }else{
        ui->status->setText("The process went wrong!");
    }
    // this->close();
}

