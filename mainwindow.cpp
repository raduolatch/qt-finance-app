#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QMap>
#include <QDate>
#include <QLocale>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpressionValidator>
#include "chartdialog.h"

double currentRate = 0;

// CONSTRUCTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // VALIDATOR (hanya angka & titik)
    QRegularExpression rx("[0-9.]*");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->inputAmount->setValidator(validator);

    ui->inputDate->setDate(QDate::currentDate());

    ui->btnAdd->setStyleSheet("background-color: #4CAF50; color: white;");
    ui->btnDelete->setStyleSheet("background-color: #f44336; color: white;");
    ui->btnChart->setStyleSheet("background-color: #2196F3; color: white;");

    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->btnAdd,    &QPushButton::clicked, this, &MainWindow::addTransaction);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::DeleteTransaction);
    connect(ui->btnChart,  &QPushButton::clicked, this, &MainWindow::showChart);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::onApiResult);
    connect(ui->btnAPI, &QPushButton::clicked, this, &MainWindow::getExchangeRate);

    loadData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ADD
void MainWindow::addTransaction()
{
    if (ui->inputCategory->text().isEmpty() || ui->inputAmount->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Semua field harus diisi!");
        return;
    }

    QString date     = ui->inputDate->date().toString("dd/MM/yyyy");
    QString type     = ui->comboType->currentText();
    QString category = ui->inputCategory->text();

    QString text = ui->inputAmount->text().trimmed();
    text.remove(".");

    bool ok;
    double amount = text.toDouble(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Error", "Amount tidak valid!");
        return;
    }

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(date));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(category));


    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);//FORMAT + TAMBAH "Rp"
    QString formatted = "Rp " + locale.toString(static_cast<qlonglong>(amount));

    QTableWidgetItem *item = new QTableWidgetItem(formatted);


    item->setData(Qt::UserRole, amount);//simpan nilai asli

    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->tableWidget->setItem(row, 3, item);

    ui->tableWidget->selectRow(row);

    updateBalance();
    saveData();

    ui->inputCategory->clear();
    ui->inputAmount->clear();
}

// DELETE
void MainWindow::DeleteTransaction()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Pilih data dulu!");
        return;
    }
    ui->tableWidget->removeRow(row);
    updateBalance();
    saveData();
}

// UPDATE BALANCE
void MainWindow::updateBalance()
{
    double total = 0;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QString t = ui->tableWidget->item(i, 1)->text();

        double a = ui->tableWidget->item(i, 3)->data(Qt::UserRole).toDouble();

        if (t == "Income") total += a;
        else               total -= a;
    }

    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);
    ui->labelBalance->setText("Total Saldo: Rp " + locale.toString(static_cast<qlonglong>(total)));
}

// SAVE
void MainWindow::saveData()
{
    QFile file("data.csv");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);

        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {

            double amount = ui->tableWidget->item(i, 3)->data(Qt::UserRole).toDouble(); // simpan angka asli

            out << ui->tableWidget->item(i, 0)->text() << ","
                << ui->tableWidget->item(i, 1)->text() << ","
                << ui->tableWidget->item(i, 2)->text() << ","
                << amount << "\n";
        }
        file.close();
    }
}

// LOAD
void MainWindow::loadData()
{
    QFile file("data.csv");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QStringList data = in.readLine().split(",");
            if (data.size() < 4) continue;

            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(data[0]));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(data[1]));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(data[2]));

            double amount = data[3].toDouble();

            QLocale locale(QLocale::Indonesian, QLocale::Indonesia);//FORMAT + "Rp"
            QString formatted = "Rp " + locale.toString(static_cast<qlonglong>(amount));

            QTableWidgetItem *item = new QTableWidgetItem(formatted);

            item->setData(Qt::UserRole, amount);//simpan nilai asli

            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

            ui->tableWidget->setItem(row, 3, item);
        }

        file.close();
    }

    updateBalance();
}

// GET ALL TRANSACTIONS
QList<Transaction> MainWindow::getAllTransactions()
{
    QList<Transaction> list;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        Transaction t;
        t.date     = QDate::fromString(ui->tableWidget->item(i, 0)->text(), "dd/MM/yyyy");
        t.type     = ui->tableWidget->item(i, 1)->text();
        t.category = ui->tableWidget->item(i, 2)->text();
        t.amount   = ui->tableWidget->item(i, 3)->data(Qt::UserRole).toDouble();
        list.append(t);
    }

    return list;
}

// SHOW CHART (buka ChartDialog)
void MainWindow::showChart()
{
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::information(this, "Info", "Belum ada data transaksi!");
        return;
    }

    ChartDialog *dialog = new ChartDialog(getAllTransactions(), this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

//  API REQUEST
void MainWindow::getExchangeRate()
{
    QUrl url("https://api.exchangerate-api.com/v4/latest/USD");
    networkManager->get(QNetworkRequest(url));
}

//  API RESPONSE
void MainWindow::onApiResult(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Error", "Gagal ambil data API");
        reply->deleteLater();
        return;
    }

    QJsonObject rates = QJsonDocument::fromJson(reply->readAll())
                            .object()["rates"].toObject();
    double idr  = rates["IDR"].toDouble();
    currentRate = idr;
    ui->labelKurs->setText("Kurs USD: Rp " + QString::number(idr, 'f', 0));
    reply->deleteLater();
}