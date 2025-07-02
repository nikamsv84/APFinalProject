#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class signup; }
QT_END_NAMESPACE

class signup : public QWidget
{
    Q_OBJECT
public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();

private slots:
    void on_signup_2_clicked();
    void onServerMessage(const QString& msg);

private:
    Ui::signup *ui;
};

#endif // SIGNUP_H
