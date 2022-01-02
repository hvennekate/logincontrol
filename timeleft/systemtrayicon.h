#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QObject>

class SystemtrayIcon : public QSystemTrayIcon
{
  Q_OBJECT
public:
  explicit SystemtrayIcon(QObject *parent = nullptr);
public slots:
  void receiveMessage(QString user, uint secsLeft);
private slots:
  void showSettings();
};

#endif // SYSTEMTRAYICON_H
