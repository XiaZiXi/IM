#include "chat.h"
#include <QIcon>
#include <QSize>
#include <QPalette>
#include <QDebug>
#include <QMessageBox>
#include <QGroupBox>
#include <QPixmap>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "scrawl.h"
#include "bases.h"
#include "fileTcpServer.h"
#include "fileTcpClient.h"
#include "scrawlclient.h"
#include "scrawlserver.h"

enum {MaxLimitTimes = 20};
enum {MaxLimitSpace = 10};

Chat::Chat(QString friendnickname,QString friendip,int friendid,QString myip,int myid,QWidget *parent)
    : friendnickname(friendnickname),
      friendip(friendip),
      friendid(friendid),
      myip(myip),
      myid(myid),
      QWidget(parent),
    m_timer(new QTimer(this)),
    m_nPosition(0),
    m_curPos(QPoint())
{
    qDebug() << "在Chat的构造函数中::::::::::::::::::"
             << "friendnickname:" << friendnickname
             << "friendip" << friendip
             << "friendid" << friendid
             << "myip" << myip
             << "myid" << myid;
    scrawlserver = new ScrawlServer(NULL,myip,myid+100);//绑定的端口号偏移100，不和文件传送的端口号冲突
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress(myip),myid + 1000);//绑定的端口号移位1000，不和主窗口的port冲突
    connect(socket,SIGNAL(readyRead()),this,SLOT(processMsg()));

    setupUI();
    this->nickNameLabel->setText(friendnickname);

    setupActions();
    m_timer->setInterval(40);//设置间隔
}

Chat::~Chat()
{

}

void Chat::slot_btnShakeClicked()
{
    m_nPosition = 0;
    m_curPos = this->pos();
    m_timer->start();

    //窗口抖动在textBrowser中的描述
    //获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    QString nickName = nickNameLabel->text();
    //将1"" 2发送时间 3textEdit内容   追加到textBrowser中
    textBrowser->setTextColor(QColor(Qt::blue));
    textBrowser->setFont(QFont("汉仪南宫体简",14,200));
    textBrowser->append(QString("我说") + " : "+ currentTime);
    textBrowser->append("你向对方发送了一个窗口抖动");

    qDebug() << "我的IP和绑定的端口是" << myip << myid+1000
             << "我向对方发送了一个窗口抖动！他的IP为" << friendip << "发送到的端口是：" << friendid + 1000;

    SEND_MSG_CTOC msg = getSEND_MSG_CTOC(0,"");
    msg.head.setsegment(0);//0表示抖动，1表示普通文本消息(这样的话body就应该填写文本消息了)
    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << msg.head;
    datastream << msg.body;
    socket->writeDatagram(bytearray,QHostAddress(friendip),friendid + 1000);
}
void Chat::slot_timerOut()
{
    m_timer->stop();
    if(m_nPosition < MaxLimitTimes)
    {
        ++m_nPosition;
        switch(m_nPosition%4)
        {
        case 1:
        {
            QPoint tmpPos(m_curPos.x(),m_curPos.y()-MaxLimitSpace);
            this->move(tmpPos);
        }
            break;
        case 2:
        {
            QPoint tmpPos(m_curPos.x()-MaxLimitSpace,m_curPos.y()-MaxLimitSpace);
            this->move(tmpPos);
        }
            break;
        case 3:
        {
            QPoint tmpPos(m_curPos.x()-MaxLimitSpace,m_curPos.y());
            this->move(tmpPos);
        }
            break;
        default:
        case 0:
            this->move(m_curPos);
            break;
        }
        m_timer->start();
    }
}

void Chat::slot_scrawl()
{
    qDebug() << "slot_scrawl!!!";

    Scrawl *myscrawl = new Scrawl(friendnickname,friendip,friendid,myip,myid);
    myscrawl->show();


    SEND_MSG_CTOC msg = getSEND_MSG_CTOC(0,"");
    msg.head.setsegment(2);//0表示抖动,1表示普通文本消息,2表示涂鸦,3表示文件传送
    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << msg.head;
    datastream << msg.body;
    socket->writeDatagram(bytearray,QHostAddress(friendip),friendid + 1000);
}

void Chat::slot_file()
{
    qDebug() << "slot_file()!!!";
    FileTcpClient *client = new FileTcpClient(NULL,QHostAddress(friendip),(quint16)friendid);
    client->show();

    SEND_MSG_CTOC msg = getSEND_MSG_CTOC(0,"");
    msg.head.setsegment(3);//0表示抖动,1表示普通文本消息,2表示涂鸦,3表示文件传送
    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << msg.head;
    datastream << msg.body;
    socket->writeDatagram(bytearray,QHostAddress(friendip),friendid + 1000);
}

