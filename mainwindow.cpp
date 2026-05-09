#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartdialog.h"
#include "database.h"
#include <QValidator>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QMap>
#include <QDate>
#include <QLocale>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QAbstractItemView>
#include <QProcess>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QFile>

// Kurs global
double currentRate = 0;

// CONSTRUCTOR
MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUser(username)
{
    ui->setupUi(this);
    setWindowTitle("Finance App - " + currentUser);

    // TEXT TO SPEECH
    speaker = new QTextToSpeech(this);
    speaker->setLocale(QLocale(QLocale::Indonesian));
    speaker->setRate(0);

    // NETWORK MANAGER
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::onApiResult);

    // VALIDATOR: angka, titik ribuan, dan koma desimal
    QRegularExpression rx("[0-9.,]*");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->inputAmount->setValidator(validator);

    // DATE default hari ini
    ui->inputDate->setDate(QDate::currentDate());

    // TABLE settings
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnWidth(0, 120);
    ui->tableWidget->setColumnWidth(1, 120);
    ui->tableWidget->setColumnWidth(2, 220);
    ui->tableWidget->setColumnWidth(3, 180);

    // STYLE BUTTON
    ui->btnAdd->setStyleSheet(
        "background-color:#4CAF50;color:white;font-weight:bold;border-radius:6px;");
    ui->btnDelete->setStyleSheet(
        "background-color:#f44336;color:white;font-weight:bold;border-radius:6px;");
    ui->btnChart->setStyleSheet(
        "background-color:#2196F3;color:white;font-weight:bold;border-radius:6px;");
    ui->btnAPI->setStyleSheet(
        "background-color:#FF9800;color:white;font-weight:bold;border-radius:6px;");
    ui->btnAI->setStyleSheet(
        "background-color:#9C27B0;color:white;font-weight:bold;border-radius:6px;");
    ui->btnChat->setStyleSheet(
        "background-color:#009688;color:white;font-weight:bold;border-radius:6px;");

    // CONNECT SIGNALS & SLOTS
    connect(ui->btnAdd,    &QPushButton::clicked, this, &MainWindow::addTransaction);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::DeleteTransaction);
    connect(ui->btnChart,  &QPushButton::clicked, this, &MainWindow::showChart);
    connect(ui->btnAPI,    &QPushButton::clicked, this, &MainWindow::getExchangeRate);
    connect(ui->btnAI,     &QPushButton::clicked, this, &MainWindow::showAISummary);
    connect(ui->btnChat,   &QPushButton::clicked, this, &MainWindow::processAIChat);
    connect(ui->btnVoice,  &QPushButton::clicked, this, &MainWindow::processVoice);

    // Enter di inputChat langsung kirim
    connect(ui->inputChat, &QLineEdit::returnPressed,
            this, &MainWindow::processAIChat);

    loadData();
}

// DESTRUCTOR
MainWindow::~MainWindow()
{
    delete ui;
}

// UPDATE BALANCE
void MainWindow::updateBalance()
{
    double total = 0;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QString type   = ui->tableWidget->item(i, 1)->text();
        double  amount = ui->tableWidget->item(i, 3)
                            ->data(Qt::UserRole).toDouble();

        if (type == "Income")
            total += amount;
        else
            total -= amount;
    }

    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);
    ui->labelBalance->setText(
        "Total Saldo: Rp " + locale.toString((qlonglong)total));

    // Warna saldo
    if (total >= 0)
        ui->labelBalance->setStyleSheet(
            "font-size:14pt;font-weight:bold;color:#2e7d32;");
    else
        ui->labelBalance->setStyleSheet(
            "font-size:14pt;font-weight:bold;color:#c62828;");
}


