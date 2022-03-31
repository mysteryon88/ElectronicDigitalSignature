#include "server.h"
#include <iostream>
#include <QDataStream>
#include <QTextCodec>
#include <QDebug>

Server::Server(int* port, QString ipv4, QObject *parent)
    : QObject{parent}
{
    ptrServer = new QTcpServer();
    connect(ptrServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    if (!ptrServer->listen(QHostAddress(ipv4), *port)) {
        std::cout << "Server Error, enter free port" << std::endl;
            ptrServer->close();
            return;
    }
    std::cout << "Server is running" << std::endl;
    db.ConnectDatabase();
}

void Server::slotNewConnection()
{
    QTcpSocket* pClientSocket = ptrServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, &QTcpSocket::readyRead, this, &Server::ReadClient);
}

void Server::ReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_14);
    while(pClientSocket->bytesAvailable()>0)
    {
        QByteArray mes = pClientSocket->readAll();
        QString mes_ = QTextCodec::codecForName("UTF-16BE")->toUnicode(mes);
        //std::cout << mes_.toStdString() << std::endl;
        Parser(mes_, pClientSocket);
    }

}

void Server::SendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);
    out << str;

    pSocket->write(arrBlock);
    pSocket->waitForBytesWritten();
}

void  Server::Parser(QString& mes, QTcpSocket* pSocket){
    //1 - reg, 2 - e-mail

    /* 1. Type Name Surname Patron Modulus Open_exp Email
     *
     * 2. Type Email -> Modulus Open_exp
    */

    int32_t type = mes[2].unicode() - 48;
    mes[0] = '0';
    mes[1] = '0';
    std::cout << mes.toStdString() << std::endl;
    QStringList list;
    switch (type) {
    case 1:
        list = mes.split(' ');
        if(db.InserIntoTable(list.at(1), list.at(2), list.at(3), list.at(4), list.at(5), list.at(6)))
            std::cout << "Added a new person" << std::endl;
        SendToClient(pSocket, "ok");
        break;
    case 2:
        list = mes.split(' ');
        QString key = db.FindKey(list.at(1));
        if(key == "err1")
            SendToClient(pSocket, "err1");
        if (key == "err2")
            SendToClient(pSocket, "err2");
        else SendToClient(pSocket, "2 " + key);
        break;
    }
}
