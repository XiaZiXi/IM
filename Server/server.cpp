#include "server.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QIcon>
#include <QDebug>
#include <QMessageBox>
#include <QtSql/QSqlQueryModel>
#include "setserver.h"
#include "sendsysmsg.h"
#include "bases.h"
#include "sqls.h"

Server::Server(QWidget *parent) : QWidget(parent)
{   
    setupUI();
    setupActions();
    setupDatabase();
    setupDefault();

    IP = "192.168.191.1";
    PORT = "9999";

    hostAddress = new QHostAddress(IP);
    udpSocket = new QUdpSocket(this);
#if 0
    addMemberOnView(10001,"康小超","192.168.214.128",true);
    addMemberOnView(10002,"康小超","192.168.214.128",true);
    addMemberOnView(10003,"康小超","192.168.214.128",true);
    addMemberOnView(10004,"康小超","192.168.214.128",true);
    addMemberOnView(10005,"康小超","192.168.214.128",true);
    addMemberOnView(10006,"康小超","192.168.214.128",true);
    addMemberOnView(10007,"康小超","192.168.214.128",true);
    addMemberOnView(10008,"康小超","192.168.214.128",true);
    sqlQueryModel->setQuery("select *from memberinfo_onview");
    tableView->show();
    tableView->setColumnWidth(0,55);//这个函数必须要在addMemberOnView之后调用方可生效,这也是没有放在setupUI中的原因
    tableView->setColumnWidth(1,90);
    tableView->setColumnWidth(2,136);
    tableView->setColumnWidth(3,50);
#endif
}

Server::~Server()
{
}

void Server::setupUI()
{
    this->setFixedSize(380,280);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/images/Server.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);

    //tableModel
    sqlQueryModel = new QSqlQueryModel;
    sqlQueryModel->setHeaderData(1,Qt::Horizontal,"ID");
    sqlQueryModel->setHeaderData(2,Qt::Horizontal,"昵称");
    sqlQueryModel->setHeaderData(3,Qt::Horizontal,"IP");
    sqlQueryModel->setHeaderData(4,Qt::Horizontal,"在线");
    //tableView
    tableView = new QTableView;
    //tableView->setStyleSheet("background-color: rgb(0,255,255)");
    tableView->setFont(QFont("汉仪南宫体简",12,200));
    tableView->setFixedSize(357,180);
    tableView->setModel(sqlQueryModel);

    //labelLayout
    ipLabel = new QLabel("服务器地址:192.168.191.1");
    ipLabel->setFont(QFont("汉仪南宫体简",13,240));
    ipLabel->setFixedSize(380,30);
    portLabel = new QLabel("开放端口号:9999");
    portLabel->setFont(QFont("汉仪南宫体简",13,240));
    portLabel->setFixedSize(380,30);
    QSpacerItem *spacer = new QSpacerItem(90,30);
    QHBoxLayout *labelLayout = new QHBoxLayout;
    labelLayout->addWidget(ipLabel);
    labelLayout->addItem(spacer);
    labelLayout->addWidget(portLabel);

    //btnsLayout
    sysBtn  = new QPushButton(QIcon(":/images/sys.png"),"");
    sysBtn->setToolTip("消息");
    sysBtn->setFixedSize(85,30);
    sysBtn->setEnabled(false);
    setBtn  = new QPushButton(QIcon(":/images/set.png"),"");
    setBtn->setToolTip("设置");
    setBtn->setFixedSize(85,30);
    hideBtn = new QPushButton(QIcon(":/images/hide.png"),"");
    hideBtn->setToolTip("最小");
    hideBtn->setFixedSize(85,30);
    exitBtn = new QPushButton(QIcon(":/images/exit.png"),"");
    exitBtn->setToolTip("退出");
    exitBtn->setFixedSize(85,30);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    //btnsLayout->setAlignment(Qt::AlignTop);
    btnsLayout->addWidget(setBtn);
    btnsLayout->addWidget(sysBtn);
    btnsLayout->addWidget(hideBtn);
    btnsLayout->addWidget(exitBtn);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setAlignment(Qt::AlignBottom);
    mainlayout->addWidget(tableView);
    mainlayout->addLayout(labelLayout);
    mainlayout->addLayout(btnsLayout);

}

void Server::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void Server::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}

