#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>

namespace Ui {
class signin;
}

class signin : public QWidget
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = nullptr);
    ~signin();

private slots:
    void on_signin_2_clicked();
    void on_forgot_password_clicked();

    void onServerMessage(const QString& msg);

private:
    Ui::signin *ui;
};

#endif // SIGNIN_H