// ADD TRANSACTION
void MainWindow::addTransaction()
{
    QString date     = ui->inputDate->date().toString("yyyy-MM-dd");
    QString type     = ui->comboType->currentText();
    QString category = ui->inputCategory->text().trimmed();
    QString amtText  = ui->inputAmount->text().trimmed();

    if (category.isEmpty() || amtText.isEmpty())
    {
        QMessageBox::warning(this, "Input Error",
                             "Kategori dan jumlah wajib diisi!");
        return;
    }

    // Hapus pemisah ribuan (titik), ganti koma desimal jadi titik
    QString cleanAmt = amtText;
    cleanAmt.remove('.');       // 5.000.000 → 5000000
    cleanAmt.replace(',', '.'); // 5000,50  → 5000.50

    bool ok;
    double amount = cleanAmt.toDouble(&ok);
    if (!ok || amount <= 0)
    {
        QMessageBox::warning(this, "Input Error",
                             "Jumlah harus berupa angka positif!\n"
                             "Contoh: 5000000 atau 5.000.000");
        return;
    }

    // Auto-correct kategori
    category = autoCorrectCategory(category);

    // Simpan ke SQLite
    QSqlQuery query;
    query.prepare(
        "INSERT INTO transactions (username, date, type, category, amount) "
        "VALUES (:username, :date, :type, :category, :amount)");
    query.bindValue(":username", currentUser);
    query.bindValue(":date",     date);
    query.bindValue(":type",     type);
    query.bindValue(":category", category);
    query.bindValue(":amount",   amount);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Database Error",
                             query.lastError().text());
        return;
    }

    int dbId = query.lastInsertId().toInt();

    // Tambah ke tabel UI
    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);
    QString formatted = "Rp " + locale.toString((qlonglong)amount);

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(date));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(category));

    QTableWidgetItem *amtItem = new QTableWidgetItem(formatted);
    amtItem->setData(Qt::UserRole,     amount); // nilai asli
    amtItem->setData(Qt::UserRole + 1, dbId);   // id database
    amtItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, 3, amtItem);

    // Reset input
    ui->inputCategory->clear();
    ui->inputAmount->clear();
    ui->inputDate->setDate(QDate::currentDate());

    updateBalance();
    statusBar()->showMessage("Transaksi berhasil ditambahkan!", 3000);
}


// DELETE TRANSACTION
void MainWindow::DeleteTransaction()
{
    int row = ui->tableWidget->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(this, "Pilih Baris",
                             "Pilih transaksi yang ingin dihapus!");
        return;
    }

    auto reply = QMessageBox::question(
        this, "Konfirmasi", "Hapus transaksi ini?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    // Ambil id dari database
    int id = ui->tableWidget->item(row, 3)
                 ->data(Qt::UserRole + 1).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM transactions WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Database Error",
                             query.lastError().text());
        return;
    }

    ui->tableWidget->removeRow(row);
    updateBalance();
    statusBar()->showMessage("Transaksi berhasil dihapus!", 3000);
}

// SHOW CHART
void MainWindow::showChart()
{
    if (ui->tableWidget->rowCount() == 0)
    {
        QMessageBox::information(this, "Info",
                                 "Belum ada data transaksi!");
        return;
    }

    QList<Transaction> list;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        Transaction t;
        t.date     = QDate::fromString(
            ui->tableWidget->item(i, 0)->text(), "yyyy-MM-dd");
        t.type     = ui->tableWidget->item(i, 1)->text();
        t.category = ui->tableWidget->item(i, 2)->text();
        t.amount   = ui->tableWidget->item(i, 3)
                       ->data(Qt::UserRole).toDouble();
        list.append(t);
    }

    ChartDialog dialog(list, this);
    dialog.exec();
}


// GET EXCHANGE RATE
void MainWindow::getExchangeRate()
{
    ui->labelKurs->setText("Kurs: Memuat...");
    ui->btnAPI->setEnabled(false);

    QUrl url("https://open.er-api.com/v6/latest/USD");
    networkManager->get(QNetworkRequest(url));
}

