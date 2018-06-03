#ifndef SCRAWLSERVER_H
#define SCRAWLSERVER_H

#include <QAbstractSocket>
#include <QTcpServer>
#include <QWidget>

class QTcpSocket;
class QFile;

class ScrawlServer : public QWidget
{
    Q_OBJECT

public:
    explicit ScrawlServer(QWidget *parent,QString ip,int port);
private:
    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;     // 存放总大小信息
    qint64 bytesReceived;  // 已收到数据的大小
    qint64 fileNameSize;   // 文件名的大小信息
    QString fileName;      // 存放文件名
    QFile *localFile;      // 本地文件
    QByteArray inBlock;    // 数据缓冲区

    QString ip;
    int port;

private slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);
};

#endif // SCRAWLSERVER_H
