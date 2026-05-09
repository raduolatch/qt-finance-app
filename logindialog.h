#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QString getUsername() const;

signals:
    void loginSuccess(const QString &username);

private slots:
    void login();
    void openRegister();

private:
    Ui::LoginDialog *ui;
    QString currentUsername;
};

#endif // LOGINDIALOG_H