#include "chartdialog.h"
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QMap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QtCharts/QDateTimeAxis>
#include <QSet>
#include <algorithm>

ChartDialog::ChartDialog(const QList<Transaction> &transactions, QWidget *parent)
    : QDialog(parent), allTransactions(transactions)
{
    setWindowTitle("📊 Laporan Keuangan");
    setMinimumSize(800, 600);
    setupUI();
    onFilterChanged(); // render awal
}

ChartDialog::~ChartDialog() {}

// ─── UI SETUP ────────────────────────────────────────────────────────────────
void ChartDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    setupFilterPanel();

    // Tambah filter panel ke layout utama
    QGroupBox *filterBox = new QGroupBox("🔍 Filter Tanggal");
    QVBoxLayout *filterBoxLayout = new QVBoxLayout(filterBox);

    // Radio buttons
    QHBoxLayout *radioLayout = new QHBoxLayout();
    radioMonth = new QRadioButton("Per Bulan");
    radioYear  = new QRadioButton("Per Tahun");
    radioRange = new QRadioButton("Range Custom");
    radioMonth->setChecked(true);
    radioLayout->addWidget(radioMonth);
    radioLayout->addWidget(radioYear);
    radioLayout->addWidget(radioRange);
    radioLayout->addStretch();
    filterBoxLayout->addLayout(radioLayout);

    // Filter controls
    QHBoxLayout *controlLayout = new QHBoxLayout();

    comboMonth = new QComboBox();
    QStringList months = {"Januari","Februari","Maret","April","Mei","Juni",
                          "Juli","Agustus","September","Oktober","November","Desember"};
    comboMonth->addItems(months);
    comboMonth->setCurrentIndex(QDate::currentDate().month() - 1);

    comboYear = new QComboBox();
    int currentYear = QDate::currentDate().year();
    for (int y = currentYear - 5; y <= currentYear + 1; y++)
        comboYear->addItem(QString::number(y));
    comboYear->setCurrentText(QString::number(currentYear));

    dateFrom = new QDateEdit(QDate::currentDate().addMonths(-1));
    dateFrom->setCalendarPopup(true);
    dateFrom->setDisplayFormat("dd/MM/yyyy");

    dateTo = new QDateEdit(QDate::currentDate());
    dateTo->setCalendarPopup(true);
    dateTo->setDisplayFormat("dd/MM/yyyy");

    btnApply = new QPushButton("Terapkan Filter");
    btnApply->setStyleSheet("background-color: #2196F3; color: white; padding: 5px 15px;");

    controlLayout->addWidget(new QLabel("Bulan:"));
    controlLayout->addWidget(comboMonth);
    controlLayout->addWidget(new QLabel("Tahun:"));
    controlLayout->addWidget(comboYear);
    controlLayout->addWidget(new QLabel("Dari:"));
    controlLayout->addWidget(dateFrom);
    controlLayout->addWidget(new QLabel("Sampai:"));
    controlLayout->addWidget(dateTo);
    controlLayout->addWidget(btnApply);
    controlLayout->addStretch();
    filterBoxLayout->addLayout(controlLayout);

    mainLayout->addWidget(filterBox);

    // Tab widget untuk chart
    tabWidget = new QTabWidget();
    barChartView  = new QChartView();
    lineChartView = new QChartView();
    pieChartView  = new QChartView();
    tabWidget->addTab(barChartView,  "📊 Bar Chart");
    tabWidget->addTab(lineChartView, "📈 Line Chart");
    tabWidget->addTab(pieChartView,  "🥧 Pie Chart");
    mainLayout->addWidget(tabWidget);

    // Connections
    connect(btnApply, &QPushButton::clicked, this, &ChartDialog::onFilterChanged);
    connect(radioMonth, &QRadioButton::toggled, this, &ChartDialog::onFilterModeChanged);
    connect(radioYear,  &QRadioButton::toggled, this, &ChartDialog::onFilterModeChanged);
    connect(radioRange, &QRadioButton::toggled, this, &ChartDialog::onFilterModeChanged);

    onFilterModeChanged(); // set initial visibility
}

void ChartDialog::setupFilterPanel() {}

// ─── FILTER MODE ─────────────────────────────────────────────────────────────
void ChartDialog::onFilterModeChanged()
{
    bool isMon   = radioMonth->isChecked();
    bool isYear  = radioYear->isChecked();
    bool isRange = radioRange->isChecked();

    comboMonth->setVisible(isMon);
    comboYear->setVisible(isMon || isYear);

    // Label visibility
    dateFrom->setVisible(isRange);
    dateTo->setVisible(isRange);
}

