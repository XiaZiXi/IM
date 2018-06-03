#ifndef REGIST_H
#define REGIST_H

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
#include <QDialog>
#include <QUdpSocket>
#include "bases.h"


//这个类用于     1注册  和  2个人信息的更新
class Regist : public QWidget
{
    Q_OBJECT

public:
    Regist(QWidget *parent,QString serverIP,USERID_2T serverPORT,QString myIP);
    ~Regist();
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上
private slots:
    void updateBtnClicked();
    void cleanAll();
    void receiveMsg();
private :
    void setupUI();
    void setupDefault();
    void setupActions();
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
public :
    QToolButton *hideBtn;
    QToolButton *exitBtn;

    QPushButton *updateBtn;
    QPushButton *cancelBtn;

    QSpacerItem *hSpacer1;
    QSpacerItem *vSpacer1;
    QSpacerItem *vSpacer2;


    QHBoxLayout *hlayout1;
    QHBoxLayout *hlayout2;
    QHBoxLayout *hlayout3;

    QVBoxLayout *mainlayout;

    QLabel *nickNameLabel;
    QLabel *nickNamePicture;
    QLabel *nameLabel;
    QLabel *ageLabel;
    QLabel *passwordLabel;
    QLabel *confirmPasswordLabel;
    QLabel *telephoneLabel;
    QLabel *emailLabel;
    QLabel *departmentLabel;
    QLabel *hometownLabel;

    QLineEdit *nickName;
    QLineEdit *name;
    QLineEdit *age;
    QLineEdit *password;
    QLineEdit *confirmPassword;
    QLineEdit *telephone;
    QLineEdit *email;
    QLineEdit *department;
    QLineEdit *hometown;
private :
    QString serverIP;
    USERID_2T serverPORT;
    QString myIP;
    QUdpSocket *udpSocket;//至关重要的成员，用于和服务器端的注册通信
public :
    void sendRegistToServer();
    QString getServerIP();
    USERID_2T getServerPORT();
    QString getMyIP();
};

#endif // REGIST_H
