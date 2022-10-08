#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <sessionmanager.h>
#include <QSqlDatabase>

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        abort();
      case QtInfoMsg:
        fprintf(stderr, "Info: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
    }
}

enum Mode {
  Daemon,
  Login
};

void runDaemon() {
  auto timer = new QTimer(qApp);
  timer->setInterval(60 * 1000);
  timer->setTimerType(Qt::VeryCoarseTimer);
  auto sessionManager = new SessionManager(qApp);
  timer->callOnTimeout(sessionManager, &SessionManager::checkSessions);
  timer->start();
}

int main(int argc, char **argv) {
  qInstallMessageHandler(messageOutput);
  QCoreApplication app(argc, argv);

  auto db = QSqlDatabase::addDatabase("QSQLITE3");
  db.setDatabaseName("/var/log/logintimes");
  db.open();

  auto arguments = app.arguments();
  if (arguments.size() != 2 || ( "daemon" != arguments[1] && "login" != arguments[1])) {
    qInfo() << "Need to specify exactly: mode (login or daemon). Exiting.";
    return 1;
  }
  Mode mode = "daemon" == arguments[1] ? Daemon : Login;

  switch (mode) {
    case Daemon:
      runDaemon();
      return app.exec();
    case Login:
      auto user = qgetenv("PAM_USER");
      return SessionManager().minutesLeft(user) > 0 ? 0 : 1;
  }
}
