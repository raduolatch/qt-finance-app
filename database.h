#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QList>
#include "transaction.h"

class Database
{
public:
    static void connect();
    static bool registerUser(const QString &username, const QString &password, const QString &role = "user");
    static bool loginUser(const QString &username, const QString &password);
    static QString getUserRole(const QString &username);
    static bool addTransaction(const QString &username, const Transaction &t);
    static bool deleteTransaction(int id);
    static QList<Transaction> getTransactions(const QString &username);

    // ADMIN ONLY
    static QList<Transaction> getAllTransactions();
    static QStringList getAllUsers();
    static bool deleteUser(const QString &username);
};

#endif // DATABASE_H