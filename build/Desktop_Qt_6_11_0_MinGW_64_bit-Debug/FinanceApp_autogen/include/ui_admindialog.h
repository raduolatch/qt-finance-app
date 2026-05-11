/********************************************************************************
** Form generated from reading UI file 'admindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINDIALOG_H
#define UI_ADMINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QTabWidget *tabWidget;
    QWidget *tabTransactions;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_2;
    QLineEdit *inputFilter;
    QPushButton *btnFilter;
    QPushButton *btnAllTransactions;
    QTableWidget *tableTransactions;
    QHBoxLayout *hboxLayout1;
    QLabel *labelTransactionCount;
    QSpacerItem *spacerItem;
    QPushButton *btnDeleteTransaction;
    QWidget *tabUsers;
    QVBoxLayout *vboxLayout1;
    QTableWidget *tableUsers;
    QHBoxLayout *hboxLayout2;
    QLabel *labelUserCount;
    QSpacerItem *spacerItem1;
    QPushButton *btnAllUsers;
    QPushButton *btnDeleteUser;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem2;
    QPushButton *btnClose;

    void setupUi(QDialog *AdminDialog)
    {
        if (AdminDialog->objectName().isEmpty())
            AdminDialog->setObjectName("AdminDialog");
        AdminDialog->resize(950, 650);
        verticalLayout = new QVBoxLayout(AdminDialog);
        verticalLayout->setObjectName("verticalLayout");
        labelTitle = new QLabel(AdminDialog);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(labelTitle);

        tabWidget = new QTabWidget(AdminDialog);
        tabWidget->setObjectName("tabWidget");
        tabTransactions = new QWidget();
        tabTransactions->setObjectName("tabTransactions");
        vboxLayout = new QVBoxLayout(tabTransactions);
        vboxLayout->setObjectName("vboxLayout");
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        label_2 = new QLabel(tabTransactions);
        label_2->setObjectName("label_2");

        hboxLayout->addWidget(label_2);

        inputFilter = new QLineEdit(tabTransactions);
        inputFilter->setObjectName("inputFilter");

        hboxLayout->addWidget(inputFilter);

        btnFilter = new QPushButton(tabTransactions);
        btnFilter->setObjectName("btnFilter");

        hboxLayout->addWidget(btnFilter);

        btnAllTransactions = new QPushButton(tabTransactions);
        btnAllTransactions->setObjectName("btnAllTransactions");

        hboxLayout->addWidget(btnAllTransactions);


        vboxLayout->addLayout(hboxLayout);

        tableTransactions = new QTableWidget(tabTransactions);
        tableTransactions->setObjectName("tableTransactions");
        tableTransactions->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableTransactions->setAlternatingRowColors(true);
        tableTransactions->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        vboxLayout->addWidget(tableTransactions);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName("hboxLayout1");
        labelTransactionCount = new QLabel(tabTransactions);
        labelTransactionCount->setObjectName("labelTransactionCount");

        hboxLayout1->addWidget(labelTransactionCount);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout1->addItem(spacerItem);

        btnDeleteTransaction = new QPushButton(tabTransactions);
        btnDeleteTransaction->setObjectName("btnDeleteTransaction");

        hboxLayout1->addWidget(btnDeleteTransaction);


        vboxLayout->addLayout(hboxLayout1);

        tabWidget->addTab(tabTransactions, QString());
        tabUsers = new QWidget();
        tabUsers->setObjectName("tabUsers");
        vboxLayout1 = new QVBoxLayout(tabUsers);
        vboxLayout1->setObjectName("vboxLayout1");
        tableUsers = new QTableWidget(tabUsers);
        tableUsers->setObjectName("tableUsers");
        tableUsers->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableUsers->setAlternatingRowColors(true);
        tableUsers->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        vboxLayout1->addWidget(tableUsers);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName("hboxLayout2");
        labelUserCount = new QLabel(tabUsers);
        labelUserCount->setObjectName("labelUserCount");

        hboxLayout2->addWidget(labelUserCount);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout2->addItem(spacerItem1);

        btnAllUsers = new QPushButton(tabUsers);
        btnAllUsers->setObjectName("btnAllUsers");

        hboxLayout2->addWidget(btnAllUsers);

        btnDeleteUser = new QPushButton(tabUsers);
        btnDeleteUser->setObjectName("btnDeleteUser");

        hboxLayout2->addWidget(btnDeleteUser);


        vboxLayout1->addLayout(hboxLayout2);

        tabWidget->addTab(tabUsers, QString());

        verticalLayout->addWidget(tabWidget);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName("hboxLayout3");
        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout3->addItem(spacerItem2);

        btnClose = new QPushButton(AdminDialog);
        btnClose->setObjectName("btnClose");

        hboxLayout3->addWidget(btnClose);


        verticalLayout->addLayout(hboxLayout3);


        retranslateUi(AdminDialog);
        QObject::connect(btnClose, &QPushButton::clicked, AdminDialog, qOverload<>(&QDialog::close));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AdminDialog);
    } // setupUi

    void retranslateUi(QDialog *AdminDialog)
    {
        AdminDialog->setWindowTitle(QCoreApplication::translate("AdminDialog", "Admin Panel", nullptr));
        labelTitle->setStyleSheet(QCoreApplication::translate("AdminDialog", "font-size:16pt;font-weight:bold;color:#1565C0;padding:8px;", nullptr));
        labelTitle->setText(QCoreApplication::translate("AdminDialog", "\360\237\233\241\357\270\217 Admin Panel", nullptr));
        label_2->setText(QCoreApplication::translate("AdminDialog", "Filter Username:", nullptr));
        inputFilter->setPlaceholderText(QCoreApplication::translate("AdminDialog", "Ketik username untuk filter...", nullptr));
        btnFilter->setStyleSheet(QCoreApplication::translate("AdminDialog", "background-color:#1976D2;color:white;font-weight:bold;border-radius:6px;padding:6px 12px;", nullptr));
        btnFilter->setText(QCoreApplication::translate("AdminDialog", "\360\237\224\215 Filter", nullptr));
        btnAllTransactions->setStyleSheet(QCoreApplication::translate("AdminDialog", "background-color:#388E3C;color:white;font-weight:bold;border-radius:6px;padding:6px 12px;", nullptr));
        btnAllTransactions->setText(QCoreApplication::translate("AdminDialog", "\360\237\224\204 Tampilkan Semua", nullptr));
        labelTransactionCount->setStyleSheet(QCoreApplication::translate("AdminDialog", "font-weight:bold;color:#555;", nullptr));
        labelTransactionCount->setText(QCoreApplication::translate("AdminDialog", "Total: 0 transaksi", nullptr));
        btnDeleteTransaction->setStyleSheet(QCoreApplication::translate("AdminDialog", "background-color:#D32F2F;color:white;font-weight:bold;border-radius:6px;padding:6px 12px;", nullptr));
        btnDeleteTransaction->setText(QCoreApplication::translate("AdminDialog", "\360\237\227\221\357\270\217 Hapus Transaksi", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTransactions), QCoreApplication::translate("AdminDialog", "\360\237\223\212 Semua Transaksi", nullptr));
        labelUserCount->setStyleSheet(QCoreApplication::translate("AdminDialog", "font-weight:bold;color:#555;", nullptr));
        labelUserCount->setText(QCoreApplication::translate("AdminDialog", "Total: 0 user", nullptr));
        btnAllUsers->setStyleSheet(QCoreApplication::translate("AdminDialog", "background-color:#388E3C;color:white;font-weight:bold;border-radius:6px;padding:6px 12px;", nullptr));
        btnAllUsers->setText(QCoreApplication::translate("AdminDialog", "\360\237\224\204 Refresh", nullptr));
        btnDeleteUser->setStyleSheet(QCoreApplication::translate("AdminDialog", "background-color:#D32F2F;color:white;font-weight:bold;border-radius:6px;padding:6px 12px;", nullptr));
        btnDeleteUser->setText(QCoreApplication::translate("AdminDialog", "\360\237\227\221\357\270\217 Hapus User", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabUsers), QCoreApplication::translate("AdminDialog", "\360\237\221\245 Manage Users", nullptr));
        btnClose->setStyleSheet(QCoreApplication::translate("AdminDialog", "background-color:#757575;color:white;font-weight:bold;border-radius:6px;padding:6px 16px;", nullptr));
        btnClose->setText(QCoreApplication::translate("AdminDialog", "\342\234\226 Tutup", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminDialog: public Ui_AdminDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINDIALOG_H
