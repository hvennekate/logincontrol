#ifndef SESSION_H
#define SESSION_H

#include <QDBusArgument>
#include <QDBusObjectPath>
#include <QString>



class Session {
  QString sessionId, userName, seatId;
  quint32 userId;
  QDBusObjectPath objectPath;
public:
  friend const QDBusArgument& operator>>(const QDBusArgument& arg, Session &session);
  friend QDebug operator<<(QDebug debug, const Session &session);

  void lock() const;
  bool isLocked() const;
  QString getUserName() const;

  static QVector<Session> allSessions();
  static QVector<Session> allUnlockedSessions();
};

const QDBusArgument& operator>>(const QDBusArgument& arg, Session &session);
QDebug operator<<(QDebug debug, const Session &session);

#endif // SESSION_H
