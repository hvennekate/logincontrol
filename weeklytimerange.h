#ifndef WEEKLYTIMERANGE_H
#define WEEKLYTIMERANGE_H

#include <QSet>
#include <QTime>
#include <Qt>
#include <timerange.h>



class WeeklyTimeRange : public TimeRange {
  Qt::DayOfWeek dayOfWeek;
  QTime start, end;
public:
  WeeklyTimeRange();
  WeeklyTimeRange(const Qt::DayOfWeek &dow, const QTime &begin, const QTime &end);
  bool isValid() const override;
  bool contains(const QDateTime &dateTime) const override;
  static WeeklyTimeRange *fromString(const QString &input);
};

#endif // WEEKLYTIMERANGE_H
