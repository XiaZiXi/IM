#include "onehuman.h"
#include <QHBoxLayout>
#include "chat.h"
#include <QDebug>
//有个想法，把端口号加上1000，这样就可以在客户端之间直接有socket了，而不用通过mainwindow的socket了！！！！！！！！！！

OneHuman::OneHuman(int myid,QString mainwindowip,int mainwindowport,QString mynickname,IP_4T friendip,QToolButton *parent)
  :myid(myid),mainwindowip(mainwindowip),mainwindowport(mainwindowport),mynickname(mynickname),friendip(friendip),QToolButton(parent)
{
    qDebug() << "在OneHuman的构造函数中::::::::::::::::::"
             << "mainwindowport:" << mainwindowport
             << "mainwindowip" << mainwindowip
             << "friendip" << getIPFromInt(friendip)
             << "mynickname " << mynickname
             << "myid" << myid;
    setupUI();
    chat = new Chat(mynickname,getIPFromInt(friendip),myid,mainwindowip,mainwindowport);
    setupActions();
}
OneHuman::~OneHuman()
{}
void OneHuman::setupUI()
{
   this->setStyleSheet("background-color: rgb(4,159,241)");//5-31修改
   //this->setStyleSheet("background-color: rgb(0,255,255)");//5-20修改
   nickNameIcon = new QLabel;
   QString pix = QString(":/") + QString::number(myid - 10000) + QString("ON.bmp");
   qDebug() << "pix is :" << pix;
   nickNameIcon->setPixmap
           (QPixmap(pix).
            scaled(QSize(56,72),
                   Qt::IgnoreAspectRatio,
                   Qt::SmoothTransformation));   //默认的头像图标
   nickNameLabel = new QLabel(mynickname);            //昵称
   nickNameLabel->setFixedSize(200,24);
   nickNameLabel->setAlignment(Qt::AlignLeft);
   nickNameLabel->setFont(QFont("汉仪南宫体简",12,200));
   this->setFixedSize(233,72);
   this->setAutoRaise(true);

   QHBoxLayout *layout = new QHBoxLayout(this);
   layout->addWidget(nickNameIcon);
   layout->addWidget(nickNameLabel);
}


void OneHuman::mouseDoubleClickEvent(QMouseEvent *)
{
//弹出一个聊天框，和对方聊天
    //建立了聊天框之前，应该判断是否是登录好了，如果是离线状态的话，应当禁止发送消息
    chat->show();

    //接下来就是操作数据库来更新个人信息了
}

void OneHuman::setupActions()
{
}

OneHuman *OneHuman::createDefaultOneHuman()
{
    //OneHuman *newMan = new OneHuman(0,"127.0.0.1",0,"");
    //return newMan;
    return NULL;
}

