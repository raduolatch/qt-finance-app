/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QComboBox *comboType;
    QLineEdit *inputAmount;
    QLineEdit *inputCategory;
    QPushButton *btnAdd;
    QLabel *labelBalance;
    QTableWidget *tableWidget;
    QPushButton *btnDelete;
    QPushButton *btnChart;
    QDateEdit *inputDate;
    QPushButton *btnAPI;
    QLabel *labelKurs;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        comboType = new QComboBox(centralwidget);
        comboType->addItem(QString());
        comboType->addItem(QString());
        comboType->setObjectName("comboType");
        comboType->setGeometry(QRect(130, 70, 151, 41));
        comboType->setMaxVisibleItems(10);
        inputAmount = new QLineEdit(centralwidget);
        inputAmount->setObjectName("inputAmount");
        inputAmount->setGeometry(QRect(430, 80, 131, 31));
        inputCategory = new QLineEdit(centralwidget);
        inputCategory->setObjectName("inputCategory");
        inputCategory->setGeometry(QRect(290, 80, 131, 31));
        btnAdd = new QPushButton(centralwidget);
        btnAdd->setObjectName("btnAdd");
        btnAdd->setGeometry(QRect(570, 80, 121, 31));
        labelBalance = new QLabel(centralwidget);
        labelBalance->setObjectName("labelBalance");
        labelBalance->setGeometry(QRect(80, 460, 181, 31));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(85, 120, 581, 281));
        tableWidget->setColumnCount(4);
        btnDelete = new QPushButton(centralwidget);
        btnDelete->setObjectName("btnDelete");
        btnDelete->setGeometry(QRect(700, 80, 91, 31));
        btnChart = new QPushButton(centralwidget);
        btnChart->setObjectName("btnChart");
        btnChart->setGeometry(QRect(130, 410, 211, 51));
        inputDate = new QDateEdit(centralwidget);
        inputDate->setObjectName("inputDate");
        inputDate->setGeometry(QRect(0, 70, 121, 41));
        btnAPI = new QPushButton(centralwidget);
        btnAPI->setObjectName("btnAPI");
        btnAPI->setGeometry(QRect(430, 410, 191, 51));
        labelKurs = new QLabel(centralwidget);
        labelKurs->setObjectName("labelKurs");
        labelKurs->setGeometry(QRect(80, 490, 171, 31));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        comboType->setItemText(0, QCoreApplication::translate("MainWindow", "Income", nullptr));
        comboType->setItemText(1, QCoreApplication::translate("MainWindow", "Expense", nullptr));

        inputCategory->setText(QString());
        btnAdd->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        labelBalance->setText(QCoreApplication::translate("MainWindow", "Total Saldo: Rp 0", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Amount", nullptr));
        btnDelete->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        btnChart->setText(QCoreApplication::translate("MainWindow", "Show Chart", nullptr));
        btnAPI->setText(QCoreApplication::translate("MainWindow", "Cek Kurs USD", nullptr));
        labelKurs->setText(QCoreApplication::translate("MainWindow", "Kurs: -", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
