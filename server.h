#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QMessageBox>
#include <QFile>
class Server: public QObject
{
    Q_OBJECT
    QTcpServer *tcp;
    bool started=false;
    QTcpSocket *client;
    QFile file;//file for clients messages
    void createTempFile();
public:
    Server(QObject *parent);
    QString start();
    bool stop();
    bool isStarted();
public slots:
    void newUser();
    void slotRead();
};

#endif // SERVER_H
