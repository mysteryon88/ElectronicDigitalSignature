#include <QCoreApplication>
#include <iostream>
#include <QNetworkInterface>
#include <QList>
#include <QHostAddress>
#include <QTextStream>

#include "database.h"
#include "server.h"

QString run(int* port);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int port = 0;
    Server serv(&port, run(&port));
    return a.exec();
}

QString run(int* port)
{
    int ip = 0;
    std::cout << "You started the server\nSelect the ip address where the server will meet" << std::endl;

    QNetworkInterface inter;
    QList <QHostAddress>inter_ = inter.allAddresses();
    QString ipv4_, choose;
    QTextStream cin(stdin);

    do {

        for(int i = 0; i < inter_.size(); ++i)
        {
            QHostAddress ipv4(inter_[i].toIPv4Address());
            ipv4_ = ipv4.toString();
            std::cout << i << "]" << ipv4_.toStdString() << std::endl;
        }

        if(!(std::cin >> ip).fail() && ip < inter_.size()) {

            QHostAddress ipv4(inter_[ip].toIPv4Address());
            ipv4_ = ipv4.toString();

            std::cout << "You choosed: " << ipv4_.toStdString() << " Y/N?" << std::endl;

            cin >> choose;

            if(choose == "y" || choose == "Y") break;
        }
        else {
            std::cout << "Enter again!" << std::endl;
            std::cin.clear();
            std::cin.ignore(100,'\n');
        }
    } while(true);

    std::cout << "Enter a free port: "<< std::endl;
    std::cin >> *port;

    return ipv4_;
}
