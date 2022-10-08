#include "sessiontimelogger.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include "dbutils.h"

SessionTimeLogger::SessionTimeLogger()
{
  initializeTable("usage",
                  "create table usage ( "
                  "username varchar(255), "
                  "date date, "
                  "time time, "
                      "FOREIGN KEY (username) REFERENCES users(name))");
}

// log simply every minute
void SessionTimeLogger::log(const QString &username)
{
  QSqlQuery insertQuery;
  insertQuery.prepare("INSERT INTO usage(date, username, time)"
                      "VALUES (:date, :user, :time)");
  auto now = QDateTime::currentDateTime();
  insertQuery.bindValue(":date", { now.date().toString(Qt::ISODate) } );
  insertQuery.bindValue(":user", { username });
  insertQuery.bindValue(":time", { now.time().toString(Qt::ISODate) });
  insertQuery.exec();
  QSqlDatabase::database().commit();
}

// get count of entries today as minutes
unsigned SessionTimeLogger::getMinutesToday(const QString &username) const
{
  QSqlQuery getQuery;
  getQuery.prepare("SELECT count(*)"
                   " FROM usage"
                   " WHERE username = :user"
                   " AND date = :date");
  getQuery.bindValue(":user", { username });
  getQuery.bindValue(":date", { QDate::currentDate().toString(Qt::ISODate) });
  if (!getQuery.exec()) {
    qInfo() << "Query failed:" << getQuery.lastError() << getQuery.lastQuery() << getQuery.boundValues();
    return 0;
  }
  if (!getQuery.next()) {
    qInfo() << "Query returned no results:" << getQuery.lastQuery() << getQuery.size();
    return 0;
  }
  auto minutes = getQuery.value(0).toUInt();
  qDebug() << "Minutes consumed today for user" << username << minutes;
  return minutes;
}
