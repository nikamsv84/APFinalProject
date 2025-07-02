#ifndef EDIT_INFO_H
#define EDIT_INFO_H

#include <QWidget>

namespace Ui {
class edit_info;
}

class edit_info : public QWidget
{
    Q_OBJECT

public:
    explicit edit_info(QWidget *parent = nullptr);
    ~edit_info();

private slots:
    void on_edit_clicked();
    void onServerMessage(const QString& msg);

private:
    Ui::edit_info *ui;
};

#endif // EDIT_INFO_H
