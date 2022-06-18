#ifndef PASS_H
#define PASS_H

#include <QWidget>

#include "Utilities/RSA.h"

namespace Ui {
class Pass;
}

class Pass : public QWidget
{
    Q_OBJECT

public:
    explicit Pass(QWidget *parent = nullptr);
    ~Pass();
     void Show(RSA* rsa_);
private slots:
    void on_Confirm_clicked();

    void on_Cancel_clicked();

private:
    Ui::Pass *ui;
    RSA     *rsa;
signals:
    void Good();
};

#endif // PASS_H
