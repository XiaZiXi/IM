#include "mainwindow.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include "regist.h"
#include <QColorDialog>
#include <QScrollBar>
#include <QListWidgetItem>
#include "about.h"
#include "sqls.h"
#include "const.h"
#include <QMessageBox>

MainWindow::MainWindow(QString serverIP,USERID_2T serverPORT,USERID_2T port,QString myIP,RegistRequest selfinfo,QWidget *parent)
    : serverIP(serverIP),serverPORT(serverPORT),port(port),myIP(myIP),selfinfo(selfinfo),QWidget(parent)
{
#if 0
    QMessageBox::information(this,"kkk",selfinfo.age + selfinfo.department +
                             selfinfo.email + selfinfo.hometown +
                             selfinfo.name + selfinfo.nickname +
                             selfinfo.telephone + selfinfo.password);
#endif
    setupUI();
    setupNetwork();
    setupActions();
    setupDatabases();//初始化数据库
    updataLocalDB();//更新从服务器发送过来的数据
    loadDataToUI();//从本地加载数据到界面上
}

MainWindow::~MainWindow()
{
    myExit();
    qDebug() << "我已经被销毁了~MainWindow()";
}

void MainWindow::setupUI()
{
    this->setFixedSize(280,680);//设置固定大小(黄金分割！以前是280,600)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/MainWindow.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);

//1.hlayout1 退出和最小化
    QSpacerItem *spacer = new QSpacerItem(200,30);
    //退出
    hideBtn = new QToolButton(this);
    hideBtn->setToolTip("最小");
    hideBtn->setFixedSize(30,30);
    hideBtn->setIcon(QIcon(":/MainWindow_hide.png"));
    //最小化
    exitBtn = new QToolButton(this);
    exitBtn->setToolTip("退出");
    exitBtn->setFixedSize(30,30);
    exitBtn->setIcon(QIcon(":/MainWindow_exit.png"));

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addItem(spacer);
    hlayout1->addWidget(hideBtn);
    hlayout1->addWidget(exitBtn);
//2.mainwindowup
    mainWindowUp = new MainWindowUp(port,selfinfo,NULL,serverIP,serverPORT,myIP);
//3.mainwindowdown
    mainWindowDown = new MainWindowDown(selfinfo,NULL,serverIP,serverPORT,myIP,port,myIP);
    //mainWindowDown->setStyleSheet("background-color: rgb(128,255,255)");//5-20修改
    mainWindowDown->setStyleSheet("background-color: rgb(142,194,245)");//5-31修改

    QSpacerItem *spaceLeft = new QSpacerItem(10,10);
    QSpacerItem *spaceRight = new QSpacerItem(10,10);
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addItem(spaceLeft);
    hlayout2->addWidget(mainWindowDown);
    hlayout2->addItem(spaceRight);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QSpacerItem *spaceBottom = new QSpacerItem(30,30);
    mainlayout->setAlignment(Qt::AlignTop);
    mainlayout->addLayout(hlayout1);
    mainlayout->addWidget(mainWindowUp);
    mainlayout->addLayout(hlayout2);
    mainlayout->addItem(spaceBottom);

}

void MainWindow::setupNetwork()
{
    udpsocket = new QUdpSocket(this);
    udpsocket->bind(QHostAddress(myIP),getPort(),QAbstractSocket::ShareAddress);
    qDebug() << "初始化网络。。。。。。。。port:" << getPort();
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}

