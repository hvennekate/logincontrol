#include "weeklytimerange.h"
#include <QDebug>
#include <QtDebug>

WeeklyTimeRange::WeeklyTimeRange() {}

WeeklyTimeRange::WeeklyTimeRange(const Qt::DayOfWeek &dow, const QTime &begin, const QTime &end)
  : dayOfWeek(dow),
    start(begin),
    end(end)
{}

bool WeeklyTimeRange::isValid() const {
  return start.isValid()
      && end.isValid()
      && start < end
      && dayOfWeek >= 1 && dayOfWeek <= 7;
}

quint32 WeeklyTimeRange::remainingSecs(const QDateTime &dateTime) const {
  qDebug() << "testing" << dateTime << dayOfWeek << start << end;
  if (dateTime.date().dayOfWeek() != dayOfWeek) return 0;
  auto time = dateTime.time();
  if (time < start) return 0;
  return qMax(0, time.secsTo(end));
}

WeeklyTimeRange *WeeklyTimeRange::fromString(const QString &input) {
  qDebug() << "parsing line" << input;
  auto parts = input.split(QRegExp("\\s+"));
  if (parts.size() != 3) return nullptr;
  return new WeeklyTimeRange((Qt::DayOfWeek) parts[0].toUInt(),
      QTime::fromString(parts[1], "H:mm"),
      QTime::fromString(parts[2], "H:mm"));
}
