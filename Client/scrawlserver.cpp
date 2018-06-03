/*
*
* 用法:服务端用于接收scrawl，客户端用于发送scrawl
*     ScrawlServer *server = new ScrawlServer(NULL,"192.168.252.1",9988);
*     千万不要server->show();
*/
#include "scrawlserver.h"
#include <QtNetwork>
#include <QMessageBox>
#include <QFileDialog>
#include "myimage.h"

ScrawlServer::ScrawlServer(QWidget *parent,QString ip,int port) :
    QWidget(parent),ip(ip),port(port)
{
    start();
    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
    this->hide();
}


// 开启监听
void ScrawlServer::start()
{
    if (!tcpServer.listen(QHostAddress(ip),port)) {
        //qDebug() << tcpServer.errorString();
        close();
        return;
    }
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
}

// 接收连接
void ScrawlServer::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    // 关闭服务器，不再进行监听
    tcpServer.close();
}

// 接收文件并更新进度条
void ScrawlServer::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_4);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构
    if (bytesReceived <= sizeof(qint64)*2) {
        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)
                && (fileNameSize == 0)) {
            // 接收数据总大小信息和文件名大小信息
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if((tcpServerConnection->bytesAvailable() >= fileNameSize)
                && (fileNameSize != 0)) {
            // 接收文件名，并建立文件
            in >> fileName;
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly)) {
                //qDebug() << "server: open file error!";
                return;
            }
        } else {
            return;
        }
    }
    // 如果接收的数据小于总数据，那么写入文件
    if (bytesReceived < totalBytes) {
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    // 接收数据完成时
    if (bytesReceived == totalBytes) {
        tcpServerConnection->close();
        localFile->close();
        this->hide();//移植过去的时候应该是hide而不是关闭服务器啊！！！！！对吧！
        QMessageBox::information(NULL,"提示","对方向你发送过来了涂鸦!");

        //打开本地文件夹
        //QFileDialog *dialog = new QFileDialog;
        //dialog->show();

        MyImage *myimage = new MyImage(fileName,NULL);
        //myimage->exec();
        myimage->show();

        ScrawlServer *server = new ScrawlServer(NULL,"192.168.252.1",9988);//子子孙孙无穷尽已
    }
}

// 显示错误
void ScrawlServer::displayError(QAbstractSocket::SocketError socketError)
{
    tcpServerConnection->close();
}
