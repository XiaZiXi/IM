#include "login.h"
#include <QIcon>
#include <QSize>
#include <QPalette>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "mainwindow.h"
#include <QTimer>
#include "logincheck.h"
#include <QThread>
#include "const.h"

#include "setserver.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    setupDefault();
    setupUI();
    setupActions();
}

Login::~Login()
{

}

void Login::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();

    QString tooltip = QString("服务器IP : ") + QString(setServer->getServerIP())
            + QString("\n") + QString("服务器PORT : ") + QString(setServer->getServerPORT())
            + QString("\n") + QString("本地IP : ") + QString(setServer->getMyIP());
    seting->setToolTip(tooltip);
}

void Login::loginBtnClicked()
{
    QString serverip = setServer->serverIP->text();
    qDebug() << serverip;
    this->serverIP = serverip;
    QString serverport = setServer->serverPORT->text();
    qDebug() << serverport;
    this->serverPORT = serverport.toInt();
    QString myip = setServer->myIP->text();
    qDebug() << myip;
    this->myIP = myip;

    //全局变量
    MyIPAddress::myipaddress = myip;



    udpsocket->bind(QHostAddress(myIP),TEMP_PORT_LOGIN,QAbstractSocket::ShareAddress);

    //判定是否输入了用户名和密码
    if(ID->text().isEmpty() || PD->text().isEmpty())
    {
        QMessageBox::warning(this,"错误","请输入用户名和密码!");
        return;
    }
    //发送登录信息给服务端
    sendLoginToServer();

}

void Login::setingBtnClicked()
{
    seting->setEnabled(false);
    //qDebug() << "%%%" << setServer->getServerIP();
    //打开设置界面，在其中收集数据，把数据记录下来
    //setServer = new SetServer;
    setServer->show();

    login->setEnabled(true);
    regist->setEnabled(true);

}

void Login::registBtnClicked()
{
    QString serverip = setServer->serverIP->text();
    qDebug() << serverip;
    this->serverIP = serverip;
    QString serverport = setServer->serverPORT->text();
    qDebug() << serverport;
    this->serverPORT = serverport.toInt();
    QString myip = setServer->myIP->text();
    qDebug() << myip;
    this->myIP = myip;

    //udpsocket->bind(QHostAddress(myIP),TEMP_PORT_REGIST,QAbstractSocket::ShareAddress);

    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!port" << getServerPORT();
    //现在才把注册界面创建起来
    registered = new Regist(NULL,getServerIP(),getServerPORT(),getMyIP());
    registered->updateBtn->setText("注册");
    registered->hide();
    //打开注册界面
    registered->show();
}


void Login::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}

void Login::setupDefault()
{
    setServerIP(QString("192.168.252.1"));
    setServerPORT(9999);
    setMyIP(QString(""));

    setServer = new SetServer;
    //registered = new Regist;

    udpsocket = new QUdpSocket(this);
    //udpsocket->bind(QHostAddress(myIP),TEMP_PORT_LOGIN,QAbstractSocket::ShareAddress);

    //registered->updateBtn->setText("注册");
    //registered->hide();
}

