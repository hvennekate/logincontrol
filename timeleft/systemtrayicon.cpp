#include "systemtrayicon.h"

#include <QApplication>
#include <QIcon>
#include <QInputDialog>
#include <QMenu>
#include <QSettings>
#include <QDebug>

const char START_WARNING_MINUTES_LEFT[] = "startWarningWhenMinsLeft";

SystemtrayIcon::SystemtrayIcon(QObject *parent)
  : QSystemTrayIcon(QIcon::fromTheme("utilities-terminal"), parent)
{
  auto contextMenu = new QMenu();
  contextMenu->addAction("Settings...", this, &SystemtrayIcon::showSettings);
  contextMenu->addAction(QIcon::fromTheme("application-exit"),
                         "Exit",
                         qApp,
                         &QApplication::exit);
  setContextMenu(contextMenu);
}

void SystemtrayIcon::receiveMessage(QString user, uint secsLeft)
{
  qDebug() << "Received message" << user << secsLeft << qgetenv("USER");
  if (user != qgetenv("USER")) return;
  int minutesLeft = secsLeft/60.;
  if (minutesLeft > QSettings().value(START_WARNING_MINUTES_LEFT).toInt()) return;
  showMessage("Remaining time",
              QString("Remaining time: %1 minutes!").arg(minutesLeft),
              QIcon::fromTheme("dialog-warning"));
}

void SystemtrayIcon::showSettings()
{
  bool ok = false;
  int minsLeftForWarning = QInputDialog::getInt(nullptr,
                                                "Settings",
                                                "Start warning when how many minutes left?",
                                                QSettings().value(START_WARNING_MINUTES_LEFT, 10).toInt(),
                                                1, 100, 5, &ok);
  if (ok) QSettings().setValue(START_WARNING_MINUTES_LEFT, minsLeftForWarning);
}
