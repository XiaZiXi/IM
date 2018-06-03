#include "about.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMouseEvent>

About::About(QDialog *parent) : QDialog(parent)
{
    setupUI();
    setupActions();
    //this->setModal(true);//设置为模态对话框
}
About::~About()
{
}

void About::setupUI()
{
    this->setFixedSize(200,180);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/images/About.jpg"));//设置窗口图标
//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);

    sysMsg = new QTextBrowser;
    sysMsg->setTextColor(Qt::red);
    sysMsg->setFont(QFont("汉仪南宫体简",14,200));
    sysMsg->setText("软件系本人毕业设计\n历时一月\n可共享源码\n欢迎探讨共同进步\nQQ:0000000000");
    sysMsg->setToolTip("期待一块儿学习");
    QFont *font1 = new QFont("汉仪南宫体简",13,180);
    //font1->setBold(true);
    sysMsg->setFont(*font1);
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(sysMsg);


    okBtn = new QPushButton("确定");
    QFont *font2 = new QFont("汉仪南宫体简",14,200);
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

void About::setupActions()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(on_okBtn_clicked()));
}

void About::on_okBtn_clicked()
{
    this->close();
}

void About::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void About::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