void Login::setupUI()
{
    this->setFixedSize(280,640);//设置固定大小(黄金分割！370:600但是不好看)280 600
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/Login.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）44 99 ee ff
        //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/Login_bkg.jpg")));//背景图片
    this->setPalette(palette);

//0.vspacer
    vSpacer5 = new QSpacerItem(30,35);
//1.hlayout1 退出和最小化
    //退出
    hideBtn = new QToolButton(this);
    hideBtn->setToolTip("最小");
    hideBtn->setFixedSize(30,30);
    hideBtn->setIcon(QIcon(":/hide.png"));
    //最小化
    exitBtn = new QToolButton(this);
    exitBtn->setToolTip("退出");
    exitBtn->setFixedSize(30,30);
    exitBtn->setIcon(QIcon(":/exit.png"));
    //hspacer
    hSpacer1 = new QSpacerItem(160,30);

    hlayout1 = new QHBoxLayout;
    hlayout1->addItem(hSpacer1);
    hlayout1->addWidget(hideBtn);
    hlayout1->addWidget(exitBtn);

//2.vSpacer1
    vSpacer1 = new QSpacerItem(30,90);

//3.vlayout1 ID PD 登录
    //ID
    IDLabel = new QLabel("ID:",this);
    IDLabel->setFixedSize(30,30);
    IDLabel->setFont(QFont("汉仪南宫体简",13,255));
    ID = new QLineEdit;
    ID->setPlaceholderText("请输入会员ID");
    ID->setFont(QFont("汉仪南宫体简",13,200));
    ID->setFixedSize(180,30);
    hSpacer2 = new QSpacerItem(30,60);
    hSpacer8 = new QSpacerItem(30,60);
    hlayout2 = new QHBoxLayout;
    hlayout2->addItem(hSpacer2);
    hlayout2->addWidget(IDLabel);
    hlayout2->addWidget(ID);
    hlayout2->addItem(hSpacer8);

    //PD
    PDLabel = new QLabel("PD:",this);
    PDLabel->setFixedSize(30,30);
    PDLabel->setFont(QFont("汉仪南宫体简",13,255));
    PD = new QLineEdit;
    PD->setPlaceholderText("请输入会员密码");
    PD->setEchoMode(QLineEdit::Password);
    PD->setFont(QFont("汉仪南宫体简",13,200));
    PD->setFixedSize(180,30);
    hSpacer3 = new QSpacerItem(30,60);
    hSpacer9 = new QSpacerItem(30,60);
    hlayout3 = new QHBoxLayout;
    hlayout3->addItem(hSpacer3);
    hlayout3->addWidget(PDLabel);
    hlayout3->addWidget(PD);
    hlayout3->addItem(hSpacer9);

    //登录
    login = new QPushButton("登录");
    login->setFont(QFont("汉仪南宫体简",16,255));
    login->setFixedSize(70,40);
    hSpacer7 = new QSpacerItem(20,30);
    hSpacer10 = new QSpacerItem(20,30);
    hlayout6 = new QHBoxLayout;
    hlayout6->addItem(hSpacer7);
    hlayout6->addWidget(login);
    hlayout6->addItem(hSpacer10);

    hSpacer6 = new QSpacerItem(30,40);
    vlayout1 = new QVBoxLayout;
    vlayout1->addLayout(hlayout2);
    vlayout1->addLayout(hlayout3);
    vlayout1->addItem(hSpacer6);
    vlayout1->addLayout(hlayout6);

//4.vSpacer2
    vSpacer2 = new QSpacerItem(30,150);

//5.vlayout2 注册和设置
    //注册
    regist = new QPushButton(QIcon(":Login_register.png"),"");
    regist->setFixedSize(50,30);
    registLabel = new QLabel("申请新会员");
    registLabel->setFont(QFont("汉仪南宫体简",14,255));
    hSpacer4 = new QSpacerItem(45,30);
    hlayout4 = new QHBoxLayout;
    hlayout4->addItem(hSpacer4);
    hlayout4->addWidget(regist);
    hlayout4->addWidget(registLabel);

    //spacer
    vSpacer4 = new QSpacerItem(30,20);

    //设置
    seting = new QPushButton(QIcon(":Login_setting.png"),"");
    seting->setFixedSize(50,30);
    setingLabel = new QLabel("设置");
    setingLabel->setFont(QFont("汉仪南宫体简",14,255));
    hSpacer5 = new QSpacerItem(45,30);
    hlayout5 = new QHBoxLayout;
    hlayout5->addItem(hSpacer5);
    hlayout5->addWidget(seting);
    hlayout5->addWidget(setingLabel);

    vlayout2 = new QVBoxLayout;
    vlayout2->addLayout(hlayout5);
    vlayout2->addItem(vSpacer4);
    vlayout2->addLayout(hlayout4);

//6.vSpacer3
    vSpacer3 = new QSpacerItem(30,120);


//主布局
    mainlayout = new QVBoxLayout;
    mainlayout->addItem(vSpacer5);
    mainlayout->addLayout(hlayout1);
    mainlayout->addItem(vSpacer1);
    mainlayout->addLayout(vlayout1);
    mainlayout->addItem(vSpacer2);
    mainlayout->addLayout(vlayout2);
    mainlayout->addItem(vSpacer3);
    this->setLayout(mainlayout);
}

