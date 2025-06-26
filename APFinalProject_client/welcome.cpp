#include "welcome.h"
#include "ui_welcome.h"
#include"signin.h"
#include"signup.h"

welcome::welcome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::welcome)
{
    ui->setupUi(this);
}

welcome::~welcome()
{
    delete ui;
}

void welcome::on_signin_clicked()
{
    signin* signinpage = new signin();
    signinpage->show();
}


void welcome::on_signup_clicked()
{
    signup* signuppage = new signup();
    signuppage->show();
}
