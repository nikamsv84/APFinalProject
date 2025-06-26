#ifndef EDITINFO_H
#define EDITINFO_H
#include <QString>
class EditInfo
{
    QString ReceivedData;
    QString DataType;
    QString PreviousData;
    QString NewData;
public:
    EditInfo(QString ReceivedData);
    void Messagehandeler();
    void EditSpecificField();
};

#endif // EDITINFO_H
