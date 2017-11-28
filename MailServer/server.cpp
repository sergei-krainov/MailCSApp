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
        //settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        //settings.endGroup();

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
}

void Server::sessionOpened()
{
    std::cout << "Test" << std::endl;
    QHostAddress hostname("127.0.0.1");
    qint16 port = 12345;

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(hostname, port)) {
        return;
    }

    std::cout << tcpServer->serverPort() << std::endl;
}

void Server::sendHello()
{
    QTextCodec *codec = QTextCodec::codecForName("Utf8");

    QByteArray block = codec->fromUnicode("Hello!");
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}
