#include "sendsysmsg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMouseEvent>

SendSysMsg::SendSysMsg(QDialog *parent) : QDialog(parent)
{
    setupUI();
    setupActions();
    //this->setModal(true);//设置为模态对话框
}
SendSysMsg::~SendSysMsg()
{
}

void SendSysMsg::setupUI()
{
    this->setFixedSize(250,180);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/images/sendSysMsg.jpg"));//设置窗口图标
//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);

    sysMsg = new QTextEdit;
    sysMsg->setTextColor(Qt::red);
    sysMsg->setToolTip("填写将要发送的系统消息");
    QFont *font1 = new QFont("汉仪南宫体简",13,240);
    //font1->setBold(true);
    sysMsg->setFont(*font1);
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(sysMsg);


    okBtn = new QPushButton("确定");
    QFont *font2 = new QFont("汉仪南宫体简",14,255);
    //font2->setBold(true);
    okBtn->setFont(*font2);
    okBtn->setFixedSize(70,30);
    spacer5 = new QSpacerItem(180,30);
    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addItem(spacer5);
    layout3->addWidget(okBtn);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->addLayout(layout1);
    mainlayout->addLayout(layout3);
}

void SendSysMsg::setupActions()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(on_okBtn_clicked()));
}

void SendSysMsg::on_okBtn_clicked()
{
    this->close();
}

void SendSysMsg::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void SendSysMsg::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


