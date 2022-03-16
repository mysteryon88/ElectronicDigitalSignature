#include "authentication.h"
#include "ui_authentication.h"

#include <QMessageBox>
#include <QFileDialog>

Authentication::Authentication(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Authentication)
{
    ui->setupUi(this);
}

Authentication::~Authentication()
{
    delete ui;
}

void Authentication::on_Cancel_clicked()
{
    Clear();
    close();
}

void Authentication::on_SignIn_clicked()
{
    QString e_mail = ui->Email->text();
    QString pass = ui->Pass->text();
    namedir = ui->Name->text();
    if (e_mail.isEmpty() || pass.isEmpty() || namedir.isEmpty()) {
        QMessageBox::warning(this, "Warning", "All fields must be filled");
        return;
    }

    QDir dir;
    dir.mkdir(namedir);

    //send-to server
    close();
}

void Authentication::Clear()
{
    ui->Email->clear();
    ui->Pass->clear();
    ui->Name->clear();
}

