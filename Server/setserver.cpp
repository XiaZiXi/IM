#include "setserver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QDebug>

SetServer::SetServer(QDialog *parent) : QDialog(parent)
{
    setupUI();
    this->setModal(true);//设置为模态对话框
    this->serverIP->setText("192.168.191.1");
    this->serverIP->selectAll();
    this->serverIP->setFocus();
    this->serverPORT->setText("9999");

    setupActions();
}
SetServer::~SetServer()
{
}

void SetServer::setupUI()
{
    this->setFixedSize(280,250);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/images/setServer.jpg"));//设置窗口图标
//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
        //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/Login_bkg.jpg")));//背景图片
    this->setPalette(palette);

    spacer1 = new QSpacerItem(18,18);
    spacer3 = new QSpacerItem(18,18);
    serverIPLabel = new QLabel("服务器地址:");
    QFont *font1 = new QFont("汉仪南宫体简",13,240);
    //font1->setBold(true);
    serverIPLabel->setFont(*font1);
    serverIP = new QLineEdit;
    QFont *font2 = new QFont("汉仪南宫体简",13,240);
    //font2->setBold(true);
    serverIP->setFont(*font2);
    QHBoxLayout *layout1 = new QHBoxLayout;
    //layout1->setAlignment(Qt::AlignCenter);
    layout1->addItem(spacer1);
    layout1->addWidget(serverIPLabel);
    layout1->addWidget(serverIP);
    layout1->addItem(spacer3);

    spacer2 = new QSpacerItem(18,18);
    spacer4 = new QSpacerItem(18,18);
    serverPORTLabel = new QLabel("开放端口号:");
    QFont *font3 = new QFont("汉仪南宫体简",13,240);
    //font3->setBold(true);
    serverPORTLabel->setFont(*font3);
    serverPORT = new QLineEdit;
    QFont *font4 = new QFont("汉仪南宫体简",13,240);
    //font4->setBold(true);
    serverPORT->setFont(*font4);
    QHBoxLayout *layout2 = new QHBoxLayout;
    //layout2->setAlignment(Qt::AlignCenter);
    layout2->addItem(spacer2);
    layout2->addWidget(serverPORTLabel);
    layout2->addWidget(serverPORT);
    layout2->addItem(spacer4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(layout1);
    layout->addLayout(layout2);

    okBtn = new QPushButton("确定");
    QFont *font5 = new QFont("汉仪南宫体简",13,240);
    //font5->setBold(true);
    okBtn->setFont(*font5);
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

void SetServer::on_okBtn_clicked()
{
   this->close();
}

void SetServer::setupActions()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(on_okBtn_clicked()));
}


void SetServer::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void SetServer::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


