#ifndef SESSIONTIMELOGGER_H
#define SESSIONTIMELOGGER_H

#include <QObject>

class SessionTimeLogger
{
  static void requireDB();
public:
  SessionTimeLogger();
  void log(const QString &username, quint32 secs);
  quint32 getSecondsToday(const QString &username) const;
};

#endif // SESSIONTIMELOGGER_H
