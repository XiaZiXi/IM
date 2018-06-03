#ifndef ONEHUMAN_H
#define ONEHUMAN_H

#include <QToolButton>
#include <QLabel>
#include "bases.h"
#include "chat.h"

class OneHuman : public QToolButton
{
    Q_OBJECT
public:
    OneHuman(int myid,QString mainwindowip,int mainwindowport,QString mynickname,IP_4T friendip,QToolButton *parent);
    ~OneHuman();

    void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;//鼠标双击后的触发事件
    void setupUI();
    void setupActions();

    static OneHuman* createDefaultOneHuman();
public slots :
signals :
public :
    QLabel *nickNameIcon;//当前会员的头像
    QLabel *nickNameLabel;//当前会员的昵称

    int myid;
    QString mainwindowip;
    int mainwindowport;
    QString mynickname;
    IP_4T friendip;

    Chat * chat;
};

#endif // ONEHUMAN_H
