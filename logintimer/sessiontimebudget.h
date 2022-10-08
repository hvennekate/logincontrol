#ifndef SESSIONTIMEBUDGET_H
#define SESSIONTIMEBUDGET_H

#include <QString>

class SessionTimeBudget
{
public:
  SessionTimeBudget();
  unsigned budgetInMinutes(const QString &username) const;
};

#endif // SESSIONTIMEBUDGET_H
