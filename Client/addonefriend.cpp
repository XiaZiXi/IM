#include "addonefriend.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtNetwork>
#include <QMessageBox>
#include <QDebug>
#include "bases.h"
#include "sqls.h"

AddOneFriend::AddOneFriend(QWidget *parent,QString serverip,quint16 serverport,QString myip,quint16 myport)
    : QWidget(parent),serverip(serverip),serverport(serverport),myip(myip),myport(myport)
{
    udpsocket = new QUdpSocket(this);

    //尼玛，这个地方千万不能够绑定呀，即使是sharedadress也是不行的哟，搞了老子半天！！！！！！！！！！！！！！！！！
    //udpsocket->bind(QHostAddress(myip),myport,QAbstractSocket::ShareAddress);

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(readMsg()));

    this->setFixedSize(240,160);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    //this->setWindowIcon(QIcon(":/Login.jpg"));//设置窗口图标


    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);


    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QHBoxLayout *uplayout = new QHBoxLayout;
    QHBoxLayout *downlayout = new QHBoxLayout;

    idEdit = new QLineEdit;
    idEdit->setFixedSize(160,30);
    idEdit->setFont(QFont("汉仪南宫体简",14,200));
    idEdit->setPlaceholderText("请输入对方ID");

    sendButton = new QPushButton("添 加");
    sendButton->setFixedSize(100,30);
    sendButton->setFont(QFont("汉仪南宫体简",14,200));

    cancelButton = new QPushButton("取 消");
    cancelButton->setFixedSize(100,30);
    cancelButton->setFont(QFont("汉仪南宫体简",14,200));

    uplayout->addWidget(idEdit);
    downlayout->addWidget(cancelButton);
    downlayout->addWidget(sendButton);


    QSpacerItem *item1 = new QSpacerItem(100,20);
    QSpacerItem *item2 = new QSpacerItem(100,20);
    mainlayout->addItem(item1);
    mainlayout->addLayout(uplayout);
    mainlayout->addLayout(downlayout);
    mainlayout->addItem(item2);

    connect(sendButton,SIGNAL(clicked(bool)),this,SLOT(on_sendButton_clicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(on_cancelButton_clicked()));
}


// 发送按钮
void AddOneFriend::on_sendButton_clicked()
{
    bool ok = true;
    int friendid = idEdit->text().toInt(&ok);

    if(!ok)
    {
        QMessageBox::warning(this,"错误","请输入正确的ID!");
        idEdit->setText("");
        idEdit->setFocus();
        return;
    }
    //下面就是向服务器发送添加好友请求了
    ADD_FRIEND_REQUEST_CTOS addfriend = getADD_FRIEND_REQUEST_CTOS((USERID_2T)myport,(USERID_2T)friendid);
    addfriend.head.setmyip(getIPFromString(myip));

    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << addfriend.head;
    datastream << addfriend.body;
    udpsocket->writeDatagram(bytearray,QHostAddress(serverip),serverport);
    
    idEdit->clear();
    this->hide();
}

void AddOneFriend::readMsg()
{
    qDebug() << "readMsg()";
    //不需要做任何处理，因为在主窗口中的socket就是负责接收所有的服务端消息的
}
void AddOneFriend::on_cancelButton_clicked()
{
    idEdit->clear();
    hide();
}
void AddOneFriend::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void AddOneFriend::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}



