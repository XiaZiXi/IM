#include "friendinfo.h"
#include <QIcon>
#include <QSize>
#include <QPalette>
#include <QDebug>
#include <QMessageBox>
#include <QGroupBox>
#include <QPixmap>
#if 1
//注意这个界面没有关闭按钮，只是用作提示作用，其本意是通过鼠标指针的悬浮触发其显示
FriendInfo::FriendInfo(QWidget *parent,
                       QString name,
                       QString age,
                       QString nickName,
                       QString identify,
                       QString telephone,
                       QString email,
                       QString department,
                       QString hometown)
    : QWidget(parent)
#endif
//FriendInfo::FriendInfo(QWidget *parent) :QWidget(parent)
{
    this->setFont(QFont("汉仪南宫体简",14,200));//为其中的控件设置共同的东东，在这里是字体
    setupUI();

    this->name->setText(name);
    this->age->setText(age);
    this->nickName->setText(nickName);
    this->identify->setText(identify);
    this->telephone->setText(telephone);
    this->email->setText(email);
    this->department->setText(department);
    this->hometown->setText(hometown);

    setupActions();
}

FriendInfo::~FriendInfo()
{

}

void FriendInfo::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void FriendInfo::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


void FriendInfo::setupUI()
{
    this->setFixedSize(280,200);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/FriendInfo.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
        //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/FriendInfo_bkg.jpg")));//背景图片
    this->setPalette(palette);


//1.basic
    //姓名 年龄 昵称 身份
    nameLabel = new QLabel("姓 名 : ");
    ageLabel = new QLabel("年 龄 : ");
    identifyLabel = new QLabel("昵 称 : ");
    nickNameLabel = new QLabel("身 份 : ");

    name = new QLabel;
    age = new QLabel;
    identify = new QLabel;
    nickName = new QLabel;

    QHBoxLayout *l1 = new QHBoxLayout;
    QHBoxLayout *l2 = new QHBoxLayout;
    QHBoxLayout *l3 = new QHBoxLayout;
    QHBoxLayout *l4 = new QHBoxLayout;
    l1->setAlignment(Qt::AlignLeft);
    l2->setAlignment(Qt::AlignLeft);
    l3->setAlignment(Qt::AlignLeft);
    l4->setAlignment(Qt::AlignLeft);

    l1->addWidget(nameLabel);
    l1->addWidget(name);
    l2->addWidget(ageLabel);
    l2->addWidget(age);
    l3->addWidget(identifyLabel);
    l3->addWidget(identify);
    l4->addWidget(nickNameLabel);
    l4->addWidget(nickName);



//2.more
    //电话 邮箱 部门 故乡
    telephoneLabel = new QLabel("电 话 : ");
    emailLabel = new QLabel("邮 箱 : ");
    departmentLabel = new QLabel("部 门 : ");
    hometownLabel = new QLabel("故 乡 : ");

    telephone = new QLabel;
    email = new QLabel;
    department = new QLabel;
    hometown = new QLabel;

    QHBoxLayout *l5 = new QHBoxLayout;
    QHBoxLayout *l6 = new QHBoxLayout;
    QHBoxLayout *l7 = new QHBoxLayout;
    QHBoxLayout *l8 = new QHBoxLayout;

    l5->setAlignment(Qt::AlignLeft);
    l6->setAlignment(Qt::AlignLeft);
    l7->setAlignment(Qt::AlignLeft);
    l8->setAlignment(Qt::AlignLeft);

    l5->addWidget(telephoneLabel);
    l5->addWidget(telephone);
    l6->addWidget(emailLabel);
    l6->addWidget(email);
    l7->addWidget(departmentLabel);
    l7->addWidget(department);
    l8->addWidget(hometownLabel);
    l8->addWidget(hometown);

//3.mainlayout
    mainlayout = new QVBoxLayout(this);

    mainlayout->addLayout(l1);
    mainlayout->addLayout(l2);
    mainlayout->addLayout(l3);
    mainlayout->addLayout(l4);
    mainlayout->addLayout(l5);
    mainlayout->addLayout(l6);
    mainlayout->addLayout(l7);
    mainlayout->addLayout(l8);
}

void FriendInfo::setupActions()
{
}
