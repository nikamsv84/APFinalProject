#include "editinfo.h"
#include <QList>
#include <QMap>
#include<QDebug>
EditInfo::EditInfo(QString ReceivedData):ReceivedData(ReceivedData){}
void EditInfo::Messagehandeler()
{
    ReceivedData.remove("\\EDITINFO\\,");
    QStringList fields = ReceivedData.split(',');
    QMap<QString, QString> signupInfo;
    for (const QString& field : fields) {
        QStringList keyValue = field.split(':');
        if (keyValue.size() == 2) {
            signupInfo[keyValue[0]] = keyValue[1];
        }
    }

    PreviousData = signupInfo["PreviousData"];
    NewData = signupInfo["NewData"];

    qDebug()<<"PreviousData:>>"+PreviousData;
    qDebug()<<"NewData:>>"+NewData;

}
void EditInfo::EditSpecificField()
{

}
