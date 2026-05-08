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
#include <QScrollBar>
#include <QTextToSpeech>
#include "chartdialog.h"
#include <QProcess>
#include <QDebug>
#include <QCoreApplication>

double currentRate = 0;

// CONSTRUCTOR

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // TEXT TO SPEECH
    speaker = new QTextToSpeech(this);

    speaker->setLocale(
        QLocale(QLocale::Indonesian)
        );

    speaker->setRate(0);

    // VALIDATOR
    QRegularExpression rx("[0-9.]*");

    QValidator *validator =
        new QRegularExpressionValidator(rx, this);

    ui->inputAmount->setValidator(validator);

    // DATE
    ui->inputDate->setDate(QDate::currentDate());

    // BUTTON STYLE
    ui->btnAdd->setStyleSheet(
        "background-color:#4CAF50;"
        "color:white;"
        "font-weight:bold;"
        );

    ui->btnDelete->setStyleSheet(
        "background-color:#f44336;"
        "color:white;"
        "font-weight:bold;"
        );

    ui->btnChart->setStyleSheet(
        "background-color:#2196F3;"
        "color:white;"
        "font-weight:bold;"
        );

    ui->btnAPI->setStyleSheet(
        "background-color:#FF9800;"
        "color:white;"
        "font-weight:bold;"
        );

    ui->btnAI->setStyleSheet(
        "background-color:#9C27B0;"
        "color:white;"
        "font-weight:bold;"
        );

    ui->btnChat->setStyleSheet(
        "background-color:#009688;"
        "color:white;"
        "font-weight:bold;"
        );

    connect(ui->btnVoice,
            &QPushButton::clicked,
            this,
            &MainWindow::processVoice);

    // TABLE
    ui->tableWidget->setAlternatingRowColors(true);

    ui->tableWidget->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tableWidget->setColumnWidth(0, 120);
    ui->tableWidget->setColumnWidth(1, 120);
    ui->tableWidget->setColumnWidth(2, 220);
    ui->tableWidget->setColumnWidth(3, 180);

    // CONNECT
    connect(ui->btnAdd,
            &QPushButton::clicked,
            this,
            &MainWindow::addTransaction);

    connect(ui->btnDelete,
            &QPushButton::clicked,
            this,
            &MainWindow::DeleteTransaction);

    connect(ui->btnChart,
            &QPushButton::clicked,
            this,
            &MainWindow::showChart);

    connect(ui->btnAPI,
            &QPushButton::clicked,
            this,
            &MainWindow::getExchangeRate);

    connect(ui->btnAI,
            &QPushButton::clicked,
            this,
            &MainWindow::showAISummary);

    connect(ui->btnChat,
            &QPushButton::clicked,
            this,
            &MainWindow::processAIChat);

    connect(ui->inputChat,
            &QLineEdit::returnPressed,
            this,
            &MainWindow::processAIChat);
    connect(ui->btnVoice,
            &QPushButton::clicked,
            this,
            &MainWindow::processVoice);

    // NETWORK
    networkManager =
        new QNetworkAccessManager(this);

    connect(networkManager,
            &QNetworkAccessManager::finished,
            this,
            &MainWindow::onApiResult);

    // LOAD DATA
    loadData();
}

// DESTRUCTOR

MainWindow::~MainWindow()
{
    delete ui;
}

// ADD TRANSACTION

void MainWindow::addTransaction()
{
    if (ui->inputCategory->text().isEmpty()
        || ui->inputAmount->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Error",
            "Semua field harus diisi!"
            );

        return;
    }

    QString date =
        ui->inputDate->date()
            .toString("dd/MM/yyyy");

    QString type =
        ui->comboType->currentText();

    QString category =
        autoCorrectCategory(
            ui->inputCategory->text()
            );

    QString text =
        ui->inputAmount->text().trimmed();

    text.remove(".");

    bool ok;

    double amount =
        text.toDouble(&ok);

    if (!ok || amount <= 0)
    {
        QMessageBox::warning(
            this,
            "Error",
            "Amount tidak valid!"
            );

        return;
    }

    int row =
        ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(
        row,
        0,
        new QTableWidgetItem(date)
        );

    ui->tableWidget->setItem(
        row,
        1,
        new QTableWidgetItem(type)
        );

    ui->tableWidget->setItem(
        row,
        2,
        new QTableWidgetItem(category)
        );

    // FORMAT RUPIAH
    QLocale locale(
        QLocale::Indonesian,
        QLocale::Indonesia
        );

    QString formatted =
        "Rp "
        + locale.toString(
            static_cast<qlonglong>(amount)
            );

    QTableWidgetItem *item =
        new QTableWidgetItem(formatted);

    item->setData(
        Qt::UserRole,
        amount
        );

    item->setTextAlignment(
        Qt::AlignRight
        | Qt::AlignVCenter
        );

    ui->tableWidget->setItem(
        row,
        3,
        item
        );

    ui->tableWidget->selectRow(row);

    updateBalance();

    saveData();

    ui->inputCategory->clear();
    ui->inputAmount->clear();
}