void Server::setupActions()
{
    connect(hideBtn,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));//最小
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(myclose()));//退出

    connect(sysBtn,SIGNAL(clicked(bool)),this,SLOT(on_sysBtn_clicked()));//发送系统消息
    connect(setBtn,SIGNAL(clicked(bool)),this,SLOT(on_setBtn_clicked()));//设置服务端的ip和端口

}

void Server::setupDatabase()
{
    //核心代码
    //必须在服务器端的主界面上   1创建数据库的连接   2设置数据库的名字   3打开数据库  4创建执行数据库的对象  5把表创建好
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");
    if(db.open())
        qDebug() << "open server.db ok";
    else
        qDebug() << "open server.db failed";


    query = new QSqlQuery(db);


    if(query->exec("create table memberinfo(memberid integer primary key,nickname text not null,name text not null,age text not null,password text not null,telephone text,department text,email text,hometown text)"))//自动增长id字段
        qDebug() << "create table memberinfo ok";
    else
        qDebug() << "create table memberinfo failed";

    bool oks = query->exec("insert into memberinfo(memberid,nickname,name,age,password) values(10000,'root','root','0','root')");//初始化时候设置memberid最小值为10000
    if(oks){qDebug() << "insertinto memberinfo default successful!";}else qDebug() << "insertinto memberinfo default failed!";

    if(query->exec("create table friendrelationship(memberid integer not null,friendid integer not null)"))
        qDebug() << "create table friendrelationship ok";
    else
        qDebug() << "create table friendrelationship failed";


    if(query->exec("create table loginfo(memberid integer not null,ifonline integer not null)"))
        qDebug() << "create table loginfo ok";
    else
        qDebug() << "create table loginfo failed";


    if(query->exec("create table memberinfo_onview(ID int primary key,nickname text,IP text,online text)"))
        qDebug() << "create table memberinfo_onview ok";
    else
        qDebug() << "create table memberinfo_onview failed";


#if 0
    //测试sql.h中的静态函数是否能够成功插入到数据库
    addMemberInDB(query,10001,"kang","huzhihui","20","123356","15881432014","研发","123@qq.com","四川",12345);
    addFriendRelationshipInDB(query,10003,10001);
    addLogInfoInDB(query,10002,1);
#endif
}

void Server::setupDefault()
{
    //服务器启动的时候，一定是所有用户都是不在线的
    setAllUserOffLine(query);
    setAllUserOffLineOnView(sqlQueryModel,tableView,query);
}


void Server::on_setBtn_clicked()
{
    SetServer *setServer = new SetServer;
    setServer->show();
    setServer->exec();//弹出模式对话框,不然的话一闪而过就没有了
    QString ip = setServer->serverIP->text();
    QString port = setServer->serverPORT->text();
    IP = ip;
    PORT = port;
    this->ipLabel->setText("服务器IP:" + ip);
    ipLabel->setStyleSheet("color : red");
    this->portLabel->setText("开放端口号:" + port);
    portLabel->setStyleSheet("color : red");
    setBtn->setToolTip("服务器已就绪");
    sysBtn->setEnabled(true);

    udpSocket->bind(*hostAddress,PORT.toInt());
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processDatagrams()));

    QMessageBox::information(this,"恭喜","服务端已就绪");

    //4-28屏蔽了设置按钮
    setBtn->setEnabled(false);
}

void Server::on_sysBtn_clicked()
{
    SendSysMsg *sendSysMsg = new SendSysMsg;
    sendSysMsg->show();
    sendSysMsg->exec();//弹出模式对话框,不然的话一闪而过就没有了
    QString msg = sendSysMsg->sysMsg->toPlainText();
    qDebug() << msg;

    //接着就是通过socket发送广播了
    int maxid = getMemberInfoTableLargestID(query);
    for(int i = 10001;i <= maxid;i ++)
    {
        QString ip =  getFriendIP(query,i);
        int id = i;

        SYSMSG_STOC sysmsg = getSYSMSG_STOC(msg);
        QByteArray bytearray;
        QDataStream datastream(&bytearray,QIODevice::ReadWrite);
        datastream << sysmsg.head;
        datastream << sysmsg.body;
        udpSocket->writeDatagram(bytearray,QHostAddress(ip),id);//这里LocalHost写死了，升级的地方
    }
}

