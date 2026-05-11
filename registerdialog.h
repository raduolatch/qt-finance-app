#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H
#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

signals:
    void registrationSuccess();

private slots:
    void registerUser();

private:
    Ui::RegisterDialog *ui;
    bool validateInput();
};

#endif // REGISTERDIALOG_H