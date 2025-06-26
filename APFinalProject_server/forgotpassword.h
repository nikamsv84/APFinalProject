#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H
#include <QString>
class ForgotPassword
{
    QString ReceivedData;
    QString PhoneNumber;
    QString NewPassword;
public:
    ForgotPassword(QString ReceivedData);
    void Messagehandeler();
    void EditForgetPassword();
};

#endif // FORGOTPASSWORD_H
