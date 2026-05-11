#include "admindialog.h"
#include "ui_admindialog.h"
#include "database.h"
#include <QMessageBox>
#include <QLocale>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>

AdminDialog::AdminDialog(const QString &adminUsername, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminDialog)
    , adminUsername(adminUsername)
{
    ui->setupUi(this);
    setWindowTitle("Admin Panel - " + adminUsername);
    resize(900, 600);

    setupUI();

    connect(ui->btnAllTransactions, &QPushButton::clicked,
            this, &AdminDialog::loadAllTransactions);
    connect(ui->btnAllUsers,        &QPushButton::clicked,
            this, &AdminDialog::loadAllUsers);
    connect(ui->btnDeleteTransaction, &QPushButton::clicked,
            this, &AdminDialog::deleteSelectedTransaction);
    connect(ui->btnDeleteUser,      &QPushButton::clicked,
            this, &AdminDialog::deleteSelectedUser);
    connect(ui->btnFilter,          &QPushButton::clicked,
            this, &AdminDialog::filterByUser);

    // Load semua transaksi saat pertama buka
    loadAllTransactions();
    loadAllUsers();
}

AdminDialog::~AdminDialog()
{
    delete ui;
}

void AdminDialog::setupUI()
{
    // Setup tabel transaksi
    ui->tableTransactions->setColumnCount(6);
    ui->tableTransactions->setHorizontalHeaderLabels(
        {"ID", "Username", "Tanggal", "Tipe", "Kategori", "Jumlah"});
    ui->tableTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTransactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableTransactions->setAlternatingRowColors(true);
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Setup tabel users
    ui->tableUsers->setColumnCount(2);
    ui->tableUsers->setHorizontalHeaderLabels({"Username", "Role"});
    ui->tableUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableUsers->setAlternatingRowColors(true);
    ui->tableUsers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// LOAD SEMUA TRANSAKSI
void AdminDialog::loadAllTransactions()
{
    ui->tableTransactions->setRowCount(0);
    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);

    QList<Transaction> list = Database::getAllTransactions();

    for (const Transaction &t : list)
    {
        int row = ui->tableTransactions->rowCount();
        ui->tableTransactions->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(t.id));
        idItem->setData(Qt::UserRole, t.id);

        ui->tableTransactions->setItem(row, 0, idItem);
        ui->tableTransactions->setItem(row, 1, new QTableWidgetItem(t.username));
        ui->tableTransactions->setItem(row, 2, new QTableWidgetItem(t.date.toString("yyyy-MM-dd")));
        ui->tableTransactions->setItem(row, 3, new QTableWidgetItem(t.type));
        ui->tableTransactions->setItem(row, 4, new QTableWidgetItem(t.category));

        QTableWidgetItem *amtItem = new QTableWidgetItem(
            "Rp " + locale.toString((qlonglong)t.amount));
        amtItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableTransactions->setItem(row, 5, amtItem);
    }

    ui->labelTransactionCount->setText(
        QString("Total: %1 transaksi").arg(list.size()));
}

// LOAD SEMUA USERS
void AdminDialog::loadAllUsers()
{
    ui->tableUsers->setRowCount(0);

    QSqlQuery query;
    query.exec("SELECT username, role FROM users ORDER BY username ASC");

    while (query.next())
    {
        int row = ui->tableUsers->rowCount();
        ui->tableUsers->insertRow(row);
        ui->tableUsers->setItem(row, 0, new QTableWidgetItem(query.value("username").toString()));
        ui->tableUsers->setItem(row, 1, new QTableWidgetItem(query.value("role").toString()));
    }

    ui->labelUserCount->setText(
        QString("Total: %1 user").arg(ui->tableUsers->rowCount()));
}

// DELETE TRANSAKSI TERPILIH
void AdminDialog::deleteSelectedTransaction()
{
    int row = ui->tableTransactions->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Pilih Baris",
                             "Pilih transaksi yang ingin dihapus!");
        return;
    }

    auto reply = QMessageBox::question(
        this, "Konfirmasi", "Hapus transaksi ini?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    int id = ui->tableTransactions->item(row, 0)->data(Qt::UserRole).toInt();

    if (Database::deleteTransaction(id))
    {
        ui->tableTransactions->removeRow(row);
        QMessageBox::information(this, "Sukses", "Transaksi berhasil dihapus!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Gagal menghapus transaksi!");
    }
}

// DELETE USER TERPILIH
void AdminDialog::deleteSelectedUser()
{
    int row = ui->tableUsers->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Pilih Baris",
                             "Pilih user yang ingin dihapus!");
        return;
    }

    QString username = ui->tableUsers->item(row, 0)->text();

    // Tidak boleh hapus diri sendiri
    if (username == adminUsername)
    {
        QMessageBox::warning(this, "Error",
                             "Tidak bisa menghapus akun admin yang sedang login!");
        return;
    }

    auto reply = QMessageBox::question(
        this, "Konfirmasi",
        "Hapus user \"" + username + "\" beserta semua transaksinya?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    if (Database::deleteUser(username))
    {
        ui->tableUsers->removeRow(row);
        loadAllTransactions(); // refresh transaksi
        QMessageBox::information(this, "Sukses",
                                 "User \"" + username + "\" berhasil dihapus!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Gagal menghapus user!");
    }
}

// FILTER TRANSAKSI BY USERNAME
void AdminDialog::filterByUser()
{
    QString filterUser = ui->inputFilter->text().trimmed();

    if (filterUser.isEmpty())
    {
        loadAllTransactions();
        return;
    }

    ui->tableTransactions->setRowCount(0);
    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);

    QList<Transaction> list = Database::getAllTransactions();
    int count = 0;

    for (const Transaction &t : list)
    {
        if (!t.username.contains(filterUser, Qt::CaseInsensitive))
            continue;

        int row = ui->tableTransactions->rowCount();
        ui->tableTransactions->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(t.id));
        idItem->setData(Qt::UserRole, t.id);

        ui->tableTransactions->setItem(row, 0, idItem);
        ui->tableTransactions->setItem(row, 1, new QTableWidgetItem(t.username));
        ui->tableTransactions->setItem(row, 2, new QTableWidgetItem(t.date.toString("yyyy-MM-dd")));
        ui->tableTransactions->setItem(row, 3, new QTableWidgetItem(t.type));
        ui->tableTransactions->setItem(row, 4, new QTableWidgetItem(t.category));

        QTableWidgetItem *amtItem = new QTableWidgetItem(
            "Rp " + locale.toString((qlonglong)t.amount));
        amtItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableTransactions->setItem(row, 5, amtItem);
        count++;
    }

    ui->labelTransactionCount->setText(
        QString("Total: %1 transaksi (filter: %2)").arg(count).arg(filterUser));
}