#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"
#include "database.h"
#include <QMessageBox>
#include <QInputDialog>

#define ADMIN_CODE "ADMIN12345"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->btnLoginUser,  &QPushButton::clicked, this, &LoginDialog::loginAsUser);
    connect(ui->btnLoginAdmin, &QPushButton::clicked, this, &LoginDialog::loginAsAdmin);
    connect(ui->btnRegister,   &QPushButton::clicked, this, &LoginDialog::openRegister);

    // Tekan Enter di password langsung login sebagai user
    connect(ui->inputPassword, &QLineEdit::returnPressed,
            this,              &LoginDialog::loginAsUser);

    currentUsername.clear();
    currentRole.clear();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// LOGIN SEBAGAI USER
void LoginDialog::loginAsUser()
{
    doLogin("user");
}

// LOGIN SEBAGAI ADMIN
void LoginDialog::loginAsAdmin()
{
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();

    // Cek username & password dulu
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Error",
                             "Username dan password wajib diisi!");
        return;
    }

    if (!Database::loginUser(username, password))
    {
        ui->inputPassword->clear();
        ui->inputPassword->setFocus();
        QMessageBox::warning(this, "Login Gagal",
                             "Username atau password salah!");
        return;
    }

    // Cek role di database
    QString role = Database::getUserRole(username);
    if (role != "admin")
    {
        QMessageBox::warning(this, "Akses Ditolak",
                             "Akun ini bukan akun admin!");
        return;
    }

    // Baru minta kode admin
    bool ok;
    QString code = QInputDialog::getText(
        this, "Kode Admin",
        "Masukkan kode admin:",
        QLineEdit::Password, "", &ok);

    if (!ok) return;

    if (code != ADMIN_CODE)
    {
        QMessageBox::warning(this, "Akses Ditolak",
                             "Kode admin salah!");
        return;
    }

    currentUsername = username;
    currentRole     = role;
    emit loginSuccess(username);
    accept();
}

// DO LOGIN
bool LoginDialog::doLogin(const QString &expectedRole)
{
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Error",
                             "Username dan password wajib diisi!");
        return false;
    }

    if (!Database::loginUser(username, password))
    {
        currentUsername.clear();
        ui->inputPassword->clear();
        ui->inputPassword->setFocus();
        QMessageBox::warning(this, "Login Gagal",
                             "Username atau password salah!");
        return false;
    }

    // Cek role di database
    QString role = Database::getUserRole(username);

    if (expectedRole == "admin" && role != "admin")
    {
        QMessageBox::warning(this, "Akses Ditolak",
                             "Akun ini bukan akun admin!");
        return false;
    }

    currentUsername = username;
    currentRole     = role;
    emit loginSuccess(username);
    accept();
    return true;
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

// GET ROLE
QString LoginDialog::getRole() const
{
    return currentRole;
}