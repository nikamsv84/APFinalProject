#ifndef RECENT_GAMES_H
#define RECENT_GAMES_H

#include <QWidget>

namespace Ui {
class recent_games;
}

class recent_games : public QWidget
{
    Q_OBJECT

public:
    explicit recent_games(QWidget *parent = nullptr);
    ~recent_games();

private slots :
    void onServerMessage(const QString& msg);
private:
    Ui::recent_games *ui;
};

#endif // RECENT_GAMES_H