// DELETE TRANSACTION

void MainWindow::DeleteTransaction()
{
    int row =
        ui->tableWidget->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Error",
            "Pilih data dulu!"
            );

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

    for (int i = 0;
         i < ui->tableWidget->rowCount();
         i++)
    {
        QString type =
            ui->tableWidget
                ->item(i, 1)
                ->text();

        double amount =
            ui->tableWidget
                ->item(i, 3)
                ->data(Qt::UserRole)
                .toDouble();

        if (type == "Income")
            total += amount;
        else
            total -= amount;
    }

    QLocale locale(
        QLocale::Indonesian,
        QLocale::Indonesia
        );

    ui->labelBalance->setText(
        "Total Saldo: Rp "
        + locale.toString(
            static_cast<qlonglong>(total)
            )
        );
}

// SAVE DATA

void MainWindow::saveData()
{
    QFile file("data.csv");

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);

        for (int i = 0;
             i < ui->tableWidget->rowCount();
             i++)
        {
            double amount =
                ui->tableWidget
                    ->item(i, 3)
                    ->data(Qt::UserRole)
                    .toDouble();

            out
                << ui->tableWidget->item(i, 0)->text()
                << ","

                << ui->tableWidget->item(i, 1)->text()
                << ","

                << ui->tableWidget->item(i, 2)->text()
                << ","

                << amount
                << "\n";
        }

        file.close();
    }
}

// LOAD DATA

void MainWindow::loadData()
{
    QFile file("data.csv");

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QStringList data =
                in.readLine().split(",");

            if (data.size() < 4)
                continue;

            int row =
                ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(
                row,
                0,
                new QTableWidgetItem(data[0])
                );

            ui->tableWidget->setItem(
                row,
                1,
                new QTableWidgetItem(data[1])
                );

            ui->tableWidget->setItem(
                row,
                2,
                new QTableWidgetItem(data[2])
                );

            double amount =
                data[3].toDouble();

            QLocale locale(
                QLocale::Indonesian,
                QLocale::Indonesia
                );

            QString formatted =
                "Rp "
                + locale.toString(
                    static_cast<qlonglong>(amount)
                    );

            QTableWidgetItem *item =
                new QTableWidgetItem(formatted);

            item->setData(
                Qt::UserRole,
                amount
                );

            item->setTextAlignment(
                Qt::AlignRight
                | Qt::AlignVCenter
                );

            ui->tableWidget->setItem(
                row,
                3,
                item
                );
        }

        file.close();
    }

    updateBalance();
}

// GET ALL TRANSACTIONS

QList<Transaction> MainWindow::getAllTransactions()
{
    QList<Transaction> list;

    for (int i = 0;
         i < ui->tableWidget->rowCount();
         i++)
    {
        Transaction t;

        t.date =
            QDate::fromString(
                ui->tableWidget
                    ->item(i, 0)
                    ->text(),
                "dd/MM/yyyy"
                );

        t.type =
            ui->tableWidget
                ->item(i, 1)
                ->text();

        t.category =
            ui->tableWidget
                ->item(i, 2)
                ->text();

        t.amount =
            ui->tableWidget
                ->item(i, 3)
                ->data(Qt::UserRole)
                .toDouble();

        list.append(t);
    }

    return list;
}

// SHOW CHART

void MainWindow::showChart()
{
    if (ui->tableWidget->rowCount() == 0)
    {
        QMessageBox::information(
            this,
            "Info",
            "Belum ada data transaksi!"
            );

        return;
    }

    ChartDialog *dialog =
        new ChartDialog(
            getAllTransactions(),
            this
            );

    dialog->setAttribute(
        Qt::WA_DeleteOnClose
        );

    dialog->show();
}

