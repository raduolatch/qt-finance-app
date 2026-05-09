#include "mainwindow.h"
#include "logindialog.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Inisialisasi database sebelum apapun
    Database::connect();

    // Tampilkan login dialog
    LoginDialog login;
    if (login.exec() == QDialog::Accepted)
    {
        QString username = login.getUsername();
        MainWindow window(username);
        window.show();
        return a.exec();
    }

    return 0;
}