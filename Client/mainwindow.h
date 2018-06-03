#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QToolButton>
#include "onehuman.h"
#include <QFrame>
#include <QAbstractScrollArea>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "bases.h"
#include <QUdpSocket>
#include "updatepersonalinfo.h"
#include "bases.h"
#include "addonefriend.h"

class MainWindowUp;
class MainWindowDown;
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QString serverIP,USERID_2T serverPORT,USERID_2T port,QString myIP,RegistRequest selfinfo,QWidget *parent = 0);
    ~MainWindow();
    void setupUI();
    void setupNetwork();
    void setupActions();
    void setupDatabases();
    void updataLocalDB();
    void loadDataToUI();
signals:
protected :
    void mouseMoveEvent(QMouseEvent *)  Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
public slots:
    void readMsg();
    void myExit();
public :
    QPoint point;
    QToolButton *hideBtn;
    QToolButton *exitBtn;
    MainWindowUp *mainWindowUp;//主窗口的上部
    MainWindowDown *mainWindowDown;//主窗口的下部
public :
    QSqlDatabase db;//至关重要的成员，全靠它来连接数据库
    QSqlQuery *query;//至关重要的成员，全靠它来执行数据库操作
    QUdpSocket *udpsocket;//至关重要的成员，全靠他来接收udp消息(发送dup消息由onehuman负责)
private :
    USERID_2T port;
    RegistRequest selfinfo;
    QString serverIP;
    USERID_2T serverPORT;
    QString myIP;
public :
    USERID_2T getPort();
    QString getServerIP();
    USERID_2T getServerPORT();
    QString getMyIP();

    void handleUpdatePersonalInfo(FAILURE_SUCCEED_2T result,QString errorReason);
    void handleAddFriend(USERID_2T userid,ANY_2T friendid,QString errorReason,QString friendnickname,IP_4T friendip);
    void CTOC_sendMsgBecauseOfAdd(QString nickname,USERID_2T userid,USERID_2T friendid,QString friendip,QString myip);
    void handleAddFriendCTOC(USERID_2T friendid,QString nickname,IP_4T friendip);
};





class MainWindowUp : public QToolButton
{
    Q_OBJECT
public :
    MainWindowUp(USERID_2T selfport,RegistRequest selfinfo,QToolButton *parent,QString serverIP,int serverPORT,QString myIP);
    void setupUI();
    void setupActions();
    void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;//鼠标双击后的触发事件
signals:
public slots:
public :
    QLabel *nickNameIcon;//当前会员的头像
    QLabel *nickNameLabel;//当前会员的昵称
    RegistRequest selfinfo;
    QString serverIP;
    int serverPORT;
    USERID_2T selfport;
    QString myIP;
};





class MainWindowDown : public QTabWidget
{
    Q_OBJECT
public :
    MainWindowDown(RegistRequest selfinfo,QTabWidget *parent,QString serverip,USERID_2T serverport,QString myip,USERID_2T myport,QString myIP);
    void setupUI();
    void setupActions();
signals :
public slots:
    void on_about_clicked();
    void on_addone_clieked();
public :
    RegistRequest selfinfo;
    QListWidget *oneList;//单个好友
    //QListWidget *groupList;//群
    QListWidget *moreList;//更多设置

    QVBoxLayout *oneListLayout;//单个好友 布局
    //QVBoxLayout *groupListLayout;//群 布局
    QVBoxLayout *moreListLayout;//更多设置 布局

    void addOneHumanOnOneList(OneHuman *oneHuman);

    QList<OneHuman> humanList;//单个好友的列表
                                //群好友的列表
                                //更多的设置界面
    QToolButton *addOneFriend;
    //QToolButton *addGroupFriend;
    AddOneFriend *addOne;
    QToolButton *about;
private :
    USERID_2T myport;
    QString myip;
    QString serverip;
    USERID_2T serverport;
    QString myIP;
public :
    USERID_2T getPort();
    QString getServerIP();
    USERID_2T getServerPORT();
    QString getMyIP();
};




#endif // MAINWINDOW_H
