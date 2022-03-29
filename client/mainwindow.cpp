#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cli = new Client("localhost", 1024, &rsa);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoadMyKeys_clicked()
{

    QString privat_key = QFileDialog::getOpenFileName(this, "Choose Private key file", "", "*.key");

    switch(rsa.LoadMyKey(privat_key)){
    case FILE_NOT_OPEN:
        QMessageBox::warning(this, "Warning", "File did not open");
        return;
        break;
    default:
        QMessageBox::information(this, "Success", "Keys have been uploaded");
        break;
    }

    ui->SingFile->setEnabled(true);
}


void MainWindow::on_SingFile_clicked()
{
   QString file = QFileDialog::getOpenFileName(this, "Choose file", "", "All files (*)");
   SHA hash;
   hash.FileHash(file);

   switch(rsa.Encrypt()){
   case FILE_NOT_OPEN:
       QMessageBox::warning(this, "Warning", "File did not open");
       break;
   default:
       QMessageBox::information(this, "Success", "The file is signed (signed.enc)");
       break;
   }
}

void MainWindow::on_Verification_clicked()
{
    QString e_mail = ui->Email->text();

    if(e_mail.isEmpty()){
        QMessageBox::warning(this, "Warning", "You didn't enter the sender's email");
        return;
    }

    cli->SendToServer("2 " + e_mail);

    QEventLoop loop;
    loop.processEvents(QEventLoop::ExcludeUserInputEvents);

    QString enc = QFileDialog::getOpenFileName(this, "Choose signed file", "", "*.enc");
    QString file = QFileDialog::getOpenFileName(this, "Choose file", "", "All files (*)");

    if(enc.isEmpty() || file.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "You didn't choose a file");
        return;
    }

    if (rsa.Verification(enc, file))
        QMessageBox::information(this, "Success", "The signature is real");
    else QMessageBox::critical(this, "Warning", "The signature is not real");

}


void MainWindow::on_Registration_clicked()
{
    reg.Show(&rsa, cli);
}


