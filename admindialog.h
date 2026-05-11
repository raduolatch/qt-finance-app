#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QDialog>
#include <QList>
#include "transaction.h"

namespace Ui {
class AdminDialog;
}

class AdminDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdminDialog(const QString &adminUsername, QWidget *parent = nullptr);
    ~AdminDialog();

private slots:
    void loadAllTransactions();
    void loadAllUsers();
    void deleteSelectedTransaction();
    void deleteSelectedUser();
    void filterByUser();

private:
    Ui::AdminDialog *ui;
    QString adminUsername;
    void setupUI();
};

#endif // ADMINDIALOG_H