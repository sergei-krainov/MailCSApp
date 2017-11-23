#include <QCoreApplication>
#include <QTimer>
#include <QObject>

#include <stdlib.h>

#include "server.h"

int main(int argc, char *argv[])
{    
    QCoreApplication a(argc, argv);
    Server server;
    //Runner r;
    QTimer::singleShot(0, &server, SLOT(sessionOpened()));

    //QTimer *timer = new QTimer();
    //QObject::connect(timer, SIGNAL(timeout()), &server, SLOT(sessionOpened()));
    //timer->start(1000);


    return a.exec();
}
