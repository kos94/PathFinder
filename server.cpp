#include "server.h"

Server::Server(QObject *parent) :
               QObject(parent)
{
}
QString Server::start(){
    if(!started){
        tcp=new QTcpServer;
        int port=6666;
        connect(tcp,SIGNAL(newConnection()),this,SLOT(newUser()));
        if(!tcp->listen(QHostAddress::Any,port))
            return "error";
        else
        {
            foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
            {
                if (interface.flags().testFlag(QNetworkInterface::IsRunning))
                    foreach (QNetworkAddressEntry entry, interface.addressEntries())
                    {
                        if ( interface.hardwareAddress() != "00:00:00:00:00:00" && entry.ip().toString().contains("."))
                        {
                            started=true;
                            return QString("%1:%2").arg(entry.ip().toString()).arg(port);
                        }
                    }
            }
        }
    }
    return "error";
}

bool Server::stop(){
    if(started){
        if(client)
            client->close();
        tcp->close();
        started=false;
        return true;
    }
    return false;
}

void Server::newUser(){
    qDebug("Connect0_0");
    if(started)
    {
        createTempFile();
        client=tcp->nextPendingConnection();
        connect(client,SIGNAL(readyRead()),SLOT(slotRead()));
    }
}

bool Server::isStarted(){
    return started;
}
void Server::createTempFile(){;
    file.setFileName("input_temp");
    file.open(QIODevice::WriteOnly);
    if(file.isOpen())
        QMessageBox::warning((QWidget*)this->parent(),"Error","Error: cant stop server.",QMessageBox::Ok);
}

void Server::slotRead(){
    file.write(client->readAll());
}
