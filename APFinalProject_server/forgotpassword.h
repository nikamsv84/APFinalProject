#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H
#include "databasemanager.h"
#include "QTcpSocket"
#include <QString>
class ForgotPassword:public DatabaseManager
{
    QString PhoneNumber;
    QString NewPassword;
public:
    ForgotPassword(QString ReceivedData);
    void EditForgetPassword(QTcpSocket* _socket);
    void InPlacingToLocalAttributes() override;
};

#endif // FORGOTPASSWORD_H
