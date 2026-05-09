#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"
#include "database.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->btnLogin,    &QPushButton::clicked,
            this,            &LoginDialog::login);
    connect(ui->btnRegister, &QPushButton::clicked,
            this,            &LoginDialog::openRegister);

    // Tekan Enter di password langsung login
    connect(ui->inputPassword, &QLineEdit::returnPressed,
            this,              &LoginDialog::login);

    currentUsername.clear();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// LOGIN
void LoginDialog::login()
{
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Error",
                             "Username dan password wajib diisi!");
        return;
    }

    if (Database::loginUser(username, password))
    {
        currentUsername = username;
        emit loginSuccess(username);
        accept();
    }
    else
    {
        currentUsername.clear();
        ui->inputPassword->clear();
        ui->inputPassword->setFocus();
        QMessageBox::warning(this, "Login Gagal",
                             "Username atau password salah!");
    }
}

// OPEN REGISTER
void LoginDialog::openRegister()
{
    RegisterDialog dialog(this);
    dialog.exec();
}

// GET USERNAME
QString LoginDialog::getUsername() const
{
    return currentUsername;
}