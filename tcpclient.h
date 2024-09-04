//#include <QTcpClient>
#include <QTcpSocket>
#include <QObject>


#ifndef TCPCLIENT_H
#define TCPCLIENT_H


class TcpClient:public QObject
{
public:
    TcpClient();
    QByteArray readData();
    void connectServer(QString inIp,int inPort);
    int connectToServer();
    bool isConnected();
    void writeData(QByteArray byte);
    void writeData(char *byte,int dataLen);
    void close();

public slots:
    void connectSucess();
    void connectLoss();

private:
    QString tcpIp;
    int tcpPort;
    bool beconnected;
    QTcpSocket * mSocket;

};

#endif // TCPCLIENT_H
