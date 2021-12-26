#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <logintimesconfig.h>

class SessionManager : public QObject
{
  Q_OBJECT
  LoginTimesConfig config;
public:
  explicit SessionManager(LoginTimesConfig &&config , QObject *parent = nullptr);
public slots:
  void checkSessions();
};

#endif // SESSIONMANAGER_H
