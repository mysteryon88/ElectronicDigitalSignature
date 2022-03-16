#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QWidget>

namespace Ui {
class Authentication;
}

class Authentication : public QWidget
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = nullptr);
    ~Authentication();
    QString namedir;

private slots:
    void on_Cancel_clicked();

    void on_SignIn_clicked();

private:
    Ui::Authentication *ui;
    void Clear();
};

#endif // AUTHENTICATION_H
