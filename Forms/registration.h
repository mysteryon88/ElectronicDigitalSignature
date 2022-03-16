#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include "Utilities/RSA.h"
#include <cstdint>

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();
    void Show(RSA* rsa);
    RSA* rsaptr;
    PubKey key;

private slots:
    void on_Cancel_clicked();
    void on_Register_clicked();

private:
    Ui::Registration *ui;
    void Clear();
};

#endif // REGISTRATION_H
