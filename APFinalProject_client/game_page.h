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
    static int hand;
    static int round;
    static bool is_stop_clicked;
    static int stop_count;
    static QVector<QString> player_cards;//player cards in each round
    static QVector<QPair<int,int>>Cards;
    explicit game_page(QWidget *parent = nullptr);
    ~game_page();
    QString manging_card_show(int degree,int name);
    static  bool is_card1_clicked;
    static  bool is_card2_clicked;
    static  bool is_card3_clicked;
    static  bool is_card4_clicked;
    static  bool is_card5_clicked;
    static  bool is_card6_clicked;
    static  bool is_card7_clicked;

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

    void on_exit_clicked();

    void on_stop_resume_clicked();

    void on_send_changes_clicked();

private:
    Ui::game_page *ui;
};

#endif // GAME_PAGE_H
