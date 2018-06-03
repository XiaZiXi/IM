#include "fileTcpClient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtNetwork>
#include <QFileDialog>
#include <QMessageBox>

FileTcpClient::FileTcpClient(QWidget *parent,QHostAddress hostAddress,quint16 port) : QWidget(parent)
{
    this->setFixedSize(240,160);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    //this->setWindowIcon(QIcon(":/Login.jpg"));//设置窗口图标


    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);


    this->hostAddress = hostAddress;
    this->port = port;

    QVBoxLayout *mainlayout = new QVBoxLayout(this);

    QHBoxLayout *uplayout = new QHBoxLayout;
    QHBoxLayout *downlayout = new QHBoxLayout;
    QHBoxLayout *middlayout = new QHBoxLayout;

    sendButton = new QPushButton("发送");
    sendButton->setFixedSize(100,30);
    sendButton->setFont(QFont("汉仪南宫体简",14,200));
    sendButton->setEnabled(false);

    openButton = new QPushButton("打开");
    openButton->setFixedSize(100,30);
    openButton->setFont(QFont("汉仪南宫体简",14,200));

    label = new QLabel("状态栏");
    label->setFrameShadow(QFrame::Raised);
    label->setWordWrap(true);
    label->setFont(QFont("汉仪南宫体简",10,150));
    middlayout->addWidget(label);
    middlayout->setAlignment(Qt::AlignHCenter);

    mainlayout->addLayout(uplayout);
    mainlayout->addLayout(middlayout);
    mainlayout->addLayout(downlayout);

    connect(sendButton,SIGNAL(clicked(bool)),this,SLOT(on_sendButton_clicked()));
    connect(openButton,SIGNAL(clicked(bool)),this,SLOT(on_openButton_clicked()));

    progressBar = new QProgressBar;
    //progressBar->setTextVisible(false);
    progressBar->setFixedSize(250,35);

    uplayout->addWidget(progressBar);
    downlayout->addWidget(sendButton);
    downlayout->addWidget(openButton);

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

    sendButton->setEnabled(false);
}


// 打开文件
void FileTcpClient::openFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        sendButton->setEnabled(true);
        label->setText(tr("打开文件 %1 成功！").arg(fileName));
    }
}

// 连接到服务器
void FileTcpClient::send()
{
    sendButton->setEnabled(false);
    // 初始化已发送字节为0
    bytesWritten = 0;
    label->setText(tr("连接中......"));
    tcpClient->connectToHost(hostAddress,port);

}

// 传输文件大小等信息
void FileTcpClient::startTransfer()
{
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly))
    {
        //qDebug() << "client: open file error!";
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

    //ui->clientStatusLabel->setText(tr("已连接"));
    outBlock.resize(0);
}

// 发送数据，并更新进度条
void FileTcpClient::updateClientProgress(qint64 numBytes)
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
    // 更新进度条
    progressBar->setMaximum(totalBytes);
    progressBar->setValue(bytesWritten);
    // 如果发送完毕
    if(bytesWritten == totalBytes) {
        label->setText(tr("传送文件 %1 成功").arg(fileName));
        localFile->close();
        tcpClient->close();
        this->close();
        QMessageBox::information(this,"提示",QString("文件 ")+QString(fileName)+QString(" 传送成功！"));
    }
}

// 显示错误
void FileTcpClient::displayError(QAbstractSocket::SocketError)
{
    tcpClient->close();
    progressBar->reset();
    label->setText(tcpClient->errorString());
    sendButton->setEnabled(true);

    QTimer *timer = new QTimer(this);
    timer->start(10000);
    connect(timer,SIGNAL(timeout()),this,SLOT(close()));//十秒后自动关闭
}

// 打开按钮
void FileTcpClient::on_openButton_clicked()
{
    progressBar->reset();
    label->setText(tr("状态：等待打开文件！"));
    openFile();
}

// 发送按钮
void FileTcpClient::on_sendButton_clicked()
{
    send();
}
void FileTcpClient::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void FileTcpClient::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}



