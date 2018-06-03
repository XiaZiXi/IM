#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QUdpSocket>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QPoint>
#include <QTimer>
#include "sendsysmsg.h"
#include "setserver.h"
#include <QTableView>
#include "sqls.h"
#include "bases.h"

class Server : public QWidget
{
    Q_OBJECT
public:
    Server(QWidget *parent = 0);
    ~Server();
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
private :
    QPoint point;

private :
    void setupUI();
    void setupActions();
    void setupDatabase();
    void setupDefault();
public :
    QLabel *ipLabel;
    QLabel *portLabel;

    QPushButton *sysBtn;
    QPushButton *setBtn;
    QPushButton *hideBtn;
    QPushButton *exitBtn;

    QString IP;
    QString PORT;
    QHostAddress *hostAddress;

    QTableView *tableView;
    QSqlQueryModel *sqlQueryModel;

    QSqlDatabase db;//至关重要的成员，全靠它来连接数据库
    QSqlQuery *query;//至关重要的成员，全靠它来执行数据库操作
    QUdpSocket *udpSocket;//至关重要的成员，全靠它和客户端通信
public slots :
    void on_setBtn_clicked();
    void on_sysBtn_clicked();
    void processDatagrams();
    void myclose();
public :

   void handleLogin(USERID_2T userid,QString password,IP_4T userip);
   void handleRegistRequest(RegistRequest registrequest,IP_4T userip);
   void handleUpdatePersonalInfo(USERID_2T userid,RegistRequest registrequest,IP_4T userip);
   void handleLogout(USERID_2T userid,IP_4T userip);
   void handleSearchFriend(USERID_2T userid,RecommandFriendInformation recommandfriendinformation,IP_4T userip);
   void handleAddFried(USERID_2T userid,ANY_2T any2,IP_4T userip);
};

#endif // SERVER_H