void Chat::processMsg()
{
    //读到数据先解析数据包头的十个字节
    qDebug() << "收到来自客户端的消息!";
    QByteArray data;
    while(socket->hasPendingDatagrams())
    {
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(),data.size());
        qDebug() << "我只执行了一遍！！";//只是执行了一遍而已
    }
    QDataStream datastream(&data,QIODevice::ReadWrite);

    DataHead_18T dataHead;
    datastream >> dataHead;//读取数据包头共十个字节

    ANY_2T any2 = dataHead.getsegment();

    QString message;
    QString empty;

    switch(any2)
    {
    case 0:
        //抖动
        qDebug() << "窗口抖动";
        datastream >> empty;
        handleShake();
        break;
    case 1:
        //普通文本
        qDebug() << "普通文本";
        datastream >> message;
        handleMessage(message);
        break;
    case 2:
        //涂鸦
        qDebug() << "涂鸦";
        datastream >> empty;
        handleScrawl();
        break;
    case 3:
        //文件传送
        qDebug() << "文件传送";
        datastream >> empty;
        handleFile();
        break;
    default :
        qDebug() << "无法知道的命令";
        break;
    }
}

void Chat::handleShake()
{
    //抖动
    m_nPosition = 0;
    m_curPos = this->pos();
    m_timer->start();

    //窗口抖动在textBrowser中的描述
    //获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    //将1"我说:" 2发送时间 3textEdit内容   追加到textBrowser中
    textBrowser->setTextColor(QColor(Qt::red));
    textBrowser->setFont(QFont("汉仪南宫体简",14,200));
    textBrowser->append(QString("Ta说") + " : "+ currentTime);
    textBrowser->append("对方向你发送了一个窗口抖动");
}

void Chat::handleScrawl()
{
    qDebug() << "handleScrawl()";
    //QMessageBox::information(this,"提示","对方当将会向你发送涂鸦");
}

void Chat::handleFile()
{
    qDebug() << "handleFile()";
    QMessageBox::information(this,"文件传送","对方将要向你发送文件！");
    FileTcpServer *server = new FileTcpServer(myip,myid);
    server->show();
}

void Chat::handleMessage(QString msg)
{
    //将消息追加上去
    //获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    //将1"我说:" 2发送时间 3textEdit内容   追加到textBrowser中
    textBrowser->setTextColor(QColor(0x00,0x99,0x99,0xff));
    textBrowser->setFont(QFont("汉仪南宫体简",14,200));
    textBrowser->append(QString("Ta说") + " : "+ currentTime);
    textBrowser->append(msg);
}
void Chat::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}

void Chat::showUpText()
{   if("" == textEdit->toPlainText())
    {
        QMessageBox::warning(this,"警告","发送内容不能为空");
        return;
    }
    else
    {
        //获取textEdit的文字内容
        QString textEditContent = textEdit->toHtml();
        //清空textEdit的文字内容
        textEdit->clear();
        //鼠标焦点放在textEdit中
        textEdit->setFocus();
        //获取当前时间
        QString currentTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
        QString nickName = nickNameLabel->text();
        //将1"我说:" 2发送时间 3textEdit内容   追加到textBrowser中
        textBrowser->setTextColor(QColor(0xcc,0x33,0x66,0xff));
        textBrowser->setFont(QFont("汉仪南宫体简",14,200));
        textBrowser->append(QString("我说") + " : "+ currentTime);
        textBrowser->append(textEditContent);

        SEND_MSG_CTOC msg = getSEND_MSG_CTOC(0,textEditContent);
        msg.head.setsegment(1);//0表示抖动，1表示普通文本消息(这样的话body就应该填写文本消息了)
        QByteArray bytearray;
        QDataStream datastream(&bytearray,QIODevice::ReadWrite);
        datastream << msg.head;
        datastream << msg.body;
        socket->writeDatagram(bytearray,QHostAddress(friendip),friendid + 1000);
    }
}

void Chat::clearUpText()
{
    if(QMessageBox::No == QMessageBox::question(this,"清空记录提醒","你确定要清空所有的通信记录吗？"))
    {
        return;
    }
    else
    {
        textBrowser->clear();
    }
}

void Chat::savaRecords()
{
    QDir dir("C:/");//保存在C盘根目录中
    QString currentTime = QDateTime::currentDateTime().toString("yyMMddhhmmss");
    QFile saveFile(dir.absolutePath() + currentTime + QString(".txt"));
    if (!saveFile.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this,"保存文件","保存文件时出现错误!");
        return;
    }
    QTextStream out(&saveFile);
    out << textBrowser->toPlainText();
    QMessageBox::information(this,"温馨提示","保存文件成功!");
}

void Chat::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