void MainWindow::readMsg()//只是负责接收消息，以及负责向服务器发送消息
{
    qDebug() << "我收到消息了......";
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

    USERID_2T userid = dataHead.getid();
    ANY_2T any2 = dataHead.getsegment();
    ANY_4T any4 = dataHead.getreserve();
    IP_4T friendip = dataHead.gethisip();

    QString password;
    RegistRequest registrequest;
    QString empty;
    QString updateinfo;
    RecommandFriendInformation recommandfriendinformation;
    QString empty2;
    QString nick;

    switch(dataHead.cmd)
    {
    case UPDATE_PERSONAL_INFORMATION_CMD_2V_STOC :
        //更新个人信息
        qDebug() << "更新个人信息";
        datastream >> updateinfo;
        handleUpdatePersonalInfo(dataHead.getsegment(),updateinfo);
        break;
    case LOGOUT_CMD_2V_STOC :
        qDebug() << "登出告知好友";
        //QString empty;
        datastream >> empty;
        //handleLogout(userid);
        QMessageBox::information(this,"通知",QString("好友")+QString::number(userid)+QString("已经下线!"));
        break;
    case SEARCH_FRIEND_CMD_2V_STOC :
        //推荐好友
        qDebug() << "推荐好友";
        //RecommandFriendInformation recommandfriendinformation;
        datastream >> recommandfriendinformation;
        //handleSearchFriend(userid,recommandfriendinformation);
        break;
    case ADD_FRIEND_CMD_2V_STOC :
        //添加好友
        qDebug() << "添加好友";
        //QString empty2;
        datastream >> empty;//第一个放""或者失败的原因
        datastream >> empty2;//第二个放好友昵称
        handleAddFriend(userid,any2,empty,empty2,friendip);
        break;
    case LOGIN_TELL_FRIEND_CMD_2V_STOC :
        //登入告知好友
        qDebug() << "登入告知好友";
        break;
    case SEND_MSG_CMD_2V_CTOC :
        //客户端到客户端的消息
        qDebug() << "客户端到客户端的基本消息";
        break;
    case ADD_FRI_CMD_2V_CTOC :
        //客户端到客户端的消息
        datastream >> nick;
        qDebug() << "客户端到客户端添加好友消息";
        handleAddFriendCTOC(userid,nick,friendip);
        break;
    case SYSMSG_CMD_2V_STOC :
        //服务端系统消息
        datastream >> empty;
        qDebug() << "服务端的系统消息";
        QMessageBox::information(this,"系统消息",empty);
        break;
    default :
        QMessageBox::warning(this,"致命的错误","无法识别的命令！");
        qDebug() << "无法知道的命令";
        break;
    }
}

void MainWindow::myExit()
{
    //退出之前要先向服务器发送登出消息.
    qDebug() << "退出之前向服务器发送登出消息";
    LOGOUT_CTOS logout = getLOGOUT_CTOS(this->port);//由于port号就是ID
    logout.head.setmyip(getIPFromString(this->getMyIP()));

    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << logout.head;
    datastream << logout.body;
    qDebug() << "cmd:" << logout.head.getcmd() << " "
             << "id:" << logout.head.getid() << " "
             << "res:" << logout.head.getreserve() << " "
             << "seg:" << logout.head.getsegment() << " "
             << "body:" << logout.body;
    udpsocket->writeDatagram(bytearray,QHostAddress(getServerIP()),getServerPORT());//这里LocalHost写死了，升级的地方

    //udpsocket->writeDatagram("kangchao",QHostAddress("192.168.252.1"),(quint16)9999);
    qDebug() << "serverIP:" << getServerIP() << "serverPORT:" << getServerPORT();
    this->close();
}

USERID_2T MainWindow::getPort()
{
    return port;
}

QString MainWindow::getServerIP()
{
    return serverIP;
}

USERID_2T MainWindow::getServerPORT()
{
    return serverPORT;
}

QString MainWindow::getMyIP()
{
    return myIP;
}

void MainWindow::CTOC_sendMsgBecauseOfAdd(QString nickname, USERID_2T userid,USERID_2T friendid,QString friendIP,QString myip)
{
    ADD_FRI_CTOC add = getADD_FRI_CTOC(userid,nickname);
    add.head.sethisip(getIPFromString(myip));

    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << add.head;
    datastream << add.body;
    qDebug() << "cmd:" << add.head.getcmd() << " "
             << "id:" << add.head.getid() << " "
             << "res:" << add.head.getreserve() << " "
             << "seg:" << add.head.getsegment() << " "
             << "body:" << add.body << " "
             << "friendid:" << friendid;
    udpsocket->writeDatagram(bytearray,QHostAddress(friendIP),friendid);//这里LocalHost写死了，升级的地方

}

//被动添加
void MainWindow::handleAddFriendCTOC(USERID_2T friendid, QString nickname,IP_4T friendip)
{
    QMessageBox::information(this,"添加好友",nickname + QString(" ") + QString::number(friendid) + QString(" ") + QString("添加你为好友！"));
    //将该好友添加至UI
    OneHuman *human = new OneHuman(friendid,getMyIP(),getPort(),nickname,friendip,NULL);
    qDebug() << "handleAddFriend:" << friendid << getPort() << nickname;
    this->mainWindowDown->oneListLayout->addWidget(human);
    //更新自己的数据表
    QString friendIP = getIPFromInt(friendip);
    addOneFriendOnFriendList(query,getPort(),friendid,friendIP,nickname);
}

