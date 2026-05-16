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

// CONNECT DATABASE (MySQL via ODBC + FreeSQLDatabase)
void Database::connect()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
        return;

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(
        "DRIVER={MySQL ODBC 9.7 Unicode Driver};"
        "SERVER=sql12.freesqldatabase.com;"
        "PORT=3306;"
        "DATABASE=sql12827122;"
        "USER=sql12827122;"
        "PASSWORD=LmsNGvgYg6;"
        "OPTION=3;"
        );

    if (!db.open())
    {
        qDebug() << "Database gagal dibuka:" << db.lastError().text();
        return;
    }

    qDebug() << "MySQL Database connected via ODBC!";

    QSqlQuery query;

    // TABLE USERS dengan kolom role
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
            "username VARCHAR(100) UNIQUE NOT NULL,"
            "password VARCHAR(255) NOT NULL,"
            "role VARCHAR(20) NOT NULL DEFAULT 'user')"))
    {
        qDebug() << "Gagal membuat table users:" << query.lastError().text();
    }

    // Tambah kolom role jika belum ada (untuk database lama)
    query.exec("ALTER TABLE users ADD COLUMN IF NOT EXISTS role VARCHAR(20) NOT NULL DEFAULT 'user'");

    // TABLE TRANSACTIONS
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS transactions ("
            "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
            "username VARCHAR(100) NOT NULL,"
            "date DATE NOT NULL,"
            "type VARCHAR(20) NOT NULL,"
            "category VARCHAR(100) NOT NULL,"
            "amount DOUBLE NOT NULL)"))
    {
        qDebug() << "Gagal membuat table transactions:" << query.lastError().text();
    }
}

// REGISTER USER
bool Database::registerUser(const QString &username, const QString &password, const QString &role)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) VALUES (:username, :password, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));
    query.bindValue(":role",     role);

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

// GET USER ROLE
QString Database::getUserRole(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();
    if (query.next())
        return query.value("role").toString();
    return "user";
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

// GET TRANSACTIONS (per user)
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

// =========================
// ADMIN FUNCTIONS
// =========================

// GET ALL TRANSACTIONS (semua user)
QList<Transaction> Database::getAllTransactions()
{
    QList<Transaction> list;
    QSqlQuery query;
    query.exec("SELECT id, username, date, type, category, amount FROM transactions ORDER BY date ASC");

    while (query.next())
    {
        Transaction t;
        t.id       = query.value("id").toInt();
        t.username = query.value("username").toString();
        t.date     = QDate::fromString(query.value("date").toString(), "yyyy-MM-dd");
        t.type     = query.value("type").toString();
        t.category = query.value("category").toString();
        t.amount   = query.value("amount").toDouble();
        list.append(t);
    }
    return list;
}

// GET ALL USERS
QStringList Database::getAllUsers()
{
    QStringList list;
    QSqlQuery query;
    query.exec("SELECT username, role FROM users ORDER BY username ASC");

    while (query.next())
        list.append(query.value("username").toString() + " (" + query.value("role").toString() + ")");

    return list;
}

// DELETE USER
bool Database::deleteUser(const QString &username)
{
    QSqlQuery query;

    // Hapus semua transaksi user dulu
    query.prepare("DELETE FROM transactions WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    // Hapus user
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec())
    {
        qDebug() << "Delete user gagal:" << query.lastError().text();
        return false;
    }
    return true;
}