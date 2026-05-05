#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "chartdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addTransaction();
    void DeleteTransaction();
    void showChart();           // ← sekarang buka ChartDialog

    // API SLOT
    void getExchangeRate();
    void onApiResult(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;

    void updateBalance();
    void saveData();
    void loadData();

    // ← TAMBAHAN: helper ambil semua transaksi sebagai list
    QList<Transaction> getAllTransactions();
};

#endif // MAINWINDOW_H