//开始读取数据，读到后自行处理。
void Server::processDatagrams()
{
    //读到数据先解析数据包头的十个字节
    qDebug() << "my personal data process!";
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

    USERID_2T userid = dataHead.getid();
    IP_4T userip = dataHead.getmyip();
    ANY_2T any2 = dataHead.getsegment();
    ANY_4T any4 = dataHead.getreserve();

    QString password;
    RegistRequest registrequest;
    QString empty;
    RegistRequest updateinfo;
    RecommandFriendInformation recommandfriendinformation;
    QString empty2;

    switch(dataHead.cmd)
    {
    case LOGIN_CMD_2V_CTOS :
        //登录
        qDebug() << "登录";
        //QString password;
        datastream >> password;
        qDebug() << "in function processDatagrams --- password=" << password;
        handleLogin(userid,password,userip);
        break;
    case REGIST_CMD_2V_CTOS :
        //注册
        qDebug() << "注册";
        //RegistRequest registrequest;
        datastream >> registrequest;
        handleRegistRequest(registrequest,userip);
        break;
    case UPDATE_PERSONAL_INFORMATION_CMD_2V_CTOS :
        //更新个人信息
        qDebug() << "更新个人信息";
        //RegistRequest updateinfo;
        datastream >> updateinfo;
        handleUpdatePersonalInfo(userid,updateinfo,userip);
        break;
    case LOGOUT_CMD_2V_CTOS :
        //登出
        qDebug() << "登出";
        //QString empty;
        datastream >> empty;
        handleLogout(userid,userip);
        break;
    case SEARCH_FRIEND_CMD_2V_CTOS :
        //推荐好友
        qDebug() << "推荐好友";
        //RecommandFriendInformation recommandfriendinformation;
        datastream >> recommandfriendinformation;
        handleSearchFriend(userid,recommandfriendinformation,userip);
        break;
    case ADD_FRIEND_CMD_2V_CTOS :
        //添加好友
        //QString empty2;
        datastream >> empty2;
        handleAddFried(userid,any2,userip);
        qDebug() << "添加好友";
        break;
    default :
        qDebug() << "无法知道的命令";
        break;
    }
}

void Server::myclose()
{
    QString msg = "服务器维护，给你带来的不便，敬请谅解！";
    int maxid = getMemberInfoTableLargestID(query);
    for(int i = 10001;i <= maxid;i ++)
    {
        QString ip =  getFriendIP(query,i);
        int id = i;

        SYSMSG_STOC sysmsg = getSYSMSG_STOC(msg);
        QByteArray bytearray;
        QDataStream datastream(&bytearray,QIODevice::ReadWrite);
        datastream << sysmsg.head;
        datastream << sysmsg.body;
        udpSocket->writeDatagram(bytearray,QHostAddress(ip),id);//这里LocalHost写死了，升级的地方
    }
    close();
}

void Server::handleLogin(USERID_2T userid, QString password,IP_4T userip)
{
    //遍历数据memberinfo及loginfo表，判定是否已经登录 和 登录信息的正确性
    //没有登录过,就可以更新系统UI
    //已经登录，就返回已经登录了的信息   登录信息不正确，返回登录信息错误的信息

    QString destIP = getIPFromInt(userip);
    qDebug() << "处理登录信息，处理过后发送到的目的ip为：" << destIP;

    LOGIN_SUCC_OR_FAIL_STOC login = getLOGIN_SUCC_OR_FAIL_STOC(userid,false);
    bool isok = clientIsOK(query,userid,password);
    QString nickname = getNickname(query,userid);
    if(isok)//密码和用户名匹配
    {
        bool islogin = clientIsLogin(query,userid);//是否已经登录了
        if(islogin)
        {
            login.head.setsegment(FAILURE_2V);//登录失败命令
            login.body = RegistRequest("该ID已经登录了哦！","","","","","","","");//第一个字段留作他用了呀
        }
        else
        {
            login.head.setsegment(SUCCESSFUL_2V);//告知客户端登录成功
            login.body = getSelfInfo(query,userid);
            //更新自己的UI
            QString usrip = getIPFromInt(userip);
            addMemberOnView(sqlQueryModel,tableView,query,userid,nickname,usrip,true);
            setOneUserOnLineOnView(sqlQueryModel,tableView,query,userid,usrip);
            //更新自己的loginfo表
            updateLoginfoLetItOnline(query,userid);
            //发送消息给他的好友，告知他们这个家伙已经登录啦
            //
        }
    }
    else
    {
        login.head.setsegment(FAILURE_2V);//登录失败命令
        login.body = RegistRequest("用户名和密码不匹配!","","","","","","","");//第一个字段留作他用了呀
    }

    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << login.head;
    datastream << login.body;

    udpSocket->writeDatagram(bytearray,QHostAddress(destIP),TEMP_PORT_LOGIN);//这里LocalHost写死了，升级的地方
}

