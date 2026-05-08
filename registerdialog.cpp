#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    connect(ui->btnRegister,
            &QPushButton::clicked,
            this,
            &RegisterDialog::registerUser);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::registerUser()
{
    QString username =
        ui->inputUsername->text().trimmed();

    QString password =
        ui->inputPassword->text().trimmed();

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Error",
            "Username dan password harus diisi!"
            );

        return;
    }

    QFile file("users.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);

        out << username
            << ","
            << password
            << "\n";

        file.close();
    }

    QMessageBox::information(
        this,
        "Register",
        "Register berhasil!"
        );

    accept();
}