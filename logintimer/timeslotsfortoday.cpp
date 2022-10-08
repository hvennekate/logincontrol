#include "timeslotsfortoday.h"

#include "dbutils.h"
#include "qmath.h"
#include <QSqlQuery>
#include <QDate>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QTime>

TimeSlotsForToday::TimeSlotsForToday()
{
  initializeTable("day_slot",
                  "create table day_slot ("
                  " day date,"
                  " username varchar(255),"
                  " begin time,"
                  " end time,"
                  "    FOREIGN KEY (username) REFERENCES user(name),"
                  "    check (begin < end),"
                  "    primary key (username, day, begin))");

  initializeTable("weekday_slot",
                  "create table weekday_slot ("
                  " weekday int,"
                  " username varchar(255),"
                  " begin time,"
                  " end time,"
                  "    FOREIGN KEY (username) REFERENCES users(name),"
                  "    CHECK (weekday between 0 and 6),"
                  "    check (begin < end),"
                  "    primary key (username, weekday, begin))");
}

QTime TimeSlotsForToday::getCurrentEndTime(const QString &username) const
{
  QSqlQuery queryForTodaysSlot;
  queryForTodaysSlot.prepare("SELECT max(end), count(end)"
                             " FROM day_slot"
                             " WHERE username = :user"
                             " AND day = :today"
                             " AND begin < :currentTime"
                             " AND end > :currentTime"); // alternative: strftime('%H:%M', 'now', 'localtime')
  queryForTodaysSlot.bindValue(":user", username);
  queryForTodaysSlot.bindValue(":today", QDate::currentDate().toString());
  queryForTodaysSlot.bindValue(":currentTime", QTime::currentTime().toString("hh:mm"));
  if (!queryForTodaysSlot.exec() || !queryForTodaysSlot.first()) {
    qInfo() << "Query for today's timeslot failed:" << queryForTodaysSlot.lastError();
    return QTime();
  }
  qDebug() << "Query:" << queryForTodaysSlot.executedQuery() << queryForTodaysSlot.boundValues();
  if (queryForTodaysSlot.value(1).toInt()) {
    auto endTime = QTime::fromString(queryForTodaysSlot.value(0).toString(), "hh:mm");
    qDebug() << "End time for today's date for user" << username << endTime;
    return endTime;
  }

  QSqlQuery queryForWeekdaySlot;
  queryForWeekdaySlot.prepare("SELECT max(end), count(end)"
                              " FROM weekday_slot"
                              " WHERE username = :user"
                              " AND weekday = :currentWeekday"
                              " AND begin < :currentTime"
                              " AND end > :currentTime"); // alternative: strftime('%H:%M', 'now', 'localtime')
  queryForWeekdaySlot.bindValue(":user", username);
  // SQLite: 0 - Sunday, 1 - Monday...; Qt: 1 - Monday, ..., 7 - Sunday
  queryForWeekdaySlot.bindValue(":currentWeekday", QDate::currentDate().dayOfWeek() % 7);
  queryForWeekdaySlot.bindValue(":currentTime", QTime::currentTime().toString("hh:mm"));
  if (!queryForWeekdaySlot.exec() || !queryForWeekdaySlot.first()) {
    qInfo() << "Query for weekday's time slot failed:" << queryForWeekdaySlot.lastError();
    return QTime();
  }
  qDebug() << "Query:" << queryForWeekdaySlot.executedQuery() << queryForWeekdaySlot.boundValues();
  if (queryForWeekdaySlot.value(1).toInt()) {
    auto endTime = QTime::fromString(queryForWeekdaySlot.value(0).toString(), "hh:mm");
    qDebug() << "End time for today's weekday for user" << username << endTime;
    return endTime;
  }
  qDebug() << "no end time found for user" << username;
  return QTime();
}

unsigned TimeSlotsForToday::remainingMinutes(const QString &username) const
{
  auto endTime = getCurrentEndTime(username);
  if (!endTime.isValid()) return 0;
  return qMax(0, qCeil(QTime::currentTime().secsTo(endTime)/60.));
}
