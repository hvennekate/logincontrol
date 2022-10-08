#include "sessiontimebudget.h"

#include "dbutils.h"
#include <QSqlQuery>
#include <QDate>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

SessionTimeBudget::SessionTimeBudget()
{
  initializeTable("day_duration",
                  "CREATE TABLE day_duration ( "
                  "day date, "
                  "username varchar(255), "
                  "minutes int, "
                      "FOREIGN KEY (username) REFERENCES user(name), "
                      "CHECK (minutes > 0), "
                      "primary key (username, day))");

  initializeTable("weekday_duration",
                  "CREATE TABLE weekday_duration ( "
                  "weekday int, "
                  "username varchar(255), "
                  "minutes int, "
                      "FOREIGN KEY (username) REFERENCES user(name), "
                      "CHECK (weekday between 0 and 6), "
                      "CHECK (minutes > 0), "
                      "primary key (username, weekday))");
}

unsigned SessionTimeBudget::budgetInMinutes(const QString &username) const
{
  QSqlQuery queryForToday;
  queryForToday.prepare("SELECT minutes"
                        " FROM day_duration"
                        " WHERE username = :user"
                        " AND day = :today");
  queryForToday.bindValue(":user", { username });
  queryForToday.bindValue(":today", { QDate::currentDate().toString(Qt::ISODate) });
  if (!queryForToday.exec()) { // TODO debugging of queries
    qInfo() << "Query for today's budget failed:" << queryForToday.lastError()
            << "\nQuery:" << queryForToday.lastQuery()
            << "\nParameters:" << queryForToday.boundValues();
    return 0;
  }
  if (queryForToday.first()) {
    auto minutes = queryForToday.value(0).toUInt();
    qDebug() << "Minutes on today's date for user" << username << minutes;
    return minutes;
  }

  QSqlQuery queryForWeekday;
  queryForWeekday.prepare("SELECT minutes"
                          " FROM weekday_duration"
                          " WHERE username = :user"
                          " AND weekday = :weekday");
  queryForWeekday.setForwardOnly(true);
  queryForWeekday.bindValue(":user", { username });
  // SQLite: 0 - Sunday, 1 - Monday...; Qt: 1 - Monday, ..., 7 - Sunday
  queryForWeekday.bindValue(":weekday", QDate::currentDate().dayOfWeek() % 7);
  if (!queryForWeekday.exec()) {
    qInfo() << "Query for weekday's budget failed:" << queryForWeekday.lastError();
    return 0;
  }
  if (queryForWeekday.first()) {
    auto minutes = queryForWeekday.value(0).toUInt();
    qDebug() << "Minutes on today's weekday for user" << username << minutes;
    return minutes;
  }
  qDebug() << "no timeslot found for user" << username;
  return 0;
}
