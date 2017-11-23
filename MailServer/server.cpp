#include <QtNetwork>

#include <stdlib.h>
#include <iostream>

#include "server.h"

Server::Server(QObject *parent)
    : QObject(parent)
    , tcpServer(Q_NULLPTR)
    , networkSession(0)
{
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }


        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        networkSession->open();
    }
    else {
        sessionOpened();
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendHello);

    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");
}

void Server::sessionOpened()
{
    std::cout << "Test" << std::endl;
    QHostAddress hostname("127.0.0.1");
    qint16 port = 12345;

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(hostname, port)) {
        //std::cout << tcpServer->errorString() << endl;
        //close();
        return;
    }

    std::cout << tcpServer->serverPort() << std::endl;
}

void Server::sendHello()
{
    /* QByteArray block;
    block.clear();
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    out << fortunes.at(qrand() % fortunes.size());
    out << "";
    QString t =  fortunes.at(qrand() % fortunes.size());
    QString t1 = "Hello";
    std::string t2 = t.toLocal8Bit().constData();
    const char *t3 = block.data();
    const char *t4 = "Hey";


    std::cout << "Sending data" << t3 << "and " << t4 << std::endl;

    */

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);



    //clientConnection->write(block);
    clientConnection->write("Hello?");
    clientConnection->disconnectFromHost();
}
