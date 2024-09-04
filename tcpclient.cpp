#include "tcpclient.h"
#include <QHostInfo>

TcpClient::TcpClient()
{
    mSocket = new QTcpSocket();
    beconnected = false;
}

QByteArray TcpClient::readData()
{
    mSocket->waitForReadyRead();
    return mSocket->readAll();
    //qDebug()<<msg;
 }

int TcpClient::connectToServer()
{
    QHostInfo info = QHostInfo::fromName("tcp.example.com");
    QString tcpIp = info.addresses().first().toString();

    tcpIp = "127.0.0.1";
    tcpPort = 6666;
    //检测链接成功信号关联槽函数
    connect(mSocket,SIGNAL(connected()),this,SLOT(connectSucess()));
    //检测掉线信号
    connect(mSocket,SIGNAL(disconnected()),this,SLOT(connectLoss()));
    //连接服务器，设置ip和端口号
    mSocket->connectToHost(tcpIp,tcpPort);
    beconnected = true;
    return 0;

}

bool TcpClient::isConnected()
{
    return beconnected;
}

 void TcpClient::connectServer(QString inIp,int inPort)
 {
     tcpIp = inIp;
     tcpPort = inPort;
     //检测链接成功信号关联槽函数
     connect(mSocket,SIGNAL(connected()),this,SLOT(connectSucess()));
     //检测掉线信号
     connect(mSocket,SIGNAL(disconnected()),this,SLOT(connectLoss()));
     //连接服务器，设置ip和端口号
     mSocket->connectToHost(tcpIp,tcpPort);


 }

 void TcpClient::writeData(QByteArray byte)
 {
     //取发送信息编辑框内容
     mSocket->write(byte);//转编码
     mSocket->waitForBytesWritten();
 }

 void TcpClient::writeData(char *byte,int dataLen)
 {
     //取发送信息编辑框内容
     mSocket->write(byte,dataLen);//转编码
     mSocket->waitForBytesWritten();
 }

void TcpClient::connectSucess()
{
     //连接成功
    //mSocket->write("3223");
}
void TcpClient::connectLoss()
{
    //连接中断
}

void TcpClient::close()
{
    beconnected = false;
    mSocket->close();
}

