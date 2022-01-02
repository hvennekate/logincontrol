#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QElapsedTimer>
#include <QObject>
#include <logintimesconfig.h>
#include <sessiontimelogger.h>
#include "logintimer_adaptor.h"

class SessionManager : public QObject
{
  Q_OBJECT
  LoginTimesConfig config;
  SessionTimeLogger logger;
  QElapsedTimer timer;
  quint32 getSecsSinceLastCall();
  LogintimerAdaptor *adaptor;
public:
  explicit SessionManager(LoginTimesConfig &&config , QObject *parent = nullptr);
public slots:
  void checkSessions();
};

#endif // SESSIONMANAGER_H
