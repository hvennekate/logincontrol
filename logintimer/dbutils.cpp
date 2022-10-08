#include "dbutils.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

void initializeTable(const QString &name, const QString &creationQuery)
{
  if (!QSqlDatabase::database().tables().contains(name)) {
    qInfo() << "Initializing table" << name << creationQuery;
    QSqlQuery tableCreation{creationQuery};
    if (tableCreation.lastError().isValid()) {
      qWarning() << "Table creation failed for table" << name
                 << "with query" << creationQuery
                 << "Error:" << tableCreation.lastError();
      throw QString("Table %1 could not be initialized!").arg(name);
    }
  }
}
