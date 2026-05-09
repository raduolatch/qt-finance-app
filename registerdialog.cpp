#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "database.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    connect(ui->btnRegister, &QPushButton::clicked,
            this,            &RegisterDialog::registerUser);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

// VALIDATE INPUT
bool RegisterDialog::validateInput()
{
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();
    QString confirm  = ui->inputConfirm->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Semua field wajib diisi!");
        return false;
    }
    if (username.length() < 3)
    {
        QMessageBox::warning(this, "Error", "Username minimal 3 karakter!");
        return false;
    }
    if (password.length() < 6)
    {
        QMessageBox::warning(this, "Error", "Password minimal 6 karakter!");
        return false;
    }
    if (password != confirm)
    {
        QMessageBox::warning(this, "Error",
                             "Password dan konfirmasi tidak cocok!");
        ui->inputConfirm->clear();
        ui->inputConfirm->setFocus();
        return false;
    }
    return true;
}


// REGISTER USER
void RegisterDialog::registerUser()
{
    if (!validateInput())
        return;

    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();

    if (Database::registerUser(username, password))
    {
        emit registrationSuccess();
        QMessageBox::information(this, "Register",
                                 "Register berhasil! Silakan login.");
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Error",
                             "Username sudah digunakan, coba yang lain!");
    }
}