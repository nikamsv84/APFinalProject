#include "databasemanager.h"


QMap<QString , QString> DatabaseManager::userInfo;

DatabaseManager::DatabaseManager(QString ReceivedData) : ReceivedData(ReceivedData) {}

void DatabaseManager::Messagehandeler(QString Specifier)
{
    ReceivedData = ReceivedData.trimmed();
    ReceivedData.remove(Specifier + ",");
    qDebug() << "ReceivedData: " << ReceivedData;

    QStringList fields = ReceivedData.split(',');
    userInfo.clear();

    for (const QString& field : fields) {
        QStringList keyValue = field.split(':');
        if (keyValue.size() == 2) {
            userInfo[keyValue[0]] = keyValue[1];
        }
    }
}
