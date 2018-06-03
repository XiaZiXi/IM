#include "regist.h"
#include <QIcon>
#include <QSize>
#include <QPalette>
#include <QDebug>
#include <QMessageBox>
#include <QGroupBox>
#include <QPixmap>

Regist::Regist(QWidget *parent,QString serverIP,USERID_2T serverPORT,QString myIP)
    : QWidget(parent),serverIP(serverIP),serverPORT(serverPORT),myIP(myIP)
{
    qDebug() << "new a Regist";
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress(myIP),TEMP_PORT_REGIST,QAbstractSocket::ShareAddress);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
    qDebug() << "***********port" << serverPORT;
    qDebug() << "*******serverip" << serverIP;
    qDebug() << "***********myip" << myIP;
    setupUI();
    this->nickName->setText("小慧");
    this->nickName->setFocus();
    this->nickName->selectAll();
    setupActions();
}

Regist::~Regist()
{
    qDebug() << "~Regist()";
}

void Regist::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}

void Regist::updateBtnClicked()
{
    //最开始，检验是否填写了全部的基本信息
    if(password->text().isEmpty() || confirmPassword->text().isEmpty()
            || name->text().isEmpty() || age->text().isEmpty() || nickName->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请完善昵称以及基本信息！");
        return;
    }
    //开始，先检验本地的 密码和校验密码相符
    if(this->password->text() != this->confirmPassword->text())
    {
        QMessageBox::warning(this,"密码冲突","两次输入的密码不一致");
        this->confirmPassword->setText("");
        this->password->setFocus();
        this->password->selectAll();
        return;
    }
    //发送注册信息给服务器
    sendRegistToServer();
}

void Regist::cleanAll()
{
    this->nickName->setText("");
    this->name->setText("");
    this->age->setText("");
    this->password->setText("");
    this->confirmPassword->setText("");
    this->telephone->setText("");
    this->email->setText("");
    this->department->setText("");
    this->hometown->setText("");

    this->nickName->setFocus();
    this->close();
}

void Regist::receiveMsg()
{
    //读到数据先解析数据包头的十个字节
    qDebug() << "收到来自服务器的注册成败消息";
    QByteArray data;
    while(udpSocket->hasPendingDatagrams())
    {
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(),data.size());
        qDebug() << "我只执行了一遍！！";//只是执行了一遍而已
    }
    QDataStream datastream(&data,QIODevice::ReadWrite);

    DataHead_18T dataHead;
    datastream >> dataHead;//读取数据包头共十个字节

    USERID_2T id = dataHead.getid();
    ANY_2T any2 = dataHead.getsegment();

    qDebug() << "注册成功，获得id为" << id;
    QString dataBody;
    datastream >> dataBody;
    switch(any2)
    {
    case SUCCESSFUL_2V :
        //成功注册
        qDebug() << "注册成功";
        QMessageBox::information(this,"恭喜成功注册!",QString("您的ID是") + QString::number(id) + QString("请妥善保管哦！"));
        break;
    case FAILURE_2V    :
        //失败注册
        qDebug() << "注册失败";
        QMessageBox::warning(this,"注册失败啦",dataBody);
        break;
    default            :
        //未知消息
        qDebug() << "不属于我想要接收的注册消息！";
        break;
    }
    delete this;//销毁注册界面
}
void Regist::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}