void MainWindow::handleUpdatePersonalInfo(FAILURE_SUCCEED_2T result, QString errorReason)
{
    switch(result)
    {
    case SUCCESSFUL_2V :
    {
        QMessageBox::information(this,"恭喜","更新个人信息成功！");
        //更新自己的界面(其实只是更新了nickanme)
        qDebug() << "globabnickname :::::: " << Const::GlobalName;
        this->mainWindowUp->nickNameLabel->setText(Const::GlobalName);
        //更新自己的selfinf表
        if(!(ConstUpdate::age.isEmpty()))
        {
            updateSelfInfoTable(ConstUpdate::age,
                            ConstUpdate::department,
                            ConstUpdate::email,
                            ConstUpdate::hometown,
                            ConstUpdate::name,
                            ConstUpdate::nickname,
                            ConstUpdate::password,
                            ConstUpdate::telephony,
                            query,getPort());
        }
        qDebug() << "更新个人信息成功！";
        break;
    }
    case FAILURE_2V :
        QMessageBox::information(this,"抱歉","更新个人信息失败！");
        break;
    default :
        QMessageBox::information(this,"抱歉","不是我想要接收到的更新个人信息！");
        break;
    }
}


//主动添加
void MainWindow::handleAddFriend(USERID_2T userid, ANY_2T friendid, QString errorReason,QString friendnickname,IP_4T friendip)
{
    if(errorReason.isEmpty())
    {
        qDebug() << "添加好友时没有任何错误！" << "好友昵称" << friendnickname << friendid;
        qDebug() << "好友ip :" << getIPFromInt(friendip);
        //更新自己的好友关系表
        addOneFriendOnFriendList(query,userid,friendid,getIPFromInt(friendip),friendnickname);
        //在UI中添加该好友
        OneHuman *human = new OneHuman(friendid,getMyIP(),getPort(),friendnickname,friendip,NULL);
        qDebug() << "handleAddFriend:" << friendid << getPort() << friendnickname;
        this->mainWindowDown->oneListLayout->addWidget(human);
        //发送消息给好友，让他更新自己的界面和好友关系表
        CTOC_sendMsgBecauseOfAdd(getNickname(query,userid),userid,friendid,getIPFromInt(friendip),getMyIP());//将自己的昵称和id和ip发给好友
        //弹出对话框
        QMessageBox::information(this,"恭喜","添加好友成功!");
    }
    else
    {
        QMessageBox::warning(this,"抱歉",errorReason);
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}

void MainWindow::setupActions()
{
    connect(hideBtn,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));//隐藏
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(myExit()));//自己的退出

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(readMsg()));//只是负责读取消息和向服务器发送信息
}

void MainWindow::setupDatabases()
{
    //核心代码
    //必须在服务器端的主界面上   1创建数据库的连接   2设置数据库的名字   3打开数据库  4创建执行数据库的对象  5把表创建好
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("client.db");
    if(db.open())
        qDebug() << "open server.db ok";
    else
        qDebug() << "open server.db failed";


    query = new QSqlQuery(db);


    if(query->exec("create table selfinfo(myid integer,nickname text not null,name text not null,age text not null,password text not null,telephone text,department text,email text,hometown text)"))//自动增长id字段
        qDebug() << "create table selfinfo ok";
    else
        qDebug() << "create table selfinfo failed";


    if(query->exec("create table friendlist(myid integer not null,friendid integer not null,friendip text not null,friendnickname text not null)"))
        qDebug() << "create table friendlist ok";
    else
        qDebug() << "create table friendlist failed";
#if 0
    //测试sql.h中的静态函数是否能够成功插入到数据库
    addSelfInfoInDB(query,10001,"kang","huzhihui","20","123356","15881432014","研发","123@qq.com","四川");
    addFriendListInDB(query,10003,10001);
#endif

}

void MainWindow::updataLocalDB()
{
    //更新从服务器发送回来的数据
    int myid = port;
    QString age = selfinfo.getage();
    QString department = selfinfo.getdepartment();
    QString email = selfinfo.getemail();
    QString hometown = selfinfo.gethometown();
    QString name = selfinfo.getname();
    QString nickname = selfinfo.getnickname();
    QString password = selfinfo.getpassword();
    QString telephone = selfinfo.gettelephone();
    qDebug() << "更新从服务端的信息" <<myid<<age<<department<<email<<hometown<<name<<nickname<<password<<telephone;
    //由于发送回来的个人信息已经在对象构造出来的时候就有了的，所以只需要将自己的成员对象更新进入selfinfo表即可
    //先判定是否是第一次登陆，如果是，应该将数据插入，如果不是应该将数据更新
    if(clientIsExist(query,myid))
        return;
    else
        addSelfInfoInDB(query,myid,nickname,name,age,password,telephone,department,email,hometown);
}

