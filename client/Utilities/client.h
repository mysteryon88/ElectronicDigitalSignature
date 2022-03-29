#ifndef CLIENT_H
#define CLIENT_H

#include "RSA.h"
#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QString& strHost, int nPort, RSA* rsa, QObject *parent = nullptr);
    void        SendToServer(const QString& str);
private:
    QTcpSocket* TcpSocket;
    quint16     NextBlockSize;
    RSA*        rsa;
private slots:
    void slotReadyRead(                            );
    void slotError    (QAbstractSocket::SocketError);
};

#endif // CLIENT_H
