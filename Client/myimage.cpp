#include "myimage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>


MyImage::MyImage(QString name,QDialog *parent)
    : name(name),QDialog(parent)
{
    setupUI();
    setupActions();
    //this->setModal(true);//设置为模态对话框
}
void MyImage::setupUI()
{
    this->setFixedSize(395,498);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowTitle("成功收到来自对方的涂鸦！");
    this->setWindowIcon(QIcon(":/images/About.jpg"));//设置窗口图标
//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);

    image = new QLabel("兰州交通大学毕业设计");
    image->setAlignment(Qt::AlignCenter);
    image->setFont(QFont("汉仪南宫体简",28,255));
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(image);


    okBtn = new QPushButton("确 定");
    QFont *font2 = new QFont("汉仪南宫体简",15,255);
    //font2->setBold(true);
    okBtn->setFont(*font2);
    okBtn->setFixedSize(80,40);
    spacer5 = new QSpacerItem(180,40);
    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addItem(spacer5);
    layout3->addWidget(okBtn);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->addLayout(layout1);
    mainlayout->addLayout(layout3);

    timer = new QTimer;
    timer->start(3000);

    connect(timer,SIGNAL(timeout()),this,SLOT(handle_timer()));
}

void MyImage::setupActions()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(on_okBtn_clicked()));
}

void MyImage::on_okBtn_clicked()
{
    this->close();
}

void MyImage::handle_timer()
{
    qDebug() << "kang";
    QString path = QDir::currentPath() + QString("/") + QString(name);
    image->setPixmap(QPixmap(path).scaled(QSize(370,432),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    timer->stop();
}

void MyImage::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void MyImage::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}
