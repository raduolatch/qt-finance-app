#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDate>
#include <QString>

struct Transaction {
    int     id       = 0;
    int     userId   = 0;
    QDate   date     = QDate::currentDate();
    QString type;
    QString category;
    double  amount   = 0.0;
};

#endif // TRANSACTION_H