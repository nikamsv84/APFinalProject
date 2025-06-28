#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <QWidget>
#include <QVector>

namespace Ui {
class game_page;
}

class game_page : public QWidget
{
    Q_OBJECT

public:
    static QVector<QPair<int,int>>Cards;
    explicit game_page(QWidget *parent = nullptr);
    ~game_page();
    QString manging_card_show(int degree,int name);

private slots:
    void onServerMessage(const QString& msg);

    void on_Card_request_clicked();

    void on_card_2_clicked();

    void on_card_3_clicked();

    void on_card_6_clicked();

    void on_card_5_clicked();

    void on_card_7_clicked();

    void on_card_4_clicked();

    void on_card_1_clicked();

private:
    Ui::game_page *ui;
};

#endif // GAME_PAGE_H
