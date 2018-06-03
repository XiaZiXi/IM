#include "setserver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>

SetServer::SetServer()
{
    setupUI();
    setupActions();
    this->setModal(true);//设置为模态对话框
    this->serverIP->setText("192.168.191.1");
    this->serverPORT->setText("9999");
    this->myIP->setText("192.168.191.1");
    this->myIP->selectAll();
    this->myIP->setFocus();
}
SetServer::~SetServer()
{
}
void SetServer::setupActions()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(myhide()));//点击后隐藏，不要退出，他的组合父控件退出时，会自动把它回收的！！
}

void SetServer::myhide()
{
    serverip = serverIP->text();
    serverport = serverPORT->text();
    myip = myIP->text();
    qDebug() << "myhide()" << serverip << serverport << myip;
    hide();
}

QString SetServer::getServerIP()
{
    return serverip;
}
QString SetServer::getServerPORT()
{
    return serverport;
}

QString SetServer::getMyIP()
{
    return myip;
}

void SetServer::setupUI()
{
    this->setFixedSize(280,250);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/SetServer.jpg"));//设置窗口图标
//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
        //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/Login_bkg.jpg")));//背景图片
    this->setPalette(palette);

    spacer11 = new QSpacerItem(18,18);
    spacer33 = new QSpacerItem(18,18);
    myIPLabel = new QLabel("本主机地址:");
    myIPLabel->setFont(QFont("汉仪南宫体简",13,200));
    myIP = new QLineEdit;
    myIP->setFont(QFont("汉仪南宫体简",13,200));
    QHBoxLayout *layout11 = new QHBoxLayout;
    //layout1->setAlignment(Qt::AlignCenter);
    layout11->addItem(spacer11);
    layout11->addWidget(myIPLabel);
    layout11->addWidget(myIP);
    layout11->addItem(spacer33);


    spacer1 = new QSpacerItem(18,18);
    spacer3 = new QSpacerItem(18,18);
    serverIPLabel = new QLabel("服务器地址:");
    serverIPLabel->setFont(QFont("汉仪南宫体简",13,200));
    serverIP = new QLineEdit;
    serverIP->setFont(QFont("汉仪南宫体简",13,200));
    QHBoxLayout *layout1 = new QHBoxLayout;
    //layout1->setAlignment(Qt::AlignCenter);
    layout1->addItem(spacer1);
    layout1->addWidget(serverIPLabel);
    layout1->addWidget(serverIP);
    layout1->addItem(spacer3);

    spacer2 = new QSpacerItem(18,18);
    spacer4 = new QSpacerItem(18,18);
    serverPORTLabel = new QLabel("服务器端口:");
    serverPORTLabel->setFont(QFont("汉仪南宫体简",13,200));
    serverPORT = new QLineEdit;
    serverPORT->setFont(QFont("汉仪南宫体简",13,200));
    QHBoxLayout *layout2 = new QHBoxLayout;
    //layout2->setAlignment(Qt::AlignCenter);
    layout2->addItem(spacer2);
    layout2->addWidget(serverPORTLabel);
    layout2->addWidget(serverPORT);
    layout2->addItem(spacer4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(layout11);
    layout->addLayout(layout1);
    layout->addLayout(layout2);

    okBtn = new QPushButton("确定");
    okBtn->setFont(QFont("汉仪南宫体简",15,255));
    okBtn->setFixedSize(70,35);
    spacer5 = new QSpacerItem(30,30);
    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addItem(spacer5);
    layout3->addWidget(okBtn);

    QVBoxLayout *mainlayout = new QVBoxLayout;
    spacer6 = new QSpacerItem(30,25);
    mainlayout->addLayout(layout);
    mainlayout->addLayout(layout3);
    mainlayout->addItem(spacer6);
    this->setLayout(mainlayout);
}

void SetServer::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void SetServer::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


