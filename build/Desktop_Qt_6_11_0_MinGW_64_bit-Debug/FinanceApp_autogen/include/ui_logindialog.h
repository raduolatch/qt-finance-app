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
    QPushButton *btnLogin;
    QPushButton *btnRegister;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(420, 320);
        labelTitle = new QLabel(LoginDialog);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setGeometry(QRect(95, 20, 240, 40));
        labelTitle->setStyleSheet(QString::fromUtf8("\n"
"font-size:20px;\n"
"font-weight:bold;\n"
"color:#2196F3;\n"
"    "));
        labelUsername = new QLabel(LoginDialog);
        labelUsername->setObjectName("labelUsername");
        labelUsername->setGeometry(QRect(40, 80, 100, 30));
        inputUsername = new QLineEdit(LoginDialog);
        inputUsername->setObjectName("inputUsername");
        inputUsername->setGeometry(QRect(140, 80, 220, 32));
        labelPassword = new QLabel(LoginDialog);
        labelPassword->setObjectName("labelPassword");
        labelPassword->setGeometry(QRect(40, 130, 100, 30));
        inputPassword = new QLineEdit(LoginDialog);
        inputPassword->setObjectName("inputPassword");
        inputPassword->setGeometry(QRect(140, 130, 220, 32));
        inputPassword->setEchoMode(QLineEdit::Password);
        btnLogin = new QPushButton(LoginDialog);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setGeometry(QRect(140, 190, 220, 38));
        btnLogin->setStyleSheet(QString::fromUtf8("\n"
"background-color:#2196F3;\n"
"color:white;\n"
"font-weight:bold;\n"
"font-size:14px;\n"
"border-radius:8px;\n"
"    "));
        btnRegister = new QPushButton(LoginDialog);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setGeometry(QRect(140, 240, 220, 38));
        btnRegister->setStyleSheet(QString::fromUtf8("\n"
"background-color:#4CAF50;\n"
"color:white;\n"
"font-weight:bold;\n"
"font-size:14px;\n"
"border-radius:8px;\n"
"    "));

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
        btnLogin->setText(QCoreApplication::translate("LoginDialog", "Login", nullptr));
        btnRegister->setText(QCoreApplication::translate("LoginDialog", "Register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
