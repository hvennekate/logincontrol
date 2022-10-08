#ifndef TIMESLOTSFORTODAY_H
#define TIMESLOTSFORTODAY_H

#include <QTime>



class TimeSlotsForToday
{
public:
  TimeSlotsForToday();
  QTime getCurrentEndTime(const QString &username) const;
  unsigned remainingMinutes(const QString &username) const;
};

#endif // TIMESLOTSFORTODAY_H
