#ifndef LOGINTIMESCONFIG_H
#define LOGINTIMESCONFIG_H

#include <QScopedPointer>
#include <QStringList>
#include <timerange.h>



class LoginTimesConfig
{
  QString userName;
  qint32 maxSecsPerDay;
  QList<TimeRange*> timeRanges;
public:
  LoginTimesConfig();
  LoginTimesConfig(const QString &user, const qint32 maxSecsPerDay, QList<TimeRange *> ranges);
  LoginTimesConfig(LoginTimesConfig &&other);
  ~LoginTimesConfig();
  bool isValid() const;
  QString getUserName() const;
  quint32 secondsLeft(const QDateTime &time, const quint32 &secsToday);

  static LoginTimesConfig fromLines(const QStringList &lines);
};

#endif // LOGINTIMESCONFIG_H
