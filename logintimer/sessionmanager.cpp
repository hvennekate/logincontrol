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

SessionManager::SessionManager(QObject *parent)
  : QObject(parent),
    adaptor(new LogintimerAdaptor(this))
{
  timer.start();
}

unsigned SessionManager::minutesLeft(const QString &user) const
{
  if (!userList.userNames().contains(user)) return 1;
  auto budget = budgets.budgetInMinutes(user);
  auto usage = logger.getMinutesToday(user);
  if (budget < usage) {
    qDebug() << "Budget consumed for user" << user;
    return 0;
  }
  auto remainingInTimeSlot = timeSlots.remainingMinutes(user);
  qDebug() << "Remaining minutes" << budget << usage << remainingInTimeSlot;
  return qMin(budget - usage, remainingInTimeSlot);
}

void SessionManager::checkSessions() {
  auto loggedInUsers = Session::allLoggedInUsers();
  QSet<QString> relevantUsers = loggedInUsers & userList.userNames();

  QMap<QString, unsigned> minutesLeft;
  for (auto user : relevantUsers) minutesLeft[user] = this->minutesLeft(user);

  std::for_each(minutesLeft.constKeyValueBegin(), minutesLeft.constKeyValueEnd(),
                [&](const std::pair<QString, unsigned> &it) { sendRemainingMinutesMessage(it.first, it.second); });

  QSet<QString> usersToLock;
  for (auto it = minutesLeft.constKeyValueBegin(); it != minutesLeft.constKeyValueEnd(); ++it)
    if (!it->second) usersToLock << it->first;

  for (auto session : Session::allUnlockedSessions()) {
    if (usersToLock.contains(session.getUserName()))
      session.lock();
  }

  qDebug() << "Logging users:" << loggedInUsers;
  for (auto user : loggedInUsers) logger.log(user);
}

void SessionManager::sendRemainingMinutesMessage(const QString username, unsigned minutes) const
{
  QDBusMessage message = QDBusMessage::createSignal("/org/hendrik/logintimer",
                                                    "org.hendrik.logintimer",
                                                    "secondsLeft");
  message << username << minutes * 60;
  QDBusConnection::systemBus().send(message);
}