void Chat::setupUI()
{
    this->setFixedSize(320,480);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/Chat.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
        //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/Chat_bkg.jpg")));//背景图片
    this->setPalette(palette);

//交流对象
    //交流对象
    nickNameLabel = new QLabel(this);//这里的this起了很大的作用哈，不然的话，就没有了交流对象的昵称在最上面的布局了
    nickNameLabel->setAlignment(Qt::AlignCenter);
    nickNameLabel->setAutoFillBackground(true);
    nickNameLabel->setPalette(QPalette(QColor(Qt::blue)));
    nickNameLabel->setFont(QFont("汉仪南宫体简",15,255));
    nickNameLabel->setFixedSize(320,30);
//spacer
    QSpacerItem *spacer = new QSpacerItem(30,30);
//上部 查看
    textBrowser = new QTextBrowser;
//buttonslayout  抖动shake 涂鸦scrawl 文件file 保存save 清空clear
    shake = new QToolButton;
    shake->setToolTip("抖动一下");
    shake->setIcon(QIcon(QPixmap(":shake.png")));
    shake->setFixedSize(55,40);

    scrawl = new QToolButton;
    scrawl->setToolTip("发送涂鸦");
    scrawl->setIcon(QIcon(QPixmap(":scrawl.png")));
    scrawl->setFixedSize(55,40);

    file = new QToolButton;
    file->setToolTip("发送文件");
    file->setIcon(QIcon(QPixmap(":file.png")));
    file->setFixedSize(55,40);

    save = new QToolButton;
    save->setToolTip("存为文本");
    save->setIcon(QIcon(QPixmap(":save.png")));
    save->setFixedSize(55,40);

    clear = new QToolButton;
    clear->setToolTip("清空记录");
    clear->setIcon(QIcon(QPixmap(":clear.png")));
    //clear->setAutoRaise(true);
    clear->setFixedSize(55,40);

    QHBoxLayout *buttonslayout = new QHBoxLayout;
    buttonslayout->addWidget(shake);
    buttonslayout->addWidget(scrawl);
    buttonslayout->addWidget(file);
    buttonslayout->addWidget(save);
    buttonslayout->addWidget(clear);

//下部 编辑
    textEdit = new QTextEdit;
    textEdit->setFont(QFont("汉仪南宫体简",14,200));
    textEdit->setFixedHeight(100);
    textEdit->setTextColor(QColor(Qt::black));
//layoutExitHideSend    最小 关闭 发送
    //最小
    hideBtn = new QPushButton;
    hideBtn->setText("最小");
    //hideBtn->setAutoRaise(true);
    hideBtn->setFont(QFont("汉仪南宫体简",15,255));
    hideBtn->setFixedSize(70,40);
    //关闭
    exitBtn = new QPushButton;
    exitBtn->setText("关闭");
    //exitBtn->setAutoRaise(true);
    exitBtn->setFont(QFont("汉仪南宫体简",15,255));
    exitBtn->setFixedSize(70,40);

    QHBoxLayout *layoutExitHide = new QHBoxLayout;
    //layoutExitHide->setMargin(10);
    layoutExitHide->addWidget(hideBtn);
    layoutExitHide->addWidget(exitBtn);

    //发送
    sendBtn = new QPushButton;
    sendBtn->setText("发 送(S)");
    sendBtn->setShortcut(Qt::CTRL+Qt::Key_S);//添加快捷键的方式之二
    sendBtn->setFont(QFont("汉仪南宫体简",15,255));
    sendBtn->setFixedSize(148,40);

    QHBoxLayout *layoutExitHideSend = new QHBoxLayout;
    layoutExitHideSend->addLayout(layoutExitHide);
    layoutExitHideSend->addWidget(sendBtn);

//mainlayout
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->addItem(spacer);
    mainlayout->addWidget(textBrowser);
    mainlayout->addLayout(buttonslayout);
    mainlayout->addWidget(textEdit);
    mainlayout->addLayout(layoutExitHideSend);



}

void Chat::setupActions()
{
    connect(hideBtn,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));//隐藏
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(hide()));//退出即是隐藏

    connect(sendBtn,SIGNAL(clicked(bool)),this,SLOT(showUpText()));//在textBrowser中显示发送的消息
    connect(clear,SIGNAL(clicked(bool)),this,SLOT(clearUpText()));//清空textB中的消息
    connect(save,SIGNAL(clicked(bool)),this,SLOT(savaRecords()));//保存聊天记录


    connect(shake,SIGNAL(clicked()),this,SLOT(slot_btnShakeClicked()));//下面两个槽用于震动屏幕
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timerOut()));


    connect(file,SIGNAL(clicked(bool)),this,SLOT(slot_file()));//文件发送

    connect(scrawl,SIGNAL(clicked(bool)),this,SLOT(slot_scrawl()));//涂鸦
}








