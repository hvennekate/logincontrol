#ifndef LOGINTIMESCONFIG_H
#define LOGINTIMESCONFIG_H

#include <QScopedPointer>
#include <QStringList>
#include <timerange.h>



class LoginTimesConfig
{
  QString userName;
  QList<TimeRange*> timeRanges;
public:
  LoginTimesConfig();
  LoginTimesConfig(const QString &user, QList<TimeRange *> ranges);
  LoginTimesConfig(LoginTimesConfig &&other);
  ~LoginTimesConfig();
  bool isValid() const;
  QString getUserName() const;
  bool allowLogin(const QDateTime &time);

  static LoginTimesConfig fromLines(const QStringList &lines);
};

#endif // LOGINTIMESCONFIG_H
