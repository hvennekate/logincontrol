#ifndef USERLIST_H
#define USERLIST_H

#include <QSet>



class UserList
{
public:
  UserList();
  QSet<QString> userNames() const;
};

#endif // USERLIST_H