void MainWindow::loadDataToUI()
{
    //将已经添加过的好友添加至UI
    //从数据表中查找好友
    //来不及了，先不管了，这个地方纯粹是为了应付啊！！！！！！！！！！！！！
    for(int i = 10001;i < 10100;++ i)//这个10100用得不好呀
    {
        if(friendIsExist(query,i))
        {
            QString friendIP = getFriendIP(query,i);
            int friendip = getIPFromString(friendIP);
            QString friendnickname = getFriendNickname(query,i);
            OneHuman *human = new OneHuman(i,getMyIP(),getPort(),friendnickname,friendip,NULL);
            //qDebug() << "handleAddFriend:" << friendid << getPort() << nickname;
            this->mainWindowDown->oneListLayout->addWidget(human);
        }
    }
}












MainWindowUp::MainWindowUp(USERID_2T selfport,RegistRequest selfinfo,QToolButton *parent,QString serverIP,int serverPORT,QString myIP)
    :selfport(selfport),selfinfo(selfinfo),QToolButton(parent),serverIP(serverIP),serverPORT(serverPORT),myIP(myIP)
{
    qDebug() << "初始化MainWindowUp  " << "selfport :" << selfport;
    setupUI();
    setupActions();
}

void MainWindowUp::setupUI()
{
   nickNameIcon = new QLabel;
   int num = selfport - 10000;
   QString pic = QString(":/")+QString::number(num)+QString("ON.bmp");
   qDebug() << "图片的名字是:" << pic;
   nickNameIcon->setPixmap
           (QPixmap(pic).
            scaled(QSize(40,56),//48,48
                   Qt::IgnoreAspectRatio,
                   Qt::SmoothTransformation));   //默认的头像图标
   //nickNameLabel = new QLabel("小女娃子");         //默认的昵称
   nickNameLabel = new QLabel(selfinfo.getnickname());
   nickNameLabel->setFixedSize(200,24);
   nickNameLabel->setAlignment(Qt::AlignLeft);
   nickNameLabel->setFont(QFont("汉仪南宫体简",15,255));
   this->setFixedSize(258,64);
   this->setAutoRaise(true);

   QHBoxLayout *layout = new QHBoxLayout(this);
   layout->addWidget(nickNameIcon);
   layout->addWidget(nickNameLabel);
}

void MainWindowUp::setupActions()
{
}


void MainWindowUp::mouseDoubleClickEvent(QMouseEvent *)
{
//弹出一个更新框，更新个人资料
    //这里不足之处有 1不能弹出模式对话框，也就是说只要我单击了鼠标两次，那么就会弹出一个对话框
    //             2关闭了this(当前窗口),但是弹出的对话框却不能跟随着关闭
    qDebug() << "更新的个人信息：" << selfinfo.getname() << selfinfo.getnickname();
    UpdatePersonalInfo *updateInfo = new UpdatePersonalInfo(selfport,selfinfo,NULL,serverIP,serverPORT,myIP);

    updateInfo->show();
    //接下来就是操作网络发送消息了,不过网络消息已经在其中封装好了.
}












MainWindowDown::MainWindowDown(RegistRequest selfinfo,QTabWidget *parent,QString serverip,USERID_2T serverport,QString myip,USERID_2T myport,QString myIP)
    : selfinfo(selfinfo),QTabWidget(parent),serverip(serverip),serverport(serverport),myip(myip),myport(myport),myIP(myIP)
{
    setupUI();
    setupActions();
}
void MainWindowDown::setupUI()
{
#if 0
//0.添加背景????????????????????为何这里添加背景添加不了啊??????????????????????????????
    //目前找到的解决方法是 setStyleSheet函数.
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0xcb,0xf3,0xfb,0xff)));//背景色（青绿色）
    this->setPalette(palette);
#endif

    this->setDocumentMode(true);//没有了那条白色的带子了
    this->setMovable(true);//可以移动one group more 的位置
    this->setTabBarAutoHide(true);


#if 1
//1这里需要先搞出三个tab，分别代表one,group,more
    oneList = new QListWidget;
    //oneList->setFixedHeight(1000);
    oneList->setFixedWidth(237);
    oneList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);//醉了！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！由于这样设置了过后任然没有在需要的时候出现滚动条，导致的一个bug就是添加的朋友数量超过9个之后其他的是看不见的！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

    //groupList = new QListWidget;
    //groupList->setFixedWidth(237);
    //groupList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    moreList = new QListWidget;
    moreList->setFixedWidth(237);
    moreList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //new出布局,需要添加什么控件的时候，添加到这些layout中即可
    oneListLayout = new QVBoxLayout(oneList);
    oneListLayout->setAlignment(Qt::AlignTop);
    oneListLayout->setMargin(0);

    //groupListLayout = new QVBoxLayout(groupList);
    //groupListLayout->setAlignment(Qt::AlignTop);
    //groupListLayout->setMargin(0);

    moreListLayout = new QVBoxLayout(moreList);
    moreListLayout->setAlignment(Qt::AlignCenter);
    moreListLayout->setMargin(0);
