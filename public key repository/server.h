#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <cstdint>
#include <QObject>
#include "database.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(int* port, QString ipv4 = "", QObject *parent = nullptr);

public slots:
     virtual void slotNewConnection();
             void ReadClient();
private:
    Database    db;
    QTcpServer* ptrServer;
    void        SendToClient(QTcpSocket* pSocket, const QString& str);
    void        Parser(QString& mes, QTcpSocket* pSocket);
};

#endif // SERVER_H
