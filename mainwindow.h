#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextToSpeech>
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
    ~MainWindow();

private slots:

    // TRANSACTION
    void addTransaction();
    void DeleteTransaction();

    // CHART
    void showChart();

    // API
    void getExchangeRate();
    void onApiResult(QNetworkReply *reply);

    // AI SUMMARY
    void showAISummary();

    // AI CHATBOT
    void processAIChat();

private:

    Ui::MainWindow *ui;

    // NETWORK
    QNetworkAccessManager *networkManager;

    // VOICE AI
    QTextToSpeech *speaker;

    // BALANCE
    void updateBalance();

    // FILE
    void saveData();
    void loadData();

    // AI FEATURE
    QString generateAISummary();
    QString autoCorrectCategory(QString text);

    // CHATBOT
    QString getAIResponse(QString question);

    // VOICE
    void speak(QString text);

    // DATA
    QList<Transaction> getAllTransactions();
};

#endif // MAINWINDOW_H