#include "sessionmanager.h"
#include <session.h>
#include <QDebug>

SessionManager::SessionManager(LoginTimesConfig &&config, QObject *parent)
  : QObject(parent),
    config(std::move(config))
{}

void SessionManager::checkSessions() {
  auto sessions = Session::allSessions();
  for (auto session : sessions) {
    if (session.getUserName() == config.getUserName()
        && !config.allowLogin(QDateTime::currentDateTime())) {
      qInfo() << "Locking session:" << session;
      session.lock();
    }
  }
}
