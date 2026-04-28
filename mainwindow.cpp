#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QMap>

//CONSTRUCTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // style tombol
    ui->btnAdd->setStyleSheet("background-color: #4CAF50; color: white;");
    ui->btnDelete->setStyleSheet("background-color: #f44336; color: white;");
    ui->btnChart->setStyleSheet("background-color: #2196F3; color: white;");

    // table
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // tombol add
    connect(ui->btnAdd, &QPushButton::clicked,
            this, &MainWindow::addTransaction);

    // tombol delete
    connect(ui->btnDelete, &QPushButton::clicked,
            this, &MainWindow::DeleteTransaction);

    // tombol chart
    connect(ui->btnChart, &QPushButton::clicked,
            this, &MainWindow::showChart);

    loadData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//ADD
void MainWindow::addTransaction()
{
    // validasi input
    if (ui->inputCategory->text().isEmpty() || ui->inputAmount->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Semua field harus diisi!");
        return;
    }

    QString type = ui->comboType->currentText();
    QString category = ui->inputCategory->text();
    double amount = ui->inputAmount->text().toDouble();

    if (amount <= 0) {
        QMessageBox::warning(this, "Error", "Amount harus lebih dari 0!");
        return;
    }

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(type));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(category));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(amount)));

    //select row baru
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

//UPDATE BALANCE
void MainWindow::updateBalance()
{
    double total = 0;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QString t = ui->tableWidget->item(i, 0)->text();
        double a = ui->tableWidget->item(i, 2)->text().toDouble();

        if (t == "Income")
            total += a;
        else
            total -= a;
    }

    // format saldo
    ui->labelBalance->setText("Total Saldo: Rp " + QString::number(total, 'f', 0));
}

//SAVE
void MainWindow::saveData()
{
    QFile file("data.csv");

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);

        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            QString type = ui->tableWidget->item(i, 0)->text();
            QString category = ui->tableWidget->item(i, 1)->text();
            QString amount = ui->tableWidget->item(i, 2)->text();

            out << type << "," << category << "," << amount << "\n";
        }

        file.close();
    }
}

//LOAD
void MainWindow::loadData()
{
    QFile file("data.csv");

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList data = line.split(",");

            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(data[0]));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(data[1]));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(data[2]));
        }

        file.close();
    }

    updateBalance();
}

// CHART
void MainWindow::showChart()
{
    QMap<QString, double> categoryTotal;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QString category = ui->tableWidget->item(i, 1)->text();
        double amount = ui->tableWidget->item(i, 2)->text().toDouble();

        categoryTotal[category] += amount;
    }

    QPieSeries *series = new QPieSeries();

    for (auto it = categoryTotal.begin(); it != categoryTotal.end(); ++it) {
        series->append(it.key(), it.value());
    }

    //chart
    int i = 0;
    for (auto slice : series->slices()) {
        slice->setLabelVisible();
        slice->setExploded(i == 0);
        i++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pengeluaran per Kategori");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize(500, 400);
    chartView->show();
}