void Server::handleRegistRequest(RegistRequest registrequest,IP_4T userip)
{
    QString destIP = getIPFromInt(userip);
    qDebug() << "处理注册信息，处理过后发送到的目的ip为：" << destIP;

    //准备给客户端发送的注册成败消息
    REGIST_SUCC_OR_FAIL_STOC regist_success = getREGIST_SUCC_OR_FAIL_STOC(0,true) ;//这里只用来两个参数，升级的地方
    REGIST_SUCC_OR_FAIL_STOC regist_failure = getREGIST_SUCC_OR_FAIL_STOC(0,false);//这里只用来两个参数，升级的地方

    regist_failure.body = "申请会员失败，可能是权限问题，请联系管理员!";

    QMessageBox::StandardButton btn = QMessageBox::information(NULL,"注册申请",QString("IP为")+destIP+QString("申请注册\n是否同意"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(btn == QMessageBox::Yes)
    {
        qDebug() << "Y";
        //将注册信息追加到数据库
        USERID_2T userid = getMemberInfoTableLargestID(query);
        qDebug() << "userid = " << userid;
        QString nickname = registrequest.getnickname();
        QString name = registrequest.getname();
        QString age = registrequest.getage();
        QString password = registrequest.getpassword();
        QString telephone = registrequest.gettelephone();
        QString department = registrequest.getdepartment();
        QString email = registrequest.getemail();
        QString hometown = registrequest.gethometown();


        USERID_2T retid = userid + 1;
        regist_success.head.setid(retid);
        qDebug() << "客户端注册成功，我返回给他的ID是" << retid;

        QByteArray bytearray;
        QDataStream datastream(&bytearray,QIODevice::ReadWrite);

        bool ok = addMemberInDB(query,retid - 1,nickname,name,age,password,telephone,department,email,hometown,userip);
        if(ok)
        {
            qDebug() << "addMemberInDB is ok!!!!!";
            datastream << regist_success.head;
            datastream << regist_success.body;
        }
        else
        {
            qDebug() << "addMemberInDB is failure!";
            datastream << regist_failure.head;
            datastream << regist_failure.body;
        }
        udpSocket->writeDatagram(bytearray,QHostAddress(destIP),TEMP_PORT_REGIST);//这里LocalHost写死了，升级的地方
    }
    else
    {
        qDebug() << "N";
        QByteArray bytearray;
        QDataStream datastream(&bytearray,QIODevice::ReadWrite);

        qDebug() << "addMemberInDB is failure!";
        datastream << regist_failure.head;
        datastream << regist_failure.body;
        udpSocket->writeDatagram(bytearray,QHostAddress(destIP),TEMP_PORT_REGIST);//这里LocalHost写死了，升级的地方
    }
}

void Server::handleUpdatePersonalInfo(USERID_2T userid, RegistRequest registrequest,IP_4T userip)
{
    QString destIP = getIPFromInt(userip);
    qDebug() << "处理更新个人信息，处理过后发送到的目的ip为：" << destIP;

    //将更新信息更新到数据库
    qDebug() << "userid = " << userid;
    //准备给客户端发送的更新成败消息
    //UPDATE_SUCC_OR_FAIL_STOC  getUPDATE_SUCC_OR_FAIL_STOC(USERID_2T id,bool isSuccessful,QString msg = "default error to updatePersonalInformation")
    UPDATE_SUCC_OR_FAIL_STOC update_success = getUPDATE_SUCC_OR_FAIL_STOC(userid,true);
    UPDATE_SUCC_OR_FAIL_STOC update_failure = getUPDATE_SUCC_OR_FAIL_STOC(userid,false);
    update_failure.body = "服务器故障，暂时无法更新个人信息";


    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);

    bool ok = updateMemberInfoInDB(query,userid,registrequest);
    if(ok)
    {
        qDebug() << "updateDB is ok!!!!!";
        datastream << update_success.head;
        datastream << update_success.body;
        //更新自己的ui(其实目前就只有昵称),iP没有更新，因为都是固定的，需要升级的地方
        updateOneUserInfoOnView(sqlQueryModel,tableView,query,userid,getIPFromInt(userip));
    }
    else
    {
        qDebug() << "updateDB is failure!";
        datastream << update_failure.head;
        datastream << update_failure.body;
    }
    qDebug() << update_success.head.getcmd();
    udpSocket->writeDatagram(bytearray,QHostAddress(destIP),userid);//这里LocalHost写死了，升级的地方
}

void Server::handleLogout(USERID_2T userid,IP_4T userip)
{
    qDebug() << "handleLogout()";
    //QString destIP = getIPFromInt(userip);

    //qDebug() << userid << destIP;
    //更新数据表loginfo
    setOneUserOffLine(query,userid);
    //更新服务端的UI，让该ID显示离线
    setOneUserOffLineOnView(sqlQueryModel,tableView,query,userid);

    //给他的所有好友发送他已经登出的消息,然后让他们更新自己的UI界面
    int friendid = getFriendID(query,userid);
    QString friendip = getFriendIP(query,friendid);

    LOGOUT_TELL_FRIEND_STOC logout = getLOGOUT_TELL_FRIEND_STOC(userid,friendid);
    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);
    datastream << logout.head;
    datastream << logout.body;
    udpSocket->writeDatagram(bytearray,QHostAddress(friendip),friendid);//这里LocalHost写死了，升级的地方
}