void MainWindow::onApiResult(QNetworkReply *reply)
{
    ui->btnAPI->setEnabled(true);

    if (reply->error() != QNetworkReply::NoError)
    {
        ui->labelKurs->setText("Kurs: Gagal memuat");
        QMessageBox::warning(this, "Network Error", reply->errorString());
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject   obj = doc.object();

    if (obj.contains("rates"))
    {
        currentRate = obj["rates"].toObject()["IDR"].toDouble();
        QLocale locale(QLocale::Indonesian, QLocale::Indonesia);
        ui->labelKurs->setText(
            "Kurs: 1 USD = Rp " + locale.toString((qlonglong)currentRate));
    }
    else
    {
        ui->labelKurs->setText("Kurs: Data tidak tersedia");
    }

    reply->deleteLater();
}


// AI SUMMARY
void MainWindow::showAISummary()
{
    if (ui->tableWidget->rowCount() == 0)
    {
        QMessageBox::information(this, "Info",
                                 "Belum ada data transaksi!");
        return;
    }

    double totalIncome  = 0;
    double totalExpense = 0;
    QMap<QString, double> categoryMap;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QString type   = ui->tableWidget->item(i, 1)->text();
        QString cat    = ui->tableWidget->item(i, 2)->text();
        double  amount = ui->tableWidget->item(i, 3)
                            ->data(Qt::UserRole).toDouble();

        if (type == "Income")
            totalIncome += amount;
        else
        {
            totalExpense += amount;
            categoryMap[cat] += amount;
        }
    }

    double saldo = totalIncome - totalExpense;

    // Cari kategori pengeluaran terbesar
    QString topCat;
    double  topAmt = 0;
    for (auto it = categoryMap.begin(); it != categoryMap.end(); ++it)
        if (it.value() > topAmt) { topAmt = it.value(); topCat = it.key(); }

    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);

    QString summary =
        "📊 Ringkasan Keuangan Anda:\n\n"
        "✅ Total Pemasukan : Rp " + locale.toString((qlonglong)totalIncome)  + "\n"
                                                     "❌ Total Pengeluaran: Rp " + locale.toString((qlonglong)totalExpense) + "\n"
                                                      "💰 Saldo Bersih    : Rp " + locale.toString((qlonglong)saldo)        + "\n\n";

    if (!topCat.isEmpty())
        summary += "🏷️  Pengeluaran terbesar: " + topCat +
                   " (Rp " + locale.toString((qlonglong)topAmt) + ")\n\n";

    if (saldo > 0)
        summary += "💡 Keuangan Anda sehat! Pertahankan pola ini.";
    else if (saldo == 0)
        summary += "⚠️  Pengeluaran sama dengan pemasukan. Coba hemat lebih!";
    else
        summary += "🚨 Pengeluaran melebihi pemasukan! Segera evaluasi keuangan Anda.";

    QMessageBox::information(this, "AI Summary", summary);

    // TTS ringkasan
    if (speaker->state() != QTextToSpeech::Speaking)
    {
        QString tts = "Saldo bersih Anda adalah " +
                      locale.toString((qlonglong)saldo) + " rupiah. ";
        tts += (saldo < 0) ? "Pengeluaran Anda melebihi pemasukan!"
                           : "Keuangan Anda dalam kondisi sehat.";
        speaker->say(tts);
    }
}

