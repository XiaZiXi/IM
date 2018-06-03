#include "groupchat.h"
#include <QIcon>
#include <QSize>
#include <QPalette>
#include <QDebug>
#include <QMessageBox>
#include <QGroupBox>
#include <QPixmap>
#include <QDateTime>
#include <QTextStream>
#include "onehuman.h"

GroupChat::GroupChat(QWidget *parent,const char *nickName)
    : QWidget(parent)
{
    setupUI();
    this->nickNameLabel->setText(nickName);

    setupActions();
}

GroupChat::~GroupChat()
{

}

void GroupChat::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}

void GroupChat::showUpText()
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
        textBrowser->setTextColor(QColor(Qt::blue));
        textBrowser->setFont(QFont("汉仪南宫体简",14));
        textBrowser->append(QString("我说") + " : "+ currentTime);
        textBrowser->append(textEditContent);
    }
}


void GroupChat::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


void GroupChat::setupUI()
{
    this->setFixedSize(500,360);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/GroupChat.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(Qt::blue)));//背景色（青绿色）
    this->setPalette(palette);

//交流对象
    //交流对象
    nickNameLabel = new QLabel(this);//这里的this起了很大的作用哈，不然的话，就没有了交流对象的昵称在最上面的布局了
    nickNameLabel->setAlignment(Qt::AlignCenter);
    nickNameLabel->setAutoFillBackground(true);
    nickNameLabel->setPalette(QPalette(QColor(Qt::red)));
    nickNameLabel->setFont(QFont("正楷",15));
    nickNameLabel->setFixedSize(500,30);
//spacer
    QSpacerItem *spacer = new QSpacerItem(500,30);
//上部 查看
    textBrowser = new QTextBrowser; 

//下部 编辑
    textEdit = new QTextEdit;
    textEdit->setFont(QFont("汉仪南宫体简",14));
    textEdit->setFixedHeight(80);
    textEdit->setTextColor(QColor(Qt::black));
//layoutExitHideSend    最小 关闭 发送
    //最小
    hideBtn = new QPushButton;
    hideBtn->setText("最小");
    //hideBtn->setAutoRaise(true);
    hideBtn->setFont(QFont("汉仪南宫体简",15));
    hideBtn->setFixedSize(80,30);
    //关闭
    exitBtn = new QPushButton;
    exitBtn->setText("关闭");
    //exitBtn->setAutoRaise(true);
    exitBtn->setFont(QFont("汉仪南宫体简",15));
    exitBtn->setFixedSize(80,30);

    QHBoxLayout *layoutExitHide = new QHBoxLayout;
    //layoutExitHide->setMargin(10);
    layoutExitHide->addWidget(hideBtn);
    layoutExitHide->addWidget(exitBtn);

    //发送
    sendBtn = new QPushButton;
    sendBtn->setText("发 送(S)");
    sendBtn->setShortcut(Qt::CTRL+Qt::Key_S);//添加快捷键的方式之二
    sendBtn->setFont(QFont("汉仪南宫体简",15));
    sendBtn->setFixedSize(154,30);

    QHBoxLayout *layoutExitHideSend = new QHBoxLayout;
    layoutExitHideSend->addLayout(layoutExitHide);
    layoutExitHideSend->addWidget(sendBtn);

//leftlayout
    QVBoxLayout *leftlayout = new QVBoxLayout;
    //leftlayout->addItem(spacer);
    leftlayout->addWidget(textBrowser);
    leftlayout->addWidget(textEdit);
    leftlayout->addLayout(layoutExitHideSend);
//右边 显示
    listWidget = new QListWidget;
    //listWidget->setFixedSize(160,306);
    listWidget->setFixedWidth(160);
    QVBoxLayout *rightlayout = new QVBoxLayout;
    rightlayout->addWidget(listWidget);

    listWidgetLayout = new QVBoxLayout(listWidget);//以后在其上添加groupMan就直接添加到这里就可以啦
    listWidgetLayout->setAlignment(Qt::AlignTop);
    OneHuman *one = new OneHuman;
    one->setStyleSheet("background-color : rgb(255,255,255)");
    one->setFixedWidth(138);
    listWidgetLayout->addWidget(one);
    OneHuman *one1 = new OneHuman;
    one1->setStyleSheet("background-color : rgb(255,255,255)");
    one1->setFixedWidth(138);
    listWidgetLayout->addWidget(one1);
    OneHuman *one2 = new OneHuman;
    one2->setStyleSheet("background-color : rgb(255,255,255)");
    one2->setFixedWidth(138);
    listWidgetLayout->addWidget(one2);

//mainlayout
    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->setAlignment(Qt::AlignTop);
    mainlayout->addLayout(leftlayout);
    mainlayout->addLayout(rightlayout);

    QVBoxLayout *mains = new QVBoxLayout(this);
    mains->addItem(spacer);
    mains->addLayout(mainlayout);
}

void GroupChat::setupActions()
{
    connect(hideBtn,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));//隐藏
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(close()));//退出

    connect(sendBtn,SIGNAL(clicked(bool)),this,SLOT(showUpText()));//在textBrowser中显示发送的消息
}
