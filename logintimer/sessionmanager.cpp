#include "sessionmanager.h"
#include <session.h>
#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>
#include "logintimer_adaptor.h"

quint32 SessionManager::getSecsSinceLastCall()
{
  return std::min(timer.restart()/1000, (qint64) std::numeric_limits<quint32>::max());
}

SessionManager::SessionManager(LoginTimesConfig &&config, QObject *parent)
  : QObject(parent),
    config(std::move(config)),
    adaptor(new LogintimerAdaptor(this))
{
  timer.start();
}

void SessionManager::checkSessions() {
  auto sessions = Session::allUnlockedSessions();
  qDebug() << "Sessions to check" << sessions;
  auto secsToLog = getSecsSinceLastCall();
  QSet<QString> loggedUsers;
  for (auto session : sessions) {
    if (session.getUserName() != config.getUserName()) continue;
    if (!loggedUsers.contains(session.getUserName())) logger.log(session.getUserName(), secsToLog);
    loggedUsers += session.getUserName();
    auto secsToday = logger.getSecondsToday(session.getUserName());
    auto secondsLeft = config.secondsLeft(QDateTime::currentDateTime(), secsToday);

    QDBusMessage message = QDBusMessage::createSignal("/org/hendrik/logintimer",
                                                      "org.hendrik.logintimer",
                                                      "secondsLeft");
    message << session.getUserName() << secondsLeft;
    QDBusConnection::systemBus().send(message);

    if(!secondsLeft) {
      qInfo() << "Locking session:" << session;
      session.lock();
    }
  }
}
