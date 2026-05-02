#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
    void addTransaction();       // tombol Add
    void DeleteTransaction();    // tombol Delete
    void showChart();            // tombol Chart

    //API SLOT
    void getExchangeRate();
    void onApiResult(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

    //API MANAGER
    QNetworkAccessManager *networkManager;

    // fungsi helper
    void updateBalance();
    void saveData();
    void loadData();
};

#endif // MAINWINDOW_H