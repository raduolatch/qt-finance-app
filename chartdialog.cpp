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
#include <cmath>

ChartDialog::ChartDialog(const QList<Transaction> &transactions, QWidget *parent)
    : QDialog(parent), allTransactions(transactions)
{
    setWindowTitle(" Laporan Keuangan");
    setMinimumSize(800, 600);
    setupUI();
    onFilterChanged(); // render awal
}

ChartDialog::~ChartDialog() {}

// UI SETUP
void ChartDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    setupFilterPanel();

    // Tambah filter panel ke layout utama
    QGroupBox *filterBox = new QGroupBox(" Filter Tanggal");
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
    tabWidget->addTab(barChartView,  " Bar Chart");
    tabWidget->addTab(lineChartView, " Line Chart");
    tabWidget->addTab(pieChartView,  " Pie Chart");
    mainLayout->addWidget(tabWidget);

    // Connections
    connect(btnApply, &QPushButton::clicked, this, &ChartDialog::onFilterChanged);
    connect(radioMonth, &QRadioButton::toggled, this, &ChartDialog::onFilterModeChanged);
    connect(radioYear,  &QRadioButton::toggled, this, &ChartDialog::onFilterModeChanged);
    connect(radioRange, &QRadioButton::toggled, this, &ChartDialog::onFilterModeChanged);

    onFilterModeChanged(); // set initial visibility
}

void ChartDialog::setupFilterPanel() {}

//  FILTER MODE
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

//  APPLY FILTER
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

//  REFRESH CHARTS
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

    tabWidget->addTab(newBar,  " Bar Chart");
    tabWidget->addTab(newLine, " Line Chart");
    tabWidget->addTab(newPie,  " Pie Chart");

    barChartView  = newBar;
    lineChartView = newLine;
    pieChartView  = newPie;
}

// BAR CHART
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

// LINE CHART
QChartView* ChartDialog::buildLineChart(const QList<Transaction> &data)
{
    QList<Transaction> sorted = data;

    std::sort(sorted.begin(), sorted.end(),
              [](const Transaction &a, const Transaction &b) {
                  return a.date < b.date;
              });

    // SERIES
    QLineSeries *saldoSeries = new QLineSeries();
    saldoSeries->setName("Saldo");
    saldoSeries->setColor(QColor("#2196F3"));

    QLineSeries *expenseSeries = new QLineSeries();
    expenseSeries->setName("Pengeluaran");
    expenseSeries->setColor(QColor("#f44336"));

    double saldo = 0;

    QMap<QDate, double> incomeMap;
    QMap<QDate, double> expenseMap;

    // Kelompokkan transaksi
    for (const auto &t : sorted) {

        if (t.type == "Income")
            incomeMap[t.date] += t.amount;
        else
            expenseMap[t.date] += t.amount;
    }

    // Ambil semua tanggal unik
    QSet<QDate> allDates;

    for (const auto &d : incomeMap.keys())
        allDates.insert(d);

    for (const auto &d : expenseMap.keys())
        allDates.insert(d);

    QList<QDate> dates = allDates.values();

    std::sort(dates.begin(), dates.end());

    // MASUKKAN DATA KE SERIES

    for (const QDate &d : dates) {

        double income  = incomeMap.value(d, 0);
        double expense = expenseMap.value(d, 0);

        saldo += income;
        saldo -= expense;

        qint64 x = d.startOfDay().toMSecsSinceEpoch();

        saldoSeries->append(x, saldo);
        expenseSeries->append(x, expense);
    }

    // CHART
    QChart *chart = new QChart();

    chart->addSeries(saldoSeries);
    chart->addSeries(expenseSeries);

    chart->setTitle("Tren Saldo dan Pengeluaran");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Kalau kosong
    if (saldoSeries->count() == 0) {

        chart->setTitle("Tidak ada data");

        QChartView *emptyView = new QChartView(chart);
        emptyView->setRenderHint(QPainter::Antialiasing);

        return emptyView;
    }

    // Kalau cuma 1 titik
    if (saldoSeries->count() == 1) {

        QPointF p1 = saldoSeries->points().first();
        QPointF p2 = expenseSeries->points().first();

        saldoSeries->append(p1.x() + 86400000, p1.y());
        expenseSeries->append(p2.x() + 86400000, p2.y());
    }

    // AXIS X
    QDateTimeAxis *axisX = new QDateTimeAxis();

    axisX->setFormat("dd MMM");
    axisX->setTitleText("Tanggal");
    axisX->setTickCount(qMin(dates.size(), 7));

    chart->addAxis(axisX, Qt::AlignBottom);

    saldoSeries->attachAxis(axisX);
    expenseSeries->attachAxis(axisX);

    // MIN MAX
    double minY = 0;
    double maxY = 0;

    auto checkSeries = [&](QLineSeries *series)
    {
        for (const QPointF &p : series->points()) {

            if (p.y() < minY)
                minY = p.y();

            if (p.y() > maxY)
                maxY = p.y();
        }
    };

    checkSeries(saldoSeries);
    checkSeries(expenseSeries);


    // KELIPATAN
    double range = maxY - minY;

    if (range <= 10000)
        range = 10000;

    double magnitude =
        pow(10, floor(log10(range)));

    double step = magnitude;

    minY = floor(minY / step) * step;
    maxY = ceil(maxY / step) * step;

    // AXIS Y
    QValueAxis *axisY = new QValueAxis();

    axisY->setTitleText("Jumlah (Rp)");

    axisY->setRange(minY, maxY);

    axisY->setTickCount(6);

    axisY->setLabelFormat("%.0f");

    chart->addAxis(axisY, Qt::AlignLeft);

    saldoSeries->attachAxis(axisY);
    expenseSeries->attachAxis(axisY);

    // STYLE
    saldoSeries->setPointsVisible(true);
    saldoSeries->setMarkerSize(10);

    expenseSeries->setPointsVisible(true);
    expenseSeries->setMarkerSize(10);

    chart->legend()->setVisible(true);

    // Background putih
    chart->setBackgroundBrush(QBrush(Qt::white));

    QChartView *view = new QChartView(chart);

    view->setRenderHint(QPainter::Antialiasing);

    return view;
}

// PIE CHART
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