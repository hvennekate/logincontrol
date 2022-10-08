#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QElapsedTimer>
#include <QObject>
#include "sessiontimelogger.h"
#include "logintimer_adaptor.h"
#include "sessiontimebudget.h"
#include "timeslotsfortoday.h"
#include "userlist.h"

class SessionManager : public QObject
{
  Q_OBJECT
  SessionTimeBudget budgets; // TODO abstraction for budget and time slots
  TimeSlotsForToday timeSlots;
  UserList userList;
  SessionTimeLogger logger;
  QElapsedTimer timer;
  quint32 getSecsSinceLastCall();
  LogintimerAdaptor *adaptor;
public:
  explicit SessionManager(QObject *parent = nullptr);
  unsigned minutesLeft(const QString &user) const;
public slots:
  void checkSessions();
private:
  void sendRemainingMinutesMessage(const QString username, unsigned minutes) const;
};

#endif // SESSIONMANAGER_H
