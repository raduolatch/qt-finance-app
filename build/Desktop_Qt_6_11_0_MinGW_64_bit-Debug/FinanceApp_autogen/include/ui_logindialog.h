/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QLabel *labelTitle;
    QLabel *labelUsername;
    QLineEdit *inputUsername;
    QLabel *labelPassword;
    QLineEdit *inputPassword;
    QPushButton *btnLoginUser;
    QPushButton *btnLoginAdmin;
    QPushButton *btnRegister;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(420, 360);
        labelTitle = new QLabel(LoginDialog);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setGeometry(QRect(95, 20, 240, 40));
        labelTitle->setStyleSheet(QString::fromUtf8("font-size:20px;font-weight:bold;color:#2196F3;"));
        labelUsername = new QLabel(LoginDialog);
        labelUsername->setObjectName("labelUsername");
        labelUsername->setGeometry(QRect(40, 80, 100, 30));
        inputUsername = new QLineEdit(LoginDialog);
        inputUsername->setObjectName("inputUsername");
        inputUsername->setGeometry(QRect(140, 80, 240, 32));
        labelPassword = new QLabel(LoginDialog);
        labelPassword->setObjectName("labelPassword");
        labelPassword->setGeometry(QRect(40, 130, 100, 30));
        inputPassword = new QLineEdit(LoginDialog);
        inputPassword->setObjectName("inputPassword");
        inputPassword->setGeometry(QRect(140, 130, 240, 32));
        inputPassword->setEchoMode(QLineEdit::EchoMode::Password);
        btnLoginUser = new QPushButton(LoginDialog);
        btnLoginUser->setObjectName("btnLoginUser");
        btnLoginUser->setGeometry(QRect(140, 185, 240, 38));
        btnLoginUser->setStyleSheet(QString::fromUtf8("background-color:#2196F3;color:white;font-weight:bold;font-size:14px;border-radius:8px;"));
        btnLoginAdmin = new QPushButton(LoginDialog);
        btnLoginAdmin->setObjectName("btnLoginAdmin");
        btnLoginAdmin->setGeometry(QRect(140, 235, 240, 38));
        btnLoginAdmin->setStyleSheet(QString::fromUtf8("background-color:#F57C00;color:white;font-weight:bold;font-size:14px;border-radius:8px;"));
        btnRegister = new QPushButton(LoginDialog);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setGeometry(QRect(140, 285, 240, 38));
        btnRegister->setStyleSheet(QString::fromUtf8("background-color:#4CAF50;color:white;font-weight:bold;font-size:14px;border-radius:8px;"));

        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Login Finance App", nullptr));
        labelTitle->setText(QCoreApplication::translate("LoginDialog", "Finance App Login", nullptr));
        labelUsername->setText(QCoreApplication::translate("LoginDialog", "Username", nullptr));
        inputUsername->setPlaceholderText(QCoreApplication::translate("LoginDialog", "Masukkan username", nullptr));
        labelPassword->setText(QCoreApplication::translate("LoginDialog", "Password", nullptr));
        inputPassword->setPlaceholderText(QCoreApplication::translate("LoginDialog", "Masukkan password", nullptr));
        btnLoginUser->setText(QCoreApplication::translate("LoginDialog", "\360\237\221\244 Login sebagai User", nullptr));
        btnLoginAdmin->setText(QCoreApplication::translate("LoginDialog", "\360\237\233\241\357\270\217 Login sebagai Admin", nullptr));
        btnRegister->setText(QCoreApplication::translate("LoginDialog", "\360\237\223\235 Register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