#if 0
    //2016-05-02把这一部分注释了，因为在初始化的时候需要读取本地的friendlist数据库
    //然后根据数据库中的朋友id生成不同的onehuman对象，加载到onelistlayout中
//2在oneListLayout中添加属于本listwidget的控件
    oneListLayout->addWidget(new OneHuman);
    oneListLayout->addWidget(new OneHuman);
    oneListLayout->addWidget(new OneHuman);
    oneListLayout->addWidget(new OneHuman);
    oneListLayout->addWidget(new OneHuman);
    oneListLayout->addWidget(new OneHuman);
    //oneList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //qDebug() << oneList->count();//呵呵，说明了QListWidget的功能是不能用的
#endif
    //添加了一个好友，用于5月3号中期答辩的演示
    //oneListLayout->addWidget(new OneHuman);
//3在groupListLayout中添加属于本listwidget的控件

//4在moreListLayout中添加属于本listwidget的控件
    addOneFriend = new QToolButton;
    addOneFriend->setStyleSheet("background-color: rgb(4,159,241)");
    //addOneFriend->setStyleSheet("background-color: rgb(0,255,255)");5-20修改
    //addOneFriend->setToolTip("添加好友");
    addOneFriend->setToolTip("添加好友\nmy lover 慧慧");
    addOneFriend->setFixedSize(80,40);
    addOneFriend->setIconSize(QSize(40,40));
    addOneFriend->setIcon(QIcon(":/addOne.png"));
    moreListLayout->addWidget(addOneFriend);
    QSpacerItem *item1 = new QSpacerItem(40,60);
    moreListLayout->addItem(item1);

    //addGroupFriend = new QToolButton;
    //addGroupFriend->setStyleSheet("background-color: rgb(4,159,241)");
    //addGroupFriend->setStyleSheet("background-color: rgb(0,255,255)");//5-20修改
    //addGroupFriend->setToolTip("创建群组");
    //addGroupFriend->setFixedSize(80,40);
    //addGroupFriend->setIconSize(QSize(40,40));
    //addGroupFriend->setIcon(QIcon(":/addGroup.png"));
    //moreListLayout->addWidget(addGroupFriend);
    //QSpacerItem *item2 = new QSpacerItem(40,60);
    //moreListLayout->addItem(item2);

    about = new QToolButton;
    about->setStyleSheet("background-color: rgb(4,159,241)");
    //about->setStyleSheet("background-color: rgb(0,255,255)");//5-20修改
    about->setToolTip("关于软件");
    about->setFixedSize(80,40);
    about->setIconSize(QSize(40,40));
    about->setIcon(QIcon(":/about.png"));
    moreListLayout->addWidget(about);
    QSpacerItem *item3 = new QSpacerItem(40,60);
    moreListLayout->addItem(item3);

//5最后在每一个tab中分别添加QToolBox
    this->setIconSize(QSize(51,20));//呵呵哒，今天才知道setIconSize就可以把当前widget中的Icon设置为固定大小了
    this->addTab(oneList,QIcon(QPixmap(":/one.png")),"");//添加one
    //this->addTab(groupList,QIcon(QPixmap(":/group.png")),"");//添加group
    this->addTab(moreList,QIcon(QPixmap(":/more.png")),"");//添加more

    addOne = new AddOneFriend(NULL,getServerIP(),getServerPORT(),this->myip,myport);//这个地方写死了,需要升级的地方
#endif
}

void MainWindowDown::setupActions()
{
    connect(this->about,SIGNAL(clicked(bool)),this,SLOT(on_about_clicked()));
    connect(this->addOneFriend,SIGNAL(clicked(bool)),this,SLOT(on_addone_clieked()));
}

void MainWindowDown::on_about_clicked()
{
    About *about = new About;
    about->show();
    about->exec();
}

void MainWindowDown::on_addone_clieked()
{
    //添加一个好友
    addOne->show();
    qDebug() << "on_addone_clicked()";
}

void MainWindowDown::addOneHumanOnOneList(OneHuman *oneHuman)
{
    this->oneListLayout->addWidget(oneHuman);
}

USERID_2T MainWindowDown::getPort()
{
    return myport;
}

QString MainWindowDown::getServerIP()
{
    return serverip;
}

USERID_2T MainWindowDown::getServerPORT()
{
    return serverport;
}

QString MainWindowDown::getMyIP()
{
    return myIP;
}
