#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H
#include <QDialog>
#include <QList>
#include <QDate>
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
#include "transaction.h"

class ChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartDialog(const QList<Transaction> &transactions,
                         QWidget *parent = nullptr);
    ~ChartDialog();

private slots:
    void onFilterChanged();
    void onFilterModeChanged();

private:
    void setupUI();
    QChartView* buildBarChart(const QList<Transaction> &data);
    QChartView* buildLineChart(const QList<Transaction> &data);
    QChartView* buildPieChart(const QList<Transaction> &data);
    QList<Transaction> applyFilter();

    QList<Transaction> allTransactions;
    QTabWidget   *tabWidget;
    QComboBox    *comboMonth;
    QComboBox    *comboYear;
    QDateEdit    *dateFrom;
    QDateEdit    *dateTo;
    QRadioButton *radioMonth;
    QRadioButton *radioYear;
    QRadioButton *radioRange;
    QPushButton  *btnApply;
    QChartView   *barChartView;
    QChartView   *lineChartView;
    QChartView   *pieChartView;

    // Label untuk visibility control
    QLabel *labelMonth;
    QLabel *labelYear;
    QLabel *labelFrom;
    QLabel *labelTo;
};

#endif // CHARTDIALOG_H