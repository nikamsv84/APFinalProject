#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H
#include "databasemanager.h"
#include <QString>
class ForgotPassword:public DatabaseManager
{
    QString PhoneNumber;
    QString NewPassword;
public:
    ForgotPassword(QString ReceivedData);
    void EditForgetPassword();
    void InPlacingToLocalAttributes() override;
};

#endif // FORGOTPASSWORD_H
