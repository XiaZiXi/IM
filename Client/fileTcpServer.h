#ifndef FILETCPSERVER_H
#define FILETCPSERVER_H

#include <QDialog>
#include <QAbstractSocket>
#include <QTcpServer>
#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

class QTcpSocket;
class QFile;

class FileTcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit FileTcpServer(QString listenIP,int listenPort,QWidget *parent = 0);
private :
    void setupUI();
    void setupActions();
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上

private:
    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;     // 存放总大小信息
    qint64 bytesReceived;  // 已收到数据的大小
    qint64 fileNameSize;   // 文件名的大小信息
    QString fileName;      // 存放文件名
    QFile *localFile;      // 本地文件
    QByteArray inBlock;    // 数据缓冲区

    QProgressBar *progressBar;//进度条
    QLabel *statusLabel;      //状态条

private slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);
public :
    QString listenIP;
    int listenPort;
};

#endif // FILETCPSERVER_H
