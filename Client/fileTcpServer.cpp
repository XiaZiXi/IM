#include "fileTcpServer.h"
#include <QtNetwork>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "myimage.h"

FileTcpServer::FileTcpServer(QString listenIP,int listenPort,QWidget *parent) :
    listenIP(listenIP),listenPort(listenPort),QWidget(parent)
{
    setupUI();
    setupActions();

    start();
    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
    //this->hide();
}

void FileTcpServer::setupUI()
{
    this->setFixedSize(240,140);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    //this->setWindowIcon(QIcon(":/Login.jpg"));//设置窗口图标


    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);


    QVBoxLayout *mainlayout = new QVBoxLayout(this);

    QHBoxLayout *uplayout = new QHBoxLayout;
    uplayout->setAlignment(Qt::AlignHCenter);
    QHBoxLayout *downlayout = new QHBoxLayout;

    mainlayout->addLayout(uplayout);
    mainlayout->addLayout(downlayout);


    progressBar = new QProgressBar;
    //progressBar->setTextVisible(false);
    progressBar->setFixedSize(250,35);
    downlayout->addWidget(progressBar);

    statusLabel = new QLabel("状态栏(等待中...)");
    uplayout->addWidget(statusLabel);
    statusLabel->setFrameShadow(QFrame::Raised);
    statusLabel->setWordWrap(true);
    statusLabel->setFont(QFont("汉仪南宫体简",11,150));
}

void FileTcpServer::setupActions()
{

}


// 开启监听
void FileTcpServer::start()
{
    if (!tcpServer.listen(QHostAddress(listenIP),listenPort)) {
        //qDebug() << tcpServer.errorString();
        close();
        return;
    }
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
}

// 接收连接
void FileTcpServer::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    statusLabel->setText("对方将要向你传送文件！");
    // 关闭服务器，不再进行监听
    tcpServer.close();
}

// 接收文件并更新进度条
void FileTcpServer::updateServerProgress()
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
            this->statusLabel->setText(tr("接收文件 %1 ...").arg(fileName));
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
    progressBar->setMaximum(totalBytes);
    progressBar->setValue(bytesReceived);
    // 接收数据完成时
    if (bytesReceived == totalBytes) {
        tcpServerConnection->close();
        localFile->close();
        statusLabel->setText(tr("接收文件 %1 成功！").arg(fileName));
        this->close();//移植过去的时候应该是hide而不是关闭服务器啊！！！！！对吧！
        QMessageBox::information(this,"提示",QString("接收文件 ")+QString(fileName)+QString(" 成功"));

        //打开本地文件夹
        QFileDialog *dialog = new QFileDialog;
        //dialog->show();
        dialog->exec();

    }
}

// 显示错误
void FileTcpServer::displayError(QAbstractSocket::SocketError socketError)
{
    //qDebug() << tcpServerConnection->errorString();
    progressBar->reset();
    this->statusLabel->setText(tr("已经准备好接收文件"));
    tcpServerConnection->close();

    QTimer *timer = new QTimer(this);
    timer->start(10000);
    connect(timer,SIGNAL(timeout()),this,SLOT(close()));//十秒后自动关闭
}

void FileTcpServer::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void FileTcpServer::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}
