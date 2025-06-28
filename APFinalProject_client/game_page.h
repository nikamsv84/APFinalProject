#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <QWidget>

namespace Ui {
class game_page;
}

class game_page : public QWidget
{
    Q_OBJECT

public:
    explicit game_page(QWidget *parent = nullptr);
    ~game_page();

private slots:
    void onServerMessage(const QString& msg);

private:
    Ui::game_page *ui;
};

#endif // GAME_PAGE_H
