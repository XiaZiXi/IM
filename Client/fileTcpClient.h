#ifndef FILETCPCLIENT_H
#define FILETCPCLIENT_H

#include <QProgressBar>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
class QTcpSocket;
class QFile;



class FileTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit FileTcpClient(QWidget *parent = 0,QHostAddress hostAddress = QHostAddress::LocalHost,quint16 port = (quint16)9999);
private :
    QPushButton  *sendButton;
    QPushButton  *openButton;
    QProgressBar *progressBar;
    QLabel *label;
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上
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
    void openFile();
    void send();
    void startTransfer();
    void updateClientProgress(qint64);
    void displayError(QAbstractSocket::SocketError);


    void on_openButton_clicked();
    void on_sendButton_clicked();
};

#endif // FILETCPCLIENT_H