void Server::handleSearchFriend(USERID_2T userid, RecommandFriendInformation recommandfriendinformation,IP_4T userip)
{
    QString destIP = getIPFromInt(userip);

}

void Server::handleAddFried(USERID_2T userid, ANY_2T any2,IP_4T userip)
{
    QString destIP = getIPFromInt(userip);
    qDebug() << "处理添加好友信息，处理过后发送到的目的ip为：" << destIP;

    //准备给客户端发送的添加好友成败消息
    //ADDFRIEND_SUCC_OR_FAIL_STOC  getADDFRIEND_SUCC_OR_FAIL_STOC(USERID_2T id,bool isSuccessful,QString msg = "default error to addfriend")
    ADDFRIEND_SUCC_OR_FAIL_STOC add_success = getADDFRIEND_SUCC_OR_FAIL_STOC(userid,true);
    ADDFRIEND_SUCC_OR_FAIL_STOC add_failure = getADDFRIEND_SUCC_OR_FAIL_STOC(userid,false);
    add_success.head.setsegment(any2);
    add_failure.head.setsegment(any2);
    add_success.body[0] = "";
    QString friendnickname = getNickname(query,any2);
    add_success.body[1] = friendnickname;
    add_failure.body[0] = "好友不存在或服务器故障,请重试！";

    QByteArray bytearray;
    QDataStream datastream(&bytearray,QIODevice::ReadWrite);


    //检验他的好友的id是否正常
    bool ok = clientIsExist(query,any2);
    if(ok)
    {
        if(friendIsExist(query,any2))
        {
            qDebug() << "addfriend is failed!!!!!";
            add_failure.body[0] = "不能添加已经存在的好友!";
            datastream << add_failure.head;
            datastream << add_failure.body[0];
        }
        else
        {
            if(any2 == userid)
            {
                qDebug() << "addfriend is failed!!!!!";
                add_failure.body[0] = "不能添加自己为好友!";
                datastream << add_failure.head;
                datastream << add_failure.body[0];
            }
            else
            {
                if(ifOnline(query,any2))
                {
                    //给他发送添加好友成功消息,并且把他的好友的IP发送过去
                    qDebug() << "addfriend is ok!!!!!";
                    //从表中查到他的朋友的ip
                    QString friendip = getFriendIP(query,any2);
                    add_success.head.sethisip(getIPFromString(friendip));
                    datastream << add_success.head;
                    datastream << add_success.body[0] << add_success.body[1];

                    //给他的好友发送添加好友消息
                    //giveHisFriendSendAddMsg();//这个还是不在这里实现了，直接用客户端的socket实现
                    //更新自己的数据表
                    updateMyFriendRelationship(query,userid,any2);
                }
                else
                {
                    qDebug() << "addfirend is failed!!!!";
                    add_failure.body[0] = "该好友目前不在线哦！";
                    datastream << add_failure.head;
                    datastream << add_failure.body[0];
                }
            }
        }
    }
    else
    {
        //给他发送添加好友失败消息
        qDebug() << "addfriend is failure!";
        datastream << add_failure.head;
        datastream << add_failure.body[0];
    }
    qDebug() << "userid:" << userid << "friendid:" << any2 << "cmd:" << add_failure.head.getcmd();
    udpSocket->writeDatagram(bytearray,QHostAddress(destIP),userid);//向该userid报告添加好友成败
    
}