// AI CHAT
void MainWindow::processAIChat()
{
    QString input = ui->inputChat->text().trimmed();
    if (input.isEmpty())
        return;

    ui->inputChat->clear();

    double totalIncome  = 0;
    double totalExpense = 0;
    QMap<QString, double> categoryMap;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QString type   = ui->tableWidget->item(i, 1)->text();
        QString cat    = ui->tableWidget->item(i, 2)->text();
        double  amount = ui->tableWidget->item(i, 3)
                            ->data(Qt::UserRole).toDouble();

        if (type == "Income")
            totalIncome += amount;
        else
        {
            totalExpense += amount;
            categoryMap[cat] += amount;
        }
    }

    double  saldo  = totalIncome - totalExpense;
    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);
    QString lower  = input.toLower();
    QString response;

    if (lower.contains("saldo") || lower.contains("balance"))
        response = "💰 Saldo Anda: Rp " + locale.toString((qlonglong)saldo);

    else if (lower.contains("pemasukan") || lower.contains("income"))
        response = "✅ Total pemasukan: Rp " + locale.toString((qlonglong)totalIncome);

    else if (lower.contains("pengeluaran") || lower.contains("expense"))
        response = "❌ Total pengeluaran: Rp " + locale.toString((qlonglong)totalExpense);

    else if (lower.contains("kurs") || lower.contains("usd") || lower.contains("dolar"))
        response = (currentRate > 0)
                       ? "💱 1 USD = Rp " + locale.toString((qlonglong)currentRate)
                       : "💱 Tekan 'Cek Kurs USD' untuk mendapatkan kurs terkini.";

    else if (lower.contains("terbesar") || lower.contains("terbanyak"))
    {
        if (!categoryMap.isEmpty())
        {
            QString topCat; double topAmt = 0;
            for (auto it = categoryMap.begin(); it != categoryMap.end(); ++it)
                if (it.value() > topAmt) { topAmt = it.value(); topCat = it.key(); }
            response = "🏷️  Pengeluaran terbesar: " + topCat +
                       " (Rp " + locale.toString((qlonglong)topAmt) + ")";
        }
        else
            response = "Belum ada data pengeluaran.";
    }
    else if (lower.contains("saran") || lower.contains("tips"))
    {
        if (saldo > totalIncome * 0.3)
            response = "💡 Anda menghemat >30% pemasukan. Pertimbangkan investasi!";
        else if (saldo > 0)
            response = "💡 Keuangan positif. Tingkatkan tabungan ke 20-30% pemasukan.";
        else
            response = "🚨 Pengeluaran melebihi pemasukan. Kurangi pengeluaran non-esensial!";
    }
    else if (lower.contains("halo") || lower.contains("hai") || lower.contains("hello"))
        response = "👋 Halo " + currentUser + "! Ada yang bisa saya bantu?";

    else
        response = "🤖 Saya bisa menjawab: saldo, pemasukan, pengeluaran, "
                   "kurs, pengeluaran terbesar, atau saran keuangan.";

    ui->chatOutput->append("🧑 " + input);
    ui->chatOutput->append("🤖 " + response);
    ui->chatOutput->append("---");

    // bacakan respon chatbot
    if (speaker->state() != QTextToSpeech::Speaking)
        speaker->say(response);
}

// VOICE (Speech Recognition via Python)
void MainWindow::processVoice()
{
    if (speaker->state() == QTextToSpeech::Speaking)
    {
        speaker->stop();
        statusBar()->showMessage("Voice dihentikan.", 2000);
        return;
    }

    statusBar()->showMessage("🎤 Mendengarkan... (tunggu hingga 10 detik)", 12000);
    ui->btnVoice->setEnabled(false);

    // Cari speech.py di berbagai lokasi
    QString scriptPath;
    QStringList candidates = {
        QCoreApplication::applicationDirPath() + "/speech.py",
        QDir::currentPath() + "/speech.py",
        QString::fromUtf8(qgetenv("PWD")) + "/speech.py",
        // cari di folder source project langsung
        QString("C:/apkproject/FinanceApp/speech.py"),
    };
    for (const QString &path : candidates)
        if (QFile::exists(path)) { scriptPath = path; break; }

    if (scriptPath.isEmpty())
    {
        ui->btnVoice->setEnabled(true);
        statusBar()->showMessage("❌ File speech.py tidak ditemukan!", 4000);
        QMessageBox::warning(this, "Error",
                             "File speech.py tidak ditemukan!\n"
                             "Pastikan speech.py ada di folder yang sama dengan executable.");
        return;
    }

    qDebug() << "Menjalankan speech.py dari:" << scriptPath;

    //Langsung pakai "python" karena Windows tidak kenal "python3"
    QString pythonCmd = "python";

    QProcess *process = new QProcess(this);

    connect(process, &QProcess::finished, this,
            [this, process](int exitCode, QProcess::ExitStatus)
            {
                ui->btnVoice->setEnabled(true);

                QString output = process->readAllStandardOutput().trimmed();
                QString errOut = process->readAllStandardError().trimmed();

                qDebug() << "Speech stdout:" << output;
                qDebug() << "Speech stderr:" << errOut;
                qDebug() << "Exit code:"     << exitCode;

                if (output.startsWith("RESULT:"))
                {
                    QString text = output.mid(7);
                    if (!text.startsWith("ERROR:"))
                    {
                        ui->inputChat->setText(text);
                        processAIChat();
                        statusBar()->showMessage("✅ Suara dikenali: " + text, 3000);
                    }
                    else
                    {
                        QString errMsg = text.mid(6);
                        statusBar()->showMessage("❌ " + errMsg, 4000);
                        ui->chatOutput->append("🎤 Gagal: " + errMsg);
                    }
                }
                else
                {
                    QString dbg = errOut.isEmpty() ? output : errOut;
                    statusBar()->showMessage("❌ Error: " + dbg.left(60), 5000);
                    qDebug() << "Output tidak dikenali:" << output;
                }

                process->deleteLater();
            });

    connect(process, &QProcess::errorOccurred, this,
            [this, process](QProcess::ProcessError error)
            {
                ui->btnVoice->setEnabled(true);
                QString msg;
                switch (error) {
                case QProcess::FailedToStart:
                    msg = "Python gagal dijalankan. Cek instalasi Python!"; break;
                case QProcess::Crashed:
                    msg = "Python crash saat berjalan."; break;
                default:
                    msg = "Error tidak diketahui saat menjalankan Python.";
                }
                statusBar()->showMessage("❌ " + msg, 5000);
                QMessageBox::warning(this, "Voice Error", msg);
                process->deleteLater();
            });

    process->start(pythonCmd, QStringList() << scriptPath);
}


