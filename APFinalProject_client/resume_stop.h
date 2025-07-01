#ifndef RESUME_STOP_H
#define RESUME_STOP_H

#include <QWidget>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>

namespace Ui {
class resume_stop;
}

class resume_stop : public QWidget
{
    Q_OBJECT

public:
    explicit resume_stop(QWidget *parent = nullptr);
    ~resume_stop();
    void startCountdown();

private slots:
    void onServerMessage(const QString& msg);
    void on_resume_clicked();

private:
    Ui::resume_stop *ui;
};

#endif // RESUME_STOP_H
