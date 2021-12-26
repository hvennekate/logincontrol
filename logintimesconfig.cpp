#include "logintimesconfig.h"
#include "weeklytimerange.h"
#include <QDebug>
#include <algorithm>

LoginTimesConfig::LoginTimesConfig()
{}

LoginTimesConfig::LoginTimesConfig(const QString &user, QList<TimeRange *> ranges)
  : userName(user),
    timeRanges(ranges)
{}

LoginTimesConfig::LoginTimesConfig(LoginTimesConfig &&other)
  : userName(other.userName)
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

bool LoginTimesConfig::allowLogin(const QDateTime &dateTime)
{
  auto allowingRangeIterator =
      std::find_if(timeRanges.cbegin(), timeRanges.cend(),
                   [&](TimeRange *r){return r->contains(dateTime);});
  return timeRanges.cend() != allowingRangeIterator;
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
  QList<TimeRange*> timeRanges;
  for (auto line : lines.mid(1)) {
    auto timeRange = parseLine(line);
    if (timeRange) timeRanges << timeRange;
    else qInfo() << "Line not a valid time range:" << line;
  }
  return LoginTimesConfig(user, timeRanges);
}
