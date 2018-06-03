#ifndef CHAT_H
#define CHAT_H

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
#include <QTextEdit>
#include <QTimer>
#include <QTextBrowser>
#include <QUdpSocket>
#include "scrawlserver.h"

class Chat : public QWidget
{
    Q_OBJECT

public:
    Chat(QString friendnickname,QString friendip,int friendid,QString myip,int myid,QWidget *parent = 0);
    ~Chat();
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上
private slots:
    void showUpText();
    void clearUpText();
    void savaRecords();
private :
    void setupUI();
    void setupActions();
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
public :
    QPushButton *hideBtn;
    QPushButton *exitBtn;
    QPushButton *sendBtn;


    QToolButton *shake;//抖动
    QToolButton *scrawl;//涂鸦
    QToolButton *clear;//清空
    QToolButton *file;//文件
    QToolButton *save;//保存

    QTextBrowser *textBrowser;//上部 显示
    QTextEdit *textEdit;//下部 编辑

    QLabel *nickNameLabel;

    QUdpSocket *socket;//至关重要的成员,用于客户端之间发送消息
    ScrawlServer *scrawlserver;//至关重要的成员，用于接收涂鸦（这里也有个想法，使用端口偏移）


private slots://下面的两个成员函数用于窗口抖动
    void slot_btnShakeClicked();
    void slot_timerOut();

    void slot_scrawl();//用于涂鸦
    void slot_file();//用于文件发送
    void processMsg();

private://下面的三个成员变量用于窗口抖动
    QTimer* m_timer;
    int m_nPosition;
    QPoint m_curPos;
public :
    QString friendnickname;
    QString friendip;
    int friendid;
    QString myip;
    int myid;
public :
    void handleShake();
    void handleScrawl();
    void handleFile();
    void handleMessage(QString msg);
};

#endif // CHAT_H
