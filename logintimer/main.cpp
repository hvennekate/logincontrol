#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <logintimesconfig.h>
#include <QTimer>
#include <sessionmanager.h>

enum Mode {
  Daemon,
  Login
};

void runDaemon(LoginTimesConfig &&config) {
  auto timer = new QTimer(qApp);
  timer->setInterval(60 * 1000);
  timer->setTimerType(Qt::VeryCoarseTimer);
  auto sessionManager = new SessionManager(std::move(config), qApp);
  timer->callOnTimeout(sessionManager, &SessionManager::checkSessions);
  timer->start();
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);
  auto arguments = app.arguments();
  if (arguments.size() != 3
      || ("daemon" != arguments[1] && "login" != arguments[1])) {
    qInfo() << "Need to specify exactly: mode (login or daemon) and config file. Exiting.";
    return 1;
  }
  Mode mode = "daemon" == arguments[1] ? Daemon : Login;
  QStringList configLines;
  QFile configFile(arguments.last());
  if (!configFile.open(QIODevice::ReadOnly)) {
    qInfo() << "Could not open file" << arguments.last();
    return 3;
  }
  while (!configFile.atEnd()) configLines << configFile.readLine().trimmed();
  auto config = LoginTimesConfig::fromLines(configLines);
  if (!config.isValid()) {
    qInfo() << "Configuration file not valid. Exiting.";
    return 2;
  }

  switch (mode) {
    case Daemon:
      runDaemon(std::move(config));
      return app.exec();
    case Login:
      if (qgetenv("PAM_USER") != config.getUserName()) {
        qInfo() << "User" << qgetenv("PAM_USER") << "is not config user" << config.getUserName();
        return 0;
      } else {
        return config.secondsLeft(QDateTime::currentDateTime(), SessionTimeLogger().getSecondsToday(config.getUserName())) ? 0 : 1;
      }
  }
}
