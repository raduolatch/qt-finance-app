#include "logindialog.h"
#include "ui_logindialog.h"

#include "registerdialog.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->btnLogin,
            &QPushButton::clicked,
            this,
            &LoginDialog::login);

    connect(ui->btnRegister,
            &QPushButton::clicked,
            this,
            &LoginDialog::openRegister);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::login()
{
    QString username =
        ui->inputUsername->text().trimmed();

    QString password =
        ui->inputPassword->text().trimmed();

    QFile file("users.txt");

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(
            this,
            "Error",
            "users.txt tidak ditemukan!"
            );

        return;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line =
            in.readLine();

        QStringList data =
            line.split(",");

        if (data.size() >= 2)
        {
            if (data[0] == username
                && data[1] == password)
            {
                file.close();

                accept();

                return;
            }
        }
    }

    file.close();

    QMessageBox::warning(
        this,
        "Login Gagal",
        "Username atau password salah!"
        );
}

void LoginDialog::openRegister()
{
    RegisterDialog dialog;

    dialog.exec();
}