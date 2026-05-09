#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

// HASH PASSWORD
static QString hashPassword(const QString &password)
{
    return QString(QCryptographicHash::hash(
                       password.toUtf8(),
                       QCryptographicHash::Sha256).toHex());
}

// CONNECT DATABASE (MySQL via ODBC + Laragon)
void Database::connect()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
        return;

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    // Connection string ODBC ke MySQL FreeSQLDatabase.com
    db.setDatabaseName(
        "DRIVER={MySQL ODBC 9.7 Unicode Driver};"
        "SERVER=sql12.freesqldatabase.com;"
        "PORT=3306;"
        "DATABASE=sql12825994;"
        "USER=sql12825994;"
        "PASSWORD=njWQHwHVah;"
        "OPTION=3;"
        );

    if (!db.open())
    {
        qDebug() << "Database gagal dibuka:" << db.lastError().text();
        return;
    }

    qDebug() << "MySQL Database connected via ODBC!";
}

// REGISTER USER
bool Database::registerUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));

    if (!query.exec())
    {
        qDebug() << "Register gagal:" << query.lastError().text();
        return false;
    }
    return true;
}

// LOGIN USER
bool Database::loginUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));
    query.exec();
    return query.next();
}

// ADD TRANSACTION
bool Database::addTransaction(const QString &username, const Transaction &t)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO transactions (username, date, type, category, amount) "
        "VALUES (:username, :date, :type, :category, :amount)");
    query.bindValue(":username", username);
    query.bindValue(":date",     t.date.toString("yyyy-MM-dd"));
    query.bindValue(":type",     t.type);
    query.bindValue(":category", t.category);
    query.bindValue(":amount",   t.amount);

    if (!query.exec())
    {
        qDebug() << "Add transaction gagal:" << query.lastError().text();
        return false;
    }
    return true;
}

// DELETE TRANSACTION
bool Database::deleteTransaction(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM transactions WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Delete transaction gagal:" << query.lastError().text();
        return false;
    }
    return true;
}

// GET TRANSACTIONS
QList<Transaction> Database::getTransactions(const QString &username)
{
    QList<Transaction> list;
    QSqlQuery query;
    query.prepare(
        "SELECT id, date, type, category, amount "
        "FROM transactions WHERE username = :username ORDER BY date ASC");
    query.bindValue(":username", username);
    query.exec();

    while (query.next())
    {
        Transaction t;
        t.id       = query.value("id").toInt();
        t.date     = QDate::fromString(query.value("date").toString(), "yyyy-MM-dd");
        t.type     = query.value("type").toString();
        t.category = query.value("category").toString();
        t.amount   = query.value("amount").toDouble();
        list.append(t);
    }
    return list;
}