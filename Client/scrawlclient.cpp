#include "scrawlclient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtNetwork>
#include <QFileDialog>

/*
 *用法:
 *  //服务端用于接收scrawl，客户端用于发送scrawl
 *  //如果服务端没有就绪，那么，5秒后会自动关闭的哟！,出现一个这样的客户端就立即向服务端发送scrawl了
 *  ScrawlClient client(NULL,QHostAddress("192.168.191.1"),9988,"C:/160510103718.png");
 *  不要client.show();
 */
ScrawlClient::ScrawlClient(QWidget *parent,QHostAddress hostAddress,quint16 port,QString filename)
    :QWidget(parent),hostAddress(hostAddress),port(port),fileName(filename)
{
    payloadSize = 64*1024; // 64KB
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket(this);

    // 当连接服务器成功时，发出connected()信号，开始传送文件
    connect(tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));

    connect(tcpClient, SIGNAL(bytesWritten(qint64)),
            this, SLOT(updateClientProgress(qint64)));

    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    tcpClient->connectToHost(hostAddress,port);
}



// 传输文件大小等信息
void ScrawlClient::startTransfer()
{
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly))
    {
        QTimer *timer = new QTimer(this);
        timer->start(5000);
        connect(timer,SIGNAL(timeout()),this,SLOT(close()));//五秒后自动关闭
        return ;
    }
    // 获取文件大小
    totalBytes = localFile->size();

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_4);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    // 保留总大小信息空间、文件名大小信息空间，然后输入文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;
    // 这里的总大小是总大小信息、文件名大小信息、文件名和实际文件大小的总和
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    // 返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64)*2));
    // 发送完文件头结构后剩余数据的大小
    bytesToWrite = totalBytes - tcpClient->write(outBlock);

    outBlock.resize(0);
}

// 发送数据，并更新进度条
void ScrawlClient::updateClientProgress(qint64 numBytes)
{
    // 已经发送数据的大小
    bytesWritten += (int)numBytes;
    // 如果已经发送了数据
    if (bytesToWrite > 0) {
        // 每次发送payloadSize大小的数据，这里设置为64KB，如果剩余的数据不足64KB，
        // 就发送剩余数据的大小
        outBlock = localFile->read(qMin(bytesToWrite, payloadSize));
        // 发送完一次数据后还剩余数据的大小
        bytesToWrite -= (int)tcpClient->write(outBlock);
        // 清空发送缓冲区
        outBlock.resize(0);
    } else { // 如果没有发送任何数据，则关闭文件
        localFile->close();
    }
    // 如果发送完毕
    if(bytesWritten == totalBytes) {
        localFile->close();
        tcpClient->close();
        this->close();
    }
}

// 显示错误
void ScrawlClient::displayError(QAbstractSocket::SocketError)
{
    tcpClient->close();
    QTimer *timer = new QTimer(this);
    timer->start(5000);
    connect(timer,SIGNAL(timeout()),this,SLOT(close()));//五秒后自动关闭
}



