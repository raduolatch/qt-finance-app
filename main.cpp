#include "mainwindow.h"
#include "logindialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // TRANSLATOR
    QTranslator translator;

    const QStringList uiLanguages =
        QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages)
    {
        const QString baseName =
            "FinanceApp_"
            + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    // LOGIN
    LoginDialog login;

    if (login.exec() == QDialog::Accepted)
    {
        MainWindow window;

        window.show();

        return a.exec();
    }

    return 0;
}