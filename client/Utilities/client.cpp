#include "client.h"
#include <QDataStream>
#include <QMessageBox>
#include <QTextCodec>
#include <iostream>

Client::Client(const QString& strHost, int nPort, RSA* rsa_, QWidget *main, QObject *parent)
    : QObject{parent}, NextBlockSize(0), Main(main)
{
    TcpSocket = new QTcpSocket(this);

    TcpSocket->connectToHost(strHost, nPort);
    connect(TcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(slotError(QAbstractSocket::SocketError)));

    rsa = rsa_;
}

void Client::slotReadyRead()
{
    QDataStream in(TcpSocket);
    in.setVersion(QDataStream::Qt_5_14);
    QByteArray mes;
    QString mes_;
        while(TcpSocket->bytesAvailable()>0)
        {
            mes = TcpSocket->readAll();
            mes_ = QTextCodec::codecForName("UTF-16BE")->toUnicode(mes);
#ifdef DEBUG
            qDebug() << mes;
            std::cout << mes_.toStdString() << std::endl;
#endif
        }

        //1 - reg, 2 - e-mail

        /* 1. Type ok or not
         *
         * 2. Type Modulus Open_exp
        */

        int32_t type = mes_[2].unicode() - 48;
#ifdef DEBUG
        std::cout << mes_.toStdString() << std::endl;
#endif
        mes_[0] = '0';
        mes_[1] = '0';
        QStringList list;
        switch (type) {
        case 1:
            list = mes_.split(' ');
            break;
        case 2:
            list = mes_.split(' ');
#ifdef DEBUG
            qDebug() << list.at(1).toULongLong();
            qDebug() << list.at(2).toULongLong();
#endif
            rsa->pub_key.modulus = list.at(1).toULongLong();
            rsa->pub_key.exp = list.at(2).toULongLong();
            break;
        }
}

void Client::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(TcpSocket->errorString()));

    QMessageBox::critical(Main, "Warning", strError);
}

void Client::SendToServer(const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);
    out << str;
    TcpSocket->write(arrBlock);
}
