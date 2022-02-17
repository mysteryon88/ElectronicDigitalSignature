#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Utilities/RSA.h"
#include "Utilities/SHA.h"

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
    void on_GenerateKey_clicked();

    void on_LoadMyKeys_clicked();

    void on_SingFile_clicked();

    void on_NewDir_clicked();

    void on_Verification_clicked();

private:
    Ui::MainWindow *ui;
    RSA rsa;
    QString namedir;
};
#endif // MAINWINDOW_H
