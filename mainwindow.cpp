#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_GenerateKey_clicked()
{
    rsa.GenerateKey(namedir);
    QMessageBox::information(this, "Success", "The keys have been generated");
    ui->GenerateKey->setDisabled(true);
    ui->LoadMyKeys->setDisabled(true);
    ui->SingFile->setEnabled(true);
}

void MainWindow::on_LoadMyKeys_clicked()
{
    QString public_key = QFileDialog::getOpenFileName(this, "Choose Public key file", "", "*.key");
    QString privat_key = QFileDialog::getOpenFileName(this, "Choose Private key file", "", "*.key");

    switch(rsa.LoadMyKey(public_key, privat_key)){
    case FILE_MIXED_UP:
        QMessageBox::warning(this, "Warning", "You may have mixed up the files");
        break;
    case FILE_NOT_OPEN:
        QMessageBox::warning(this, "Warning", "File did not open");
        break;
    default:
        QMessageBox::information(this, "Success", "Keys have been uploaded");
        break;
    }
    ui->GenerateKey->setDisabled(true);
    ui->LoadMyKeys->setDisabled(true);
    ui->SingFile->setEnabled(true);
}


void MainWindow::on_SingFile_clicked()
{
   QString file = QFileDialog::getOpenFileName(this, "Choose file", "", "All files (*)");
   SHA hash;
   hash.FileHash(file, namedir);

   switch(rsa.Encrypt(namedir)){
   case FILE_NOT_OPEN:
       QMessageBox::warning(this, "Warning", "File did not open");
       break;
   default:
       QMessageBox::information(this, "Success", "The file is signed (signed.enc)");
       break;
   }
}


void MainWindow::on_NewDir_clicked()
{
    namedir = ui->Name->text();

    if (namedir.isEmpty()) {
        QMessageBox::warning(this, "Warning", "You didn't enter a name");
        return;
    }
    QDir dir;
    dir.mkdir(namedir);
    ui->GenerateKey->setEnabled(true);
    ui->LoadMyKeys->setEnabled(true);
    ui->Verification->setEnabled(true);
}


void MainWindow::on_Verification_clicked()
{
    QString pub_key = QFileDialog::getOpenFileName(this, "Choose Public key file", "", "*.key");
    QString enc = QFileDialog::getOpenFileName(this, "Choose signed file", "", "*.enc");
    QString file = QFileDialog::getOpenFileName(this, "Choose file", "", "All files (*)");

    if(pub_key.isEmpty() || enc.isEmpty() || file.isEmpty()){
        QMessageBox::warning(this, "Warning", "You didn't choose a file");
        return;
    }

    if (rsa.Verification(pub_key, enc, file, namedir))
        QMessageBox::information(this, "Success", "The signature is real");
    else QMessageBox::critical(this, "Warning", "The signature is not real");
}

