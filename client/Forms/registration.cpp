#include "registration.h"
#include "ui_registration.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include "Utilities/RC4.h"

Registration::Registration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_Cancel_clicked()
{
    Clear();
    close();
}

void Registration::on_Register_clicked()
{
    QString e_mail = ui->Email->text();
    QString name = ui->Name->text();
    QString surname = ui->Surname->text();
    QString pat = ui->Patronyic->text();
    QString pass = ui->Password->text();
    if (e_mail.isEmpty() || name.isEmpty() ||
                surname.isEmpty() || pat.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "All fields must be filled!");
        return;
    }

    rsaptr->GenerateKey();
    rsaptr->GetPubKey(&key);

    QMessageBox::information(this, "Success", "The keys have been generated, you have been registered");
#ifdef DEBUG
    std::cout  << key.modulus << " " << key.open_exp << "\n";
#endif

    //send to server
    //Type Name Surname Patron Modulus Open_exp Email
    QString msg = "1 " + name + " " + surname + " " + pat +
                  " " + QString::number(key.modulus) + " " +
                  QString::number(key.exp) + " " + e_mail;
    qDebug() << msg;
    cliptr->SendToServer(msg);

    rsaptr->GetPriKey(&key);

    rsaptr->PrintKeys();

    QString key_str = QString::number(key.modulus) + " " + QString::number(key.exp);

    RC4 rc4(pass.toStdString());
    char K[BUFSIZ], output[BUFSIZ];

    rc4.KeyGen(K, BUFSIZ);
    rc4.PrintASCII(key_str.toLocal8Bit().data(), key_str.size());
    rc4.EnDeCrypt(output, K, BUFSIZ, key_str.toLocal8Bit().data(), key_str.size());
    std::ofstream privat_file;
    privat_file.open("private.key");
    if (privat_file.is_open())
        for (int i = 0; i <  key_str.size(); ++i)
           privat_file << (int)output[i] << " ";
    privat_file.close();

    Clear();
    close();
}

void Registration::Show(RSA* rsa,  Client* cli)
{
    show();
    rsaptr = rsa;
    cliptr = cli;
}

void Registration::Clear()
{
    ui->Email->clear();
    ui->Name->clear();
    ui->Surname->clear();
    ui->Patronyic->clear();
    ui->Password->clear();
}

void Registration::closeEvent(QCloseEvent * e)
{
    Clear();
}
