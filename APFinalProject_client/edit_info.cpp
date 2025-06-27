#include "edit_info.h"
#include "ui_edit_info.h"
#include "mainwindow.h"

edit_info::edit_info(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::edit_info)
{
    ui->setupUi(this);
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
