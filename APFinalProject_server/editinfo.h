#ifndef EDITINFO_H
#define EDITINFO_H
#include <QString>
#include "databasemanager.h"
class EditInfo:public DatabaseManager
{
    QString DataType;
    QString PreviousData;
    QString NewData;
public:
    EditInfo(QString ReceivedData);
    void EditSpecificField();
    void InPlacingToLocalAttributes() override;

};

#endif // EDITINFO_H