// GET EXCHANGE RATE

void MainWindow::getExchangeRate()
{
    QUrl url(
        "https://api.exchangerate-api.com/v4/latest/USD"
        );

    networkManager->get(
        QNetworkRequest(url)
        );
}

// API RESULT

void MainWindow::onApiResult(QNetworkReply *reply)
{
    if (reply->error()
        != QNetworkReply::NoError)
    {
        QMessageBox::warning(
            this,
            "Error",
            "Gagal ambil data API"
            );

        reply->deleteLater();

        return;
    }

    QJsonObject rates =
        QJsonDocument::fromJson(
            reply->readAll()
            )
            .object()["rates"]
            .toObject();

    double idr =
        rates["IDR"].toDouble();

    currentRate = idr;

    ui->labelKurs->setText(
        "Kurs USD: Rp "
        + QString::number(idr, 'f', 0)
        );

    reply->deleteLater();
}

// AUTO CATEGORY

QString MainWindow::autoCorrectCategory(QString text)
{
    text =
        text.trimmed().toLower();

    if (text == "mkn"
        || text == "makann"
        || text == "makanan")
    {
        return "Makan";
    }

    if (text == "ojol"
        || text == "trasnport"
        || text == "transportasii")
    {
        return "Transport";
    }

    if (text == "salary"
        || text == "gajih")
    {
        return "Gaji";
    }

    return
        text.left(1).toUpper()
        + text.mid(1);
}

// AI SUMMARY

QString MainWindow::generateAISummary()
{
    double income = 0;
    double expense = 0;

    QMap<QString, double> categoryMap;

    for (int i = 0;
         i < ui->tableWidget->rowCount();
         i++)
    {
        QString type =
            ui->tableWidget
                ->item(i, 1)
                ->text();

        QString category =
            ui->tableWidget
                ->item(i, 2)
                ->text();

        double amount =
            ui->tableWidget
                ->item(i, 3)
                ->data(Qt::UserRole)
                .toDouble();

        if (type == "Income")
        {
            income += amount;
        }
        else
        {
            expense += amount;
            categoryMap[category] += amount;
        }
    }

    QString biggestCategory = "-";
    double biggestValue = 0;

    for (auto it = categoryMap.begin();
         it != categoryMap.end();
         ++it)
    {
        if (it.value() > biggestValue)
        {
            biggestValue = it.value();
            biggestCategory = it.key();
        }
    }

    double saldo =
        income - expense;

    return
        "AI Financial Summary\n\n"

        "Total Income : Rp "
        + QString::number(income, 'f', 0)

        + "\nTotal Expense : Rp "
        + QString::number(expense, 'f', 0)

        + "\nSaldo Saat Ini : Rp "
        + QString::number(saldo, 'f', 0)

        + "\n\nKategori Pengeluaran Terbesar:\n"

        + biggestCategory

        + " (Rp "
        + QString::number(biggestValue, 'f', 0)
        + ")";
}

// SHOW AI SUMMARY

void MainWindow::showAISummary()
{
    QMessageBox::information(
        this,
        "AI Financial Summary",
        generateAISummary()
        );
}

// PROCESS AI CHAT

void MainWindow::processAIChat()
{
    QString question =
        ui->inputChat->text().trimmed();

    if (question.isEmpty())
        return;

    QString answer =
        getAIResponse(question);

    ui->chatOutput->append(
        "<b>You :</b> " + question
        );

    ui->chatOutput->append(
        "<font color='blue'><b>AI :</b></font> "
        + answer + "<br>"
        );

    speaker->say(answer);

    ui->chatOutput
        ->verticalScrollBar()
        ->setValue(
            ui->chatOutput
                ->verticalScrollBar()
                ->maximum()
            );

    ui->inputChat->clear();
}