void Regist::setupUI()
{
    this->setFixedSize(280,640);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/Regist.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
        //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/Regist_bkg.jpg")));//背景图片
    this->setPalette(palette);

//1.hlayout1 退出和最小化
    //退出
    hideBtn = new QToolButton(this);
    hideBtn->setToolTip("最小");
    hideBtn->setFixedSize(30,30);
    hideBtn->setIcon(QIcon(":/Regist_hide.png"));
    //最小化
    exitBtn = new QToolButton(this);
    exitBtn->setToolTip("退出");
    exitBtn->setFixedSize(30,30);
    exitBtn->setIcon(QIcon(":/Regist_exit.png"));
    //hspacer
    hSpacer1 = new QSpacerItem(250,30);

    hlayout1 = new QHBoxLayout;
    hlayout1->addItem(hSpacer1);
    hlayout1->addWidget(hideBtn);
    hlayout1->addWidget(exitBtn);

//2.hlayout2    nickpicture and nickname
    //picture
    nickNamePicture = new QLabel;
    nickNamePicture->setFixedSize(60,60);
    nickNamePicture->setPixmap(QPixmap(":/nickname.png").scaled(QSize(60,60),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    //label and edit
    nickNameLabel = new QLabel("昵  称");
    nickNameLabel->setFont(QFont("汉仪南宫体简",13,200));
    nickName = new QLineEdit;
    nickName->setFont(QFont("汉仪南宫体简",13,200));
    QHBoxLayout *nicklayout = new QHBoxLayout;
    nicklayout->addWidget(nickNameLabel);
    nicklayout->addWidget(nickName);


    hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(nickNamePicture);
    hlayout2->addLayout(nicklayout);
    hlayout2->addSpacing(2);

//3.basic
    QGroupBox *basic = new QGroupBox("基本信息");
    QFont *font1 = new QFont;
    font1->setBold(true);
    font1->setFamily("汉仪南宫体简");
    font1->setPointSize(13);
    basic->setFont(*font1);

    QVBoxLayout *basiclayout = new QVBoxLayout;
    //姓名 年龄 密码 确认密码
    nameLabel = new QLabel("姓  名");
    ageLabel = new QLabel("年  龄");
    passwordLabel = new QLabel("密  码");
    confirmPasswordLabel = new QLabel("确  认");

    name = new QLineEdit;
    age = new QLineEdit;
    password = new QLineEdit;
    password->setEchoMode(QLineEdit::Password);
    confirmPassword = new QLineEdit;
    confirmPassword->setEchoMode(QLineEdit::Password);

    QHBoxLayout *l1 = new QHBoxLayout;
    QHBoxLayout *l2 = new QHBoxLayout;
    QHBoxLayout *l3 = new QHBoxLayout;
    QHBoxLayout *l4 = new QHBoxLayout;

    l1->addWidget(nameLabel);
    l1->addWidget(name);
    l2->addWidget(ageLabel);
    l2->addWidget(age);
    l3->addWidget(passwordLabel);
    l3->addWidget(password);
    l4->addWidget(confirmPasswordLabel);
    l4->addWidget(confirmPassword);

    basiclayout->addLayout(l1);
    basiclayout->addLayout(l2);
    basiclayout->addLayout(l3);
    basiclayout->addLayout(l4);

    basic->setLayout(basiclayout);

//4.more
    QGroupBox *more  = new QGroupBox("更多信息");
    QFont *font2 = new QFont;
    font2->setBold(true);
    font2->setFamily("汉仪南宫体简");
    font2->setPointSize(13);
    more->setFont(*font2);


    QVBoxLayout *morelayout = new QVBoxLayout;
    //电话 邮箱 部门 故乡
    telephoneLabel = new QLabel("电  话");
    emailLabel = new QLabel("邮  箱");
    departmentLabel = new QLabel("部  门");
    hometownLabel = new QLabel("故  乡");

    telephone = new QLineEdit;
    email = new QLineEdit;
    department = new QLineEdit;
    hometown = new QLineEdit;

    QHBoxLayout *l5 = new QHBoxLayout;
    QHBoxLayout *l6 = new QHBoxLayout;
    QHBoxLayout *l7 = new QHBoxLayout;
    QHBoxLayout *l8 = new QHBoxLayout;

    l5->addWidget(telephoneLabel);
    l5->addWidget(telephone);
    l6->addWidget(emailLabel);
    l6->addWidget(email);
    l7->addWidget(departmentLabel);
    l7->addWidget(department);
    l8->addWidget(hometownLabel);
    l8->addWidget(hometown);

    morelayout->addLayout(l5);
    morelayout->addLayout(l6);
    morelayout->addLayout(l7);
    morelayout->addLayout(l8);

    more->setLayout(morelayout);

//5.spacer
    vSpacer1 = new QSpacerItem(20,20);

//6.hlayout3  更新 取消
    hlayout3 = new QHBoxLayout;

    updateBtn = new QPushButton("更  新");
    updateBtn->setFixedHeight(35);
    updateBtn->setFont(QFont("汉仪南宫体简",15,255));

    cancelBtn = new QPushButton("取  消");
    cancelBtn->setFixedHeight(35);
    cancelBtn->setFont(QFont("汉仪南宫体简",15,255));

    hlayout3->addWidget(updateBtn);
    hlayout3->addWidget(cancelBtn);

//7.spacer
    vSpacer2 = new QSpacerItem(40,40);

    mainlayout = new QVBoxLayout(this);
    mainlayout->addLayout(hlayout1);
    mainlayout->addLayout(hlayout2);
    mainlayout->addWidget(basic);
    mainlayout->addWidget(more);
    mainlayout->addItem(vSpacer1);
    mainlayout->addLayout(hlayout3);
    mainlayout->addItem(vSpacer2);

    setupDefault();
}

void Regist::setupDefault()
{
    this->name->setText(QString("小熙"));
    this->age->setText(QString("20"));
    this->telephone->setText(QString("000****0000"));
    this->email->setText(QString("000****000@qq.com"));
    this->department->setText(QString("研发"));
    this->hometown->setText(QString("中国"));
}

void Regist::setupActions()
{
    connect(hideBtn,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));//隐藏
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(cleanAll()));//退出

    connect(updateBtn,SIGNAL(clicked(bool)),this,SLOT(updateBtnClicked())) ;//更新信息
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cleanAll()));//取消就是退出
}

void Regist::sendRegistToServer()//向服务器发送注册消息
{
    qDebug() << "sendRegistToServer()";

    QString serverip = getServerIP();
    qDebug() << serverip;
    USERID_2T serverport = getServerPORT();
    qDebug() << serverport;
    QString myip = getMyIP();
    qDebug() << myip;

    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);

    USERID_2T id = 0;
    QString nickname = this->nickName->text();
    QString name = this->name->text();
    QString age = this->age->text();
    QString password = this->password->text();
    QString telephony = this->telephone->text();
    QString department = this->department->text();
    QString email = this->email->text();
    QString hometown = this->hometown->text();

    REQUEST_REGIST_CTOS registRequest = getREQUEST_REGIST_CTOS(id,nickname,name,age,password,telephony,department,email,hometown);
    registRequest.head.setmyip(getIPFromString(this->getMyIP()));//把我自己的IP发送过去，以便于对方发送消息回来
    datastream << registRequest.head;
    datastream << registRequest.body;
    udpSocket->writeDatagram(bytearray,QHostAddress(serverip),serverport);
    qDebug() << registRequest.head.getcmd() << "*" << registRequest.head.gethisip() << "*"
             << registRequest.head.getid() << "*" <<  registRequest.head.getmyip() << "*"
             << registRequest.head.getreserve() << "*" << registRequest.head.getsegment() << "*"
             << registRequest.body.getage();
    qDebug() << "serverip" << serverip << "serverport" << serverport << "myip" << myip
             << "myip:" << getIPFromInt(registRequest.head.getmyip());
}

QString Regist::getServerIP()
{
    return serverIP;
}
USERID_2T Regist::getServerPORT()
{
    return serverPORT;
}

QString Regist::getMyIP()
{
    return myIP;
}
