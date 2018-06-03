#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>
#include <QUdpSocket>

#include "setserver.h"
#include "regist.h"
#include "logincheck.h"

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = 0);
    ~Login();
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上
private slots:
    //登录按钮按下，获取ID,PD,远端服务器的IP和端口号,然后发送登录信息给服务器
    //服务器收到反馈,查找自己的数据库,返回成功或失败信息发送给客户端.
    void loginBtnClicked() ;
    void setingBtnClicked();
    void registBtnClicked();
private :
    void setupUI();
    void setupDefault();
    void setupActions();
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
public :
    QToolButton *hideBtn;
    QToolButton *exitBtn;

    QSpacerItem *hSpacer1;
    QSpacerItem *hSpacer2;
    QSpacerItem *hSpacer3;
    QSpacerItem *hSpacer4;
    QSpacerItem *hSpacer5;
    QSpacerItem *hSpacer6;
    QSpacerItem *hSpacer7;
    QSpacerItem *hSpacer8;
    QSpacerItem *hSpacer9;
    QSpacerItem *hSpacer10;

    QSpacerItem *vSpacer1;
    QSpacerItem *vSpacer2;
    QSpacerItem *vSpacer3;
    QSpacerItem *vSpacer4;
    QSpacerItem *vSpacer5;

    QHBoxLayout *hlayout1;
    QHBoxLayout *hlayout2;
    QHBoxLayout *hlayout3;
    QHBoxLayout *hlayout4;
    QHBoxLayout *hlayout5;
    QHBoxLayout *hlayout6;

    QVBoxLayout *mainlayout;
    QVBoxLayout *vlayout1;
    QVBoxLayout *vlayout2;

    QLabel *IDLabel;
    QLabel *PDLabel;
    QLabel *registLabel;
    QLabel *setingLabel;

    QLineEdit *ID;
    QLineEdit *PD;

    QPushButton *login;
    QPushButton *regist;
    QPushButton *seting;
public :
    int serverPORT;//保留服务器的端口
    QString serverIP;//保留服务器的IP地址
    QString myIP;//保留自己的IP地址

    QUdpSocket *udpsocket;//这个socket只是用于向服务器发送登录请求信息，以及接受来自服务器的反馈

    LoginCheck *animation;

    void setServerIP(QString serverIP);
    QString  getServerIP();
    void setMyIP(QString myIP);
    QString getMyIP();
    void setServerPORT(int serverPORT);
    int getServerPORT();
private :
    SetServer *setServer;
    Regist *registered;

public :
    void sendLoginToServer();
public slots :
    void receiveMsg();
public :
    bool flag;
};

#endif // LOGIN_H
