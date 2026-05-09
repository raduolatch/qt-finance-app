#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextToSpeech>
#include <QList>
#include <QDate>
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTransaction();
    void DeleteTransaction();
    void showChart();
    void getExchangeRate();
    void onApiResult(QNetworkReply *reply);
    void showAISummary();
    void processAIChat();
    void processVoice();

private:
    Ui::MainWindow          *ui;
    QString                  currentUser;
    QNetworkAccessManager   *networkManager;
    QTextToSpeech           *speaker;
    QList<Transaction>       transactions;

    void    updateBalance();
    void    saveData();
    void    loadData();
    QString autoCorrectCategory(QString text);
};

#endif // MAINWINDOW_H