#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Utilities/RSA.h"
#include "Utilities/SHA.h"
#include "Forms/registration.h"
#include <QMainWindow>
#include "Utilities/client.h"

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

private:
    Ui::MainWindow *ui;

    RSA rsa;
    QString ip;
    Client* cli;
    QString namedir;
    Registration reg;
};
#endif // MAINWINDOW_H