// ─── APPLY FILTER ────────────────────────────────────────────────────────────
QList<Transaction> ChartDialog::applyFilter()
{
    QList<Transaction> result;

    if (radioMonth->isChecked()) {
        int month = comboMonth->currentIndex() + 1;
        int year  = comboYear->currentText().toInt();
        for (const auto &t : allTransactions)
            if (t.date.month() == month && t.date.year() == year)
                result.append(t);
    }
    else if (radioYear->isChecked()) {
        int year = comboYear->currentText().toInt();
        for (const auto &t : allTransactions)
            if (t.date.year() == year)
                result.append(t);
    }
    else { // range custom
        QDate from = dateFrom->date();
        QDate to   = dateTo->date();
        for (const auto &t : allTransactions)
            if (t.date >= from && t.date <= to)
                result.append(t);
    }

    return result;
}

// ─── REFRESH CHARTS ──────────────────────────────────────────────────────────
void ChartDialog::onFilterChanged()
{
    QList<Transaction> filtered = applyFilter();

    // Rebuild semua chart
    QChartView *newBar  = buildBarChart(filtered);
    QChartView *newLine = buildLineChart(filtered);
    QChartView *newPie  = buildPieChart(filtered);

    tabWidget->removeTab(2);
    tabWidget->removeTab(1);
    tabWidget->removeTab(0);

    tabWidget->addTab(newBar,  "📊 Bar Chart");
    tabWidget->addTab(newLine, "📈 Line Chart");
    tabWidget->addTab(newPie,  "🥧 Pie Chart");

    barChartView  = newBar;
    lineChartView = newLine;
    pieChartView  = newPie;
}

// ─── BAR CHART ───────────────────────────────────────────────────────────────
QChartView* ChartDialog::buildBarChart(const QList<Transaction> &data)
{
    // Kumpulkan income & expense per bulan
    QMap<QString, double> incomeMap, expenseMap;

    for (const auto &t : data) {
        QString key = t.date.toString("MM/yyyy");
        if (t.type == "Income")
            incomeMap[key] += t.amount;
        else
            expenseMap[key] += t.amount;
    }

    // Gabungkan semua bulan unik
    QStringList categories;
    QSet<QString> keys;
    for (auto &k : incomeMap.keys())  keys.insert(k);
    for (auto &k : expenseMap.keys()) keys.insert(k);
    categories = QStringList(keys.begin(), keys.end());
    std::sort(categories.begin(), categories.end());

    QBarSet *setIncome  = new QBarSet("Income");
    QBarSet *setExpense = new QBarSet("Expense");
    setIncome->setColor(QColor("#4CAF50"));
    setExpense->setColor(QColor("#f44336"));

    for (const QString &cat : categories) {
        *setIncome  << incomeMap.value(cat, 0);
        *setExpense << expenseMap.value(cat, 0);
    }

    QBarSeries *series = new QBarSeries();
    series->append(setIncome);
    series->append(setExpense);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Income vs Expense per Bulan");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Rp");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);

    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}

// ─── LINE CHART ──────────────────────────────────────────────────────────────
QChartView* ChartDialog::buildLineChart(const QList<Transaction> &data)
{
    // Sort data by date
    QList<Transaction> sorted = data;
    std::sort(sorted.begin(), sorted.end(), [](const Transaction &a, const Transaction &b){
        return a.date < b.date;
    });

    // Hitung running balance
    QLineSeries *series = new QLineSeries();
    series->setName("Saldo");
    series->setColor(QColor("#2196F3"));

    double balance = 0;
    QMap<QDate, double> dailyNet;

    for (const auto &t : sorted) {
        if (t.type == "Income")
            dailyNet[t.date] += t.amount;
        else
            dailyNet[t.date] -= t.amount;
    }

    QList<QDate> dates = dailyNet.keys();
    std::sort(dates.begin(), dates.end());

    for (const QDate &d : dates) {
        balance += dailyNet[d];
        series->append(d.startOfDay().toMSecsSinceEpoch(), balance);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Tren Saldo dari Waktu ke Waktu");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("dd/MM/yy");
    axisX->setTitleText("Tanggal");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Saldo (Rp)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}

// ─── PIE CHART ───────────────────────────────────────────────────────────────
QChartView* ChartDialog::buildPieChart(const QList<Transaction> &data)
{
    QMap<QString, double> categoryTotal;

    for (const auto &t : data) {
        if (t.type == "Expense")
            categoryTotal[t.category] += t.amount;
    }

    QPieSeries *series = new QPieSeries();
    for (auto it = categoryTotal.begin(); it != categoryTotal.end(); ++it)
        series->append(it.key(), it.value());

    for (auto slice : series->slices())
        slice->setLabelVisible();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pengeluaran per Kategori");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}