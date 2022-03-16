#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Utilities/RSA.h"
#include "Utilities/SHA.h"
#include "Forms/registration.h"
#include "Forms/authentication.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LoadMyKeys_clicked();
    void on_SingFile_clicked();
    void on_Verification_clicked();
    void on_Registration_clicked();
    void on_SugnIn_clicked();

private:
    Ui::MainWindow *ui;
    RSA rsa;
    QString namedir;
    Registration reg;
    Authentication auth;
};
#endif // MAINWINDOW_H