void MainWindow::processVoice()
{
    QMessageBox::information(
        this,
        "Voice Assistant",
        "Setelah tekan OK, tunggu 1 detik lalu bicara"
        );

    QProcess process;

    // lokasi folder project
    process.setWorkingDirectory(
        "C:/apkproject/FinanceApp"
        );

    // jalankan python
    process.start(
        "python",
        QStringList() << "speech.py"
        );

    // tunggu python selesai
    process.waitForFinished(-1);

    QString output =
        process.readAllStandardOutput();

    QString error =
        process.readAllStandardError();

    qDebug() << "OUTPUT =" << output;
    qDebug() << "ERROR =" << error;

    output = output.trimmed();

    // hapus text bawaan python
    output.remove("Silakan bicara...");
    output = output.trimmed();

    // kalau gagal
    if (output.isEmpty()
        || output.contains("tidak dikenali")
        || output.contains("Tidak ada suara"))
    {
        QMessageBox::warning(
            this,
            "Voice",
            "Suara tidak dikenali!"
            );

        return;
    }

    // masukkan hasil suara ke chat
    ui->inputChat->setText(output);

    // kirim ke AI
    processAIChat();
}

// AI RESPONSE

QString MainWindow::getAIResponse(QString question)
{
    question =
        question.toLower();

    double income = 0;
    double expense = 0;

    QMap<QString, double> categoryMap;

    for (int i = 0;
         i < ui->tableWidget->rowCount();
         i++)
    {
        QString type =
            ui->tableWidget
                ->item(i, 1)
                ->text();

        QString category =
            ui->tableWidget
                ->item(i, 2)
                ->text();

        double amount =
            ui->tableWidget
                ->item(i, 3)
                ->data(Qt::UserRole)
                .toDouble();

        if (type == "Income")
        {
            income += amount;
        }
        else
        {
            expense += amount;
            categoryMap[category] += amount;
        }
    }

    // SAPAAN
    if (question == "hai"
        || question == "halo"
        || question == "hi"
        || question == "hello")
    {
        return
            "Hai juga 👋\n"
            "Apa yang bisa saya bantu?";
    }

    // TERIMA KASIH
    if (question.contains("terima kasih")
        || question.contains("makasih")
        || question.contains("thanks"))
    {
        return
            "Sama-sama 😊";
    }


    // JAWABAN TIDAK
    if (question == "tidak"
        || question == "ga"
        || question == "tidak ada"
        || question == "gak"
        || question == "gak jadi"
        || question == "ga jadi"
        || question == "tidak jadi"
        || question == "enggak")
    {
        return
            "Baiklah 😊\n"
            "Jika anda perlu bantuan, "
            "tanya saja kepada saya.";
    }

    // SALDO
    if (question.contains("saldo"))
    {
        double saldo =
            income - expense;

        return
            "Saldo kamu saat ini Rp "
            + QString::number(saldo, 'f', 0);
    }

    // PENGELUARAN
    if (question.contains("pengeluaran"))
    {
        return
            "Total pengeluaran kamu Rp "
            + QString::number(expense, 'f', 0);
    }

    // PEMASUKAN
    if (question.contains("income")
        || question.contains("pemasukan"))
    {
        return
            "Total income kamu Rp "
            + QString::number(income, 'f', 0);
    }

    // BOROS
    if (question.contains("boros")
        || question.contains("terbesar"))
    {
        QString biggest = "-";
        double max = 0;

        for (auto it = categoryMap.begin();
             it != categoryMap.end();
             ++it)
        {
            if (it.value() > max)
            {
                max = it.value();
                biggest = it.key();
            }
        }

        return
            "Kategori paling boros adalah "
            + biggest
            + " sebesar Rp "
            + QString::number(max, 'f', 0);
    }

    // KURS
    if (question.contains("kurs")
        || question.contains("usd"))
    {
        if (currentRate == 0)
        {
            return
                "Klik tombol Cek Kurs USD terlebih dahulu.";
        }

        return
            "1 USD = Rp "
            + QString::number(currentRate, 'f', 0);
    }

    // TIPS
    if (question.contains("tips")
        || question.contains("saran"))
    {
        if (expense > income)
        {
            return
                "Pengeluaran kamu lebih besar dari pemasukan. "
                "Coba kurangi pengeluaran tidak penting.";
        }

        return
            "Keuangan kamu cukup stabil. "
            "Coba mulai menabung atau investasi.";
    }

    // HELP
    if (question.contains("help")
        || question.contains("bantuan"))
    {
        return
            "Kamu bisa bertanya:\n"
            "- saldo\n"
            "- pengeluaran\n"
            "- pemasukan\n"
            "- kategori terbesar\n"
            "- kurs usd\n"
            "- tips keuangan";
    }

    return
        "Maaf, saya belum mengerti pertanyaan itu.";
}