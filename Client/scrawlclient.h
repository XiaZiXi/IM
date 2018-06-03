#ifndef SCRAWLCLIENT_H
#define SCRAWLCLIENT_H

#include <QAbstractSocket>
#include <QHostAddress>
#include <QWidget>
class QTcpSocket;
class QFile;



class ScrawlClient : public QWidget
{
    Q_OBJECT

public:
    explicit ScrawlClient(QWidget *parent,QHostAddress hostAddress,quint16 port,QString filename);
public :
    QHostAddress hostAddress;
    quint16 port;
private:
    QTcpSocket *tcpClient;
    QFile *localFile;  // 要发送的文件
    qint64 totalBytes;  // 发送数据的总大小
    qint64 bytesWritten;  // 已经发送数据大小
    qint64 bytesToWrite;   // 剩余数据大小
    qint64 payloadSize;   // 每次发送数据的大小
    QString fileName;  // 保存文件路径
    QByteArray outBlock;  // 数据缓冲区，即存放每次要发送的数据块

private slots:
    void startTransfer();
    void updateClientProgress(qint64);
    void displayError(QAbstractSocket::SocketError);
};

#endif // SCRAWLCLIENT_H