// SAVE DATA
void MainWindow::saveData() {}


// LOAD DATA
void MainWindow::loadData()
{
    ui->tableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare(
        "SELECT id, date, type, category, amount "
        "FROM transactions WHERE username = :username "
        "ORDER BY date ASC");
    query.bindValue(":username", currentUser);

    if (!query.exec())
    {
        qDebug() << "Load data error:" << query.lastError().text();
        return;
    }

    QLocale locale(QLocale::Indonesian, QLocale::Indonesia);

    while (query.next())
    {
        int     id       = query.value("id").toInt();
        QString date     = query.value("date").toString();
        QString type     = query.value("type").toString();
        QString category = query.value("category").toString();
        double  amount   = query.value("amount").toDouble();

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(date));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(category));

        QString formatted = "Rp " + locale.toString((qlonglong)amount);
        QTableWidgetItem *amtItem = new QTableWidgetItem(formatted);
        amtItem->setData(Qt::UserRole,     amount);
        amtItem->setData(Qt::UserRole + 1, id);
        amtItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 3, amtItem);
    }

    updateBalance();
}


// AUTO CORRECT CATEGORY
QString MainWindow::autoCorrectCategory(QString text)
{
    QMap<QString, QString> corrections =
        {
         {"makan",     "Makanan"},
         {"minum",     "Makanan"},
         {"mkn",       "Makanan"},
         {"food",      "Makanan"},
         {"transport", "Transportasi"},
         {"bensin",    "Transportasi"},
         {"ojek",      "Transportasi"},
         {"grab",      "Transportasi"},
         {"gaji",      "Gaji"},
         {"salary",    "Gaji"},
         {"belanja",   "Belanja"},
         {"shopping",  "Belanja"},
         {"listrik",   "Tagihan"},
         {"air",       "Tagihan"},
         {"internet",  "Tagihan"},
         {"hiburan",   "Hiburan"},
         {"game",      "Hiburan"},
         {"nonton",    "Hiburan"},
         {"obat",      "Kesehatan"},
         {"dokter",    "Kesehatan"},
         };

    QString lower = text.toLower();
    for (auto it = corrections.begin(); it != corrections.end(); ++it)
        if (lower.contains(it.key()))
            return it.value();

    // Capitalize huruf pertama
    if (!text.isEmpty())
        text[0] = text[0].toUpper();

    return text;
}