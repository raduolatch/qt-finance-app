#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H
#include <QDialog>
#include <QList>
#include <QStringList>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QDateEdit>
#include <QComboBox>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>

struct Transaction {
    QDate date;
    QString type;     // "Income" / "Expense"
    QString category;
    double amount;
};

class ChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartDialog(const QList<Transaction> &transactions, QWidget *parent = nullptr);
    ~ChartDialog();

private slots:
    void onFilterChanged();
    void onFilterModeChanged();

private:
    void setupUI();
    void setupFilterPanel();
    void setupCharts();

    // Chart builders
    QChartView* buildBarChart(const QList<Transaction> &data);
    QChartView* buildLineChart(const QList<Transaction> &data);
    QChartView* buildPieChart(const QList<Transaction> &data);

    // Filter
    QList<Transaction> applyFilter();

    // Data
    QList<Transaction> allTransactions;

    // UI Elements
    QTabWidget      *tabWidget;
    QComboBox       *comboMonth;
    QComboBox       *comboYear;
    QDateEdit       *dateFrom;
    QDateEdit       *dateTo;
    QRadioButton    *radioMonth;
    QRadioButton    *radioYear;
    QRadioButton    *radioRange;
    QPushButton     *btnApply;

    // Chart views (refreshable)
    QChartView      *barChartView;
    QChartView      *lineChartView;
    QChartView      *pieChartView;
};

#endif // CHARTDIALOG_H