#include <QApplication>
#include <QDBusConnection>
#include <QSystemTrayIcon>
#include <systemtrayicon.h>
#include <QDebug>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("Hendrik");
  QCoreApplication::setOrganizationDomain("hendrik.org");
  QCoreApplication::setApplicationName("Time Left Systray Icon");

  auto icon = new SystemtrayIcon(&app);
  auto connected = QDBusConnection::systemBus().connect(QString(),
                                       "/org/hendrik/logintimer",
                                       "org.hendrik.logintimer",
                                       "secondsLeft",
                                       icon,
                                       SLOT(receiveMessage(QString, uint)));
  qDebug() << "DBus connection:" << connected;

  icon->show();

  return app.exec();
}
