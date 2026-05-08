/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QLabel *labelTitle;
    QLineEdit *inputUsername;
    QLineEdit *inputPassword;
    QLineEdit *inputConfirm;
    QPushButton *btnRegister;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(420, 320);
        labelTitle = new QLabel(RegisterDialog);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setGeometry(QRect(120, 20, 180, 40));
        labelTitle->setStyleSheet(QString::fromUtf8("\n"
"font-size:18pt;\n"
"font-weight:bold;\n"
"color:#2196F3;\n"
"    "));
        inputUsername = new QLineEdit(RegisterDialog);
        inputUsername->setObjectName("inputUsername");
        inputUsername->setGeometry(QRect(60, 90, 300, 40));
        inputPassword = new QLineEdit(RegisterDialog);
        inputPassword->setObjectName("inputPassword");
        inputPassword->setGeometry(QRect(60, 150, 300, 40));
        inputPassword->setEchoMode(QLineEdit::Password);
        inputConfirm = new QLineEdit(RegisterDialog);
        inputConfirm->setObjectName("inputConfirm");
        inputConfirm->setGeometry(QRect(60, 210, 300, 40));
        inputConfirm->setEchoMode(QLineEdit::Password);
        btnRegister = new QPushButton(RegisterDialog);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setGeometry(QRect(120, 270, 180, 35));
        btnRegister->setStyleSheet(QString::fromUtf8("\n"
"background-color:#4CAF50;\n"
"color:white;\n"
"font-weight:bold;\n"
"border-radius:8px;\n"
"    "));

        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Register Account", nullptr));
        labelTitle->setText(QCoreApplication::translate("RegisterDialog", "REGISTER ACCOUNT", nullptr));
        inputUsername->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "Username", nullptr));
        inputPassword->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "Password", nullptr));
        inputConfirm->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "Confirm Password", nullptr));
        btnRegister->setText(QCoreApplication::translate("RegisterDialog", "Register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
