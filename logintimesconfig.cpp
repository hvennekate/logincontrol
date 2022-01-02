#include "logintimesconfig.h"
#include "weeklytimerange.h"
#include <QDebug>
#include <QtGlobal>
#include <algorithm>
#include <numeric>

LoginTimesConfig::LoginTimesConfig()
{}

LoginTimesConfig::LoginTimesConfig(const QString &user, const qint32 maxSecsPerDay, QList<TimeRange *> ranges)
  : userName(user),
    maxSecsPerDay(maxSecsPerDay),
    timeRanges(ranges)
{}

LoginTimesConfig::LoginTimesConfig(LoginTimesConfig &&other)
  : userName(other.userName),
    maxSecsPerDay(other.maxSecsPerDay)
{
  timeRanges.swap(other.timeRanges);
}

LoginTimesConfig::~LoginTimesConfig() {
  for (auto range : timeRanges) delete range;
}

bool LoginTimesConfig::isValid() const
{
  return !userName.isEmpty() && !timeRanges.isEmpty();
}

QString LoginTimesConfig::getUserName() const
{
  return userName;
}

quint32 LoginTimesConfig::secondsLeft(const QDateTime &dateTime, const quint32 &secsToday)
{
  quint32 secsLeftForToday = qMax(0, maxSecsPerDay - (qint32) secsToday);
  if (maxSecsPerDay >= 0 && !secsLeftForToday) return 0;
  quint32 secsFromTimeRanges = std::transform_reduce(timeRanges.cbegin(), timeRanges.cend(), 0,
                                                     [](quint32 a, quint32 b) {return qMax(a,b);},
                                                     [&](TimeRange *r){return r->remainingSecs(dateTime);});
  return qMin(secsFromTimeRanges, secsLeftForToday);
}

TimeRange *parseLine(const QString &line) {
  if (line.startsWith("dow "))
    return WeeklyTimeRange::fromString(line.mid(4));
  return nullptr; // for the time being...
}

LoginTimesConfig LoginTimesConfig::fromLines(const QStringList &lines) {
  qDebug() << "creating config" << lines;
  if (lines.isEmpty()) return LoginTimesConfig();
  auto user = lines.first();
  auto secsPerDay = lines.size() > 1 ? lines[1].toInt() : -1;
  QList<TimeRange*> timeRanges;
  for (auto line : lines.mid(2)) {
    auto timeRange = parseLine(line);
    if (timeRange) timeRanges << timeRange;
    else qInfo() << "Line not a valid time range:" << line;
  }
  return LoginTimesConfig(user, secsPerDay, timeRanges);
}
