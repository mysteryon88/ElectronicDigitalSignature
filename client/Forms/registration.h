#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <Utilities/client.h>
#include <QWidget>
#include "Utilities/RSA.h"
#include <cstdint>

#include <QTcpSocket>

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();
    void Show(RSA* rsa, Client* cli);
    PubKey key;
    void closeEvent(QCloseEvent * e);

private slots:
    void on_Cancel_clicked();
    void on_Register_clicked();

private:
    Ui::Registration *ui;
    Client* cliptr;
    RSA* rsaptr;
    void Clear();
};

#endif // REGISTRATION_H
