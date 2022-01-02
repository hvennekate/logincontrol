#include "sessiontimelogger.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

SessionTimeLogger::SessionTimeLogger()
{
  requireDB();
}

void SessionTimeLogger::log(const QString &username, quint32 secs)
{
  auto lastValue = getSecondsToday(username);
  QSqlQuery insertQuery;
  insertQuery.prepare("INSERT INTO sessions(date, user, time, seconds)"
                      "VALUES (:date, :user, :time, :seconds)");
  auto now = QDateTime::currentDateTime();
  insertQuery.bindValue(":date", { now.date().toString(Qt::ISODate) } );
  insertQuery.bindValue(":user", { username });
  insertQuery.bindValue(":time", { now.time().toString(Qt::ISODate) });
  insertQuery.bindValue(":seconds", { secs + lastValue });
  insertQuery.exec();
  QSqlDatabase::database().commit();
}

quint32 SessionTimeLogger::getSecondsToday(const QString &username) const
{
  QSqlQuery getQuery;
  getQuery.prepare("SELECT seconds"
                   " FROM sessions"
                   " WHERE user = :user"
                   "   AND date = :date"
                   " ORDER BY seconds DESC"
                   " LIMIT 1");
  getQuery.setForwardOnly(true);
  getQuery.bindValue(":user", { username });
  getQuery.bindValue(":date", { QDate::currentDate().toString(Qt::ISODate) });
  if (!getQuery.exec()) {
    qInfo() << "Query failed:" << getQuery.lastError();
    return 0;
  }
  if (!getQuery.next()) {
    qInfo() << "Query returned no results:" << getQuery.lastQuery() << getQuery.size();
    return 0;
  }
  return getQuery.value(0).toUInt();
}

void SessionTimeLogger::requireDB()
{
  auto db = QSqlDatabase::addDatabase("QSQLITE3");
  db.setDatabaseName("/var/log/logintimes");
  db.open();
  if (!db.tables().contains("sessions")) {
    QSqlQuery tableCreation("CREATE TABLE sessions(date text, user text, time text, seconds integer)");
    if (tableCreation.lastError().isValid()) {
      qWarning() << tableCreation.lastError();
      throw QString("DB could not be initialized!");
    }
  }
}
