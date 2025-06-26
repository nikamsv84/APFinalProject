#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <QWidget>

namespace Ui {
class game_menu;
}

class game_menu : public QWidget
{
    Q_OBJECT

public:
    explicit game_menu(QWidget *parent = nullptr);
    ~game_menu();

private slots:
    void on_edit_info_clicked();

private:
    Ui::game_menu *ui;
};

#endif // GAME_MENU_H
