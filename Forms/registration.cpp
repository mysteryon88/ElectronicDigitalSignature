#include "registration.h"
#include "ui_registration.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

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
    QString pass = ui->Pass->text();
    QString name = ui->Name->text();
    QString surname = ui->Surname->text();
    QString pat = ui->Patronyic->text();
    if (e_mail.isEmpty() || pass.isEmpty()|| name.isEmpty() || surname.isEmpty() || pat.isEmpty()){
        QMessageBox::warning(this, "Warning", "All fields must be filled");
        return;
    }
    QDir dir;
    dir.mkdir(name);

    rsaptr->GenerateKey(name);
    rsaptr->GetPubKey(&key);

    QMessageBox::information(this, "Success", "The keys have been generated, you have been registered");
#ifdef DEBUG
    std::cout  << key.modulus << " " << key.open_exp << "\n";
#endif

    //send to server
    Clear();
    close();
}

void Registration::Show(RSA* rsa){
    show();
    rsaptr = rsa;
}

void Registration::Clear()
{
    ui->Email->clear();
    ui->Pass->clear();
    ui->Name->clear();
    ui->Surname->clear();
    ui->Patronyic->clear();
}
