#include "userlist.h"
#include "dbutils.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

UserList::UserList()
{
  initializeTable("user",
                  "CREATE TABLE user ("
                  " name varchar(255) NOT NULL,"
                  " primary key (name))");
}

QSet<QString> UserList::userNames() const
{
  QSqlQuery userQuery("SELECT name FROM user");
  if (!userQuery.exec()) {
    qInfo() << "Query for user names failed:" << userQuery.lastError();
    return {};
  }
  QSet<QString> result;
  while (userQuery.next()) {
    result << userQuery.value(0).toString();
  }
  return result;
}


