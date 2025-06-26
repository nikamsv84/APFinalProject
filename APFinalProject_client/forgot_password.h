#ifndef FORGOT_PASSWORD_H
#define FORGOT_PASSWORD_H

#include <QWidget>

namespace Ui {
class forgot_password;
}

class forgot_password : public QWidget
{
    Q_OBJECT

public:
    explicit forgot_password(QWidget *parent = nullptr);
    ~forgot_password();

private slots:
    void on_pushButton_clicked();

private:
    Ui::forgot_password *ui;
};

#endif // FORGOT_PASSWORD_H
