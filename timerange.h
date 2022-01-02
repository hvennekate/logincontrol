#ifndef TIMERANGE_H
#define TIMERANGE_H

#include <QDateTime>



class TimeRange
{
public:
  virtual quint32 remainingSecs(const QDateTime &dateTime) const = 0;
  virtual bool isValid() const = 0;
  virtual ~TimeRange();
};

#endif // TIMERANGE_H
