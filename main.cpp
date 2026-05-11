#include "mainwindow.h"
#include "admindialog.h"
#include "logindialog.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Inisialisasi database sebelum apapun
    Database::connect();

    // Loop login — supaya bisa logout dan login ulang
    while (true)
    {
        LoginDialog login;
        if (login.exec() != QDialog::Accepted)
            break; // user tutup app → keluar

        QString username = login.getUsername();
        QString role     = login.getRole();

        if (role == "admin")
        {
            // Buka Admin Panel
            AdminDialog adminWindow(username);
            adminWindow.exec(); // exec() supaya blocking, setelah tutup balik ke login
        }
        else
        {
            // Buka Main Window biasa
            MainWindow window(username);
            window.show();
            a.exec(); // tunggu sampai window ditutup
        }
        // Setelah window ditutup → loop kembali ke login
    }

    return 0;
}