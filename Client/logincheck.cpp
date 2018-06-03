#include "logincheck.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QDebug>

LoginCheck::LoginCheck(QWidget *parent) : QWidget(parent)
{   
    setupUI();
}

LoginCheck::~LoginCheck()
{
}

void LoginCheck::setupUI()
{
    this->setFixedSize(280,600);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/LoginCheck.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(Qt::blue)));//背景色（青绿色）
    this->setPalette(palette);

//1.取消 和 动画
    //image labels
    label1 = new QLabel;
    label1->setPixmap(QPixmap(":/1.png"));
    label2 = new QLabel;
    label2->setPixmap(QPixmap(":/2.png"));
    label3 = new QLabel;
    label3->setPixmap(QPixmap(":/3.png"));
    label4 = new QLabel;
    label4->setPixmap(QPixmap(":/4.png"));

    label1->hide();
    label2->hide();
    label3->hide();
    label4->hide();

    //hlayout
    QHBoxLayout *hlayout = new QHBoxLayout;

    //抱怨下。。。。。如果widget隐藏了的话，布局管理器自动的排版，忽略了隐藏的widget，也是醉了！！
    hlayout->addWidget(label1,0,Qt::AlignCenter);
    hlayout->addWidget(label2,0,Qt::AlignCenter);
    hlayout->addWidget(label3,0,Qt::AlignCenter);
    hlayout->addWidget(label4,0,Qt::AlignCenter);

    //shows
    shows = new QLabel("登录中 请稍等");
    shows->setFixedSize(130,40);
    shows->setFont(QFont("汉仪南宫体简",15));

    spacer1 = new QSpacerItem(40,40);
    spacer2 = new QSpacerItem(40,130);
//2.主布局
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setAlignment(Qt::AlignCenter);
    mainlayout->addLayout(hlayout);
    mainlayout->addItem(spacer1);
    mainlayout->addWidget(shows);
    mainlayout->addItem(spacer2);

    this->setLayout(mainlayout);
#if 1
    //测试动画效果才加上的
    this->startAnimation();
#endif
}

void LoginCheck::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void LoginCheck::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}

void LoginCheck::setupActions()
{

}

//只需要调用这个函数就可以实现小球闪烁的动画了！！！
void LoginCheck::startAnimation()
{
    timer = new QTimer(this);
    timer->start(200);//200ms更新一次
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
}

void LoginCheck::update()
{
    static int index = 1;
    if(index > 4)
        index = 1;
    if(1 == index)      {label1->show();label2->setVisible(false);label3->setVisible(false);label4->setVisible(false);}
    else if(2 == index) {label2->show();label1->setVisible(false);label3->setVisible(false);label4->setVisible(false);}
    else if(3 == index) {label3->show();label2->setVisible(false);label1->setVisible(false);label4->setVisible(false);}
    else                {label4->show();label2->setVisible(false);label3->setVisible(false);label1->setVisible(false);}
    ++ index;
}

