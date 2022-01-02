#include "session.h"

#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

const QDBusArgument& operator>>(const QDBusArgument& arg, Session &session) {
  arg.beginStructure();
  arg >> session.sessionId
      >> session.userId
      >> session.userName
      >> session.seatId
      >> session.objectPath;
  arg.endStructure();

  return arg;
}

QDebug operator<<(QDebug debug, const Session &session) {
  debug.nospace() << "Session(id=" << session.sessionId
                  << ", userId=" << session.userId
                  << ", userName=" << session.userName
                  << ", seatId=" << session.seatId
                  << ", objectPath=" << session.objectPath.path()
                  << ")";
  return debug.resetFormat();
}

void Session::lock() const {
  QDBusInterface loginManager("org.freedesktop.login1",
                                objectPath.path(),
                                "org.freedesktop.login1.Session",
                                QDBusConnection::systemBus());
  loginManager.call("Lock");
}

bool Session::isLocked() const
{
  QDBusInterface dbusSession("org.freedesktop.login1",
                             objectPath.path(),
                             "org.freedesktop.login1.Session",
                             QDBusConnection::systemBus());
  auto locked = dbusSession.property("LockedHint").toBool();
  qDebug() << "checking session" << *this << locked;
  return locked;
}

QString Session::getUserName() const {
  return userName;
}

QVector<Session> Session::allSessions() {
  QDBusInterface loginManager("org.freedesktop.login1",
                              "/org/freedesktop/login1",
                              "org.freedesktop.login1.Manager",
                              QDBusConnection::systemBus());
  QDBusReply<QDBusArgument> reply = loginManager.call("ListSessions");
  const QDBusArgument inner = reply.value();

  inner.beginArray();
  QVector<Session> result;
  while (!inner.atEnd()) {
    Session session;
    inner >> session;
    result << session;
  }
  inner.endArray();
  return result;
}

QVector<Session> Session::allUnlockedSessions()
{
  auto sessions = allSessions();
  std::remove_if(sessions.begin(), sessions.end(), std::bind(&Session::isLocked, std::placeholders::_1));
  return sessions;
}