void Login::setupActions()
{
    connect(hideBtn,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));//隐藏
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(close()));//退出

    login->setEnabled(false);
    regist->setEnabled(false);

    connect(login,SIGNAL(clicked(bool)) ,this,SLOT(loginBtnClicked())) ;//登录
    connect(regist,SIGNAL(clicked(bool)),this,SLOT(registBtnClicked()));//注册
    connect(seting,SIGNAL(clicked(bool)),this,SLOT(setingBtnClicked()));//设置

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(receiveMsg()));//收到服务器的登录成败消息的处理
}

void Login::setServerIP(QString serverIP)
{
    this->serverIP = serverIP;
}

QString Login::getServerIP()
{
    return setServer->getServerIP();
}

void Login::setMyIP(QString myIP)
{
    this->myIP = myIP;
}

QString Login::getMyIP()
{
    return myIP;
}

void Login::setServerPORT(int serverPORT)
{
    this->serverPORT = serverPORT;
}

int Login::getServerPORT()
{
    return setServer->getServerPORT().toInt();
}

void Login::sendLoginToServer()//向服务器发送登录信息
{
    qDebug() << "sendLoginToServer()";

    QString serverip = setServer->serverIP->text();
    this->serverIP = serverip;
    qDebug() << serverip;
    QString serverport = setServer->serverPORT->text();
    this->serverPORT = serverport.toInt();
    qDebug() << serverport;
    QString myip = setServer->myIP->text();
    this->myIP = myip;
    qDebug() << myip;

    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);

    LOGIN_REQUEST_CTOS loginRequest = getLOGIN_REQUEST_CTOS(ID->text().toInt(),PD->text());
    loginRequest.head.setmyip(getIPFromString(myip));
    datastream << loginRequest.head;
    datastream << loginRequest.body;
    udpsocket->writeDatagram(bytearray,QHostAddress(serverip),serverport.toInt());
}

void Login::receiveMsg()
{
#if 0
    //正式主窗口登场前，先搞一个小动画//发现搞不了
    QThread m;
    LoginCheck login;
    login.show();
    QTimer timer;
    timer.start(4000);
    QObject::connect(&timer,SIGNAL(timeout()),&login,SLOT(hide()));
    login.moveToThread(&m);
    m.start();
#endif

    qDebug() << "收到服务器的登录成败消息!!";
    //读到数据先解析数据包头的十个字节
    qDebug() << "my personal data process!";
    QByteArray data;
    while(udpsocket->hasPendingDatagrams())
    {
        data.resize(udpsocket->pendingDatagramSize());
        udpsocket->readDatagram(data.data(),data.size());
        qDebug() << "我只执行了一遍！！";//只是执行了一遍而已
    }
    QDataStream datastream(&data,QIODevice::ReadWrite);

    DataHead_18T dataHead;
    datastream >> dataHead;//读取数据包头共十个字节

    USERID_2T userid = dataHead.getid();//这个东西要好好记录下来，为多客户端做准备啦！！！！！！！！！！
    qDebug() << "###########userid = " << userid;
    ANY_2T any2 = dataHead.getsegment();

    RegistRequest selfinfo;
    datastream >> selfinfo;
    QString reasonOfLoginFailed = selfinfo.getnickname();//原因在于这里的第一个字段留作他用了

    qDebug() << "消息的内容如下："
             << "cmd:" << dataHead.getcmd()
             << "id:" << dataHead.getid()
             << "res:" << dataHead.getreserve()
             << "seg:" << dataHead.getsegment()
             << "body[1]:" << selfinfo.getnickname();
    switch(dataHead.cmd)
    {
    case LOGIN_CMD_2V_STOC :
        if(FAILURE_2V == any2)
        {
            QThread::sleep(2);

            //登录失败啦
            QMessageBox::warning(this,"登录失败",reasonOfLoginFailed);
            ID->setText("");
            PD->setText("");
            ID->setFocus();
        }
        else if(SUCCESSFUL_2V == any2)
        {
            QThread::sleep(2);

            //登录成功啦
            QMessageBox::information(this,"恭喜","登录成功!");
            this->hide();
            qDebug() << "已经登录成功！我传递给他的ip和port为" << getServerIP() << " : " << getServerPORT();

            MainWindow *w =new MainWindow(getServerIP(),getServerPORT(),userid,getMyIP(),selfinfo,NULL);//我们的主窗口闪亮登场啦！！！！！！！！！
            w->show();
            qDebug() << "i have get the port :" << w->getPort();
        }
        break;
    default :
        qDebug() << "我收到来自服务端的登录成败消息，但是无法解析命令";
        break;
    }
}
