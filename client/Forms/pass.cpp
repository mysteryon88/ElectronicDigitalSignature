#include "pass.h"
#include "ui_pass.h"
#include <QMessageBox>
#include <QFileDialog>
#include "Utilities/RC4.h"
#include <fstream>
#include <iostream>
#include "qdebug.h"

Pass::Pass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pass)
{
    ui->setupUi(this);
}

Pass::~Pass()
{
    delete ui;
}

void Pass::on_Confirm_clicked()
{
    QString pass = ui->PassWord->text();

    if (pass.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "You did not enter a password");
        return;
    }

    QString privat_key = QFileDialog::getOpenFileName(this, "Choose Private key file", "", "*.key");

    RC4 rc4(pass.toStdString());

    char K[BUFSIZ], output[BUFSIZ];
    int input[BUFSIZ];

    rc4.KeyGen(K, BUFSIZ);

    std::ifstream privat_file(privat_key.toLocal8Bit());
    int i = 0;

    if (privat_file.is_open())
        while (!privat_file.eof())
            privat_file >> input[i++];
    else
    {
        QMessageBox::warning(this, "Warning", "File did not open!");
        ui->PassWord->clear();
        return;
    }
    privat_file.close();

    for (int j = 0; j < i; j++)
        output[j] = (char)input[j];

    i--;
#ifdef DEBUG
    rc4.PrintASCII(output, i);
#endif

    rc4.EnDeCrypt(output, K, BUFSIZ, output, i);

#ifdef DEBUG
    rc4.PrintASCII(output, i);

    for (int j = 0; j < i; j++)
        std::cout << output[j];
    std::cout << std::endl;
#endif

    QString output_ = QString(QByteArray(output, i));
    try
    {
        QStringList list;
        list = output_.split(' ');
        if (list.size() == 2)
            rsa->LoadMyKey(list.at(1).toULongLong(), list.at(0).toULongLong());
        else throw 1;
    }
    catch (int i)
    {
         QMessageBox::warning(this, "Warning", "Wrong passwort!");
         ui->PassWord->clear();
         return;
    }
    emit Good();
    QMessageBox::information(this, "Success", "Key has been loaded");
    ui->PassWord->clear();
    close();
}

void Pass::Show(RSA* rsa_)
{
    rsa = rsa_;
    show();
}

void Pass::on_Cancel_clicked()
{
    ui->PassWord->clear();
    close();
}

