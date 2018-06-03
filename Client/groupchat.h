#ifndef GROUP_H
#define GROUP_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>
#include <QTextEdit>
#include <QTextBrowser>
#include <QListWidget>

class GroupChat : public QWidget
{
    Q_OBJECT

public:
    GroupChat(QWidget *parent = 0,const char *nickName = "未知");
    ~GroupChat();
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上
private slots:
    void showUpText();
private :
    void setupUI();
    void setupActions();
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
public :
    QPushButton *hideBtn;
    QPushButton *exitBtn;
    QPushButton *sendBtn;

    QTextBrowser *textBrowser;//上部 显示
    QTextEdit *textEdit;//下部 编辑
    QListWidget *listWidget;//右部 显示

    QVBoxLayout *listWidgetLayout;//用于在其上添加groupMan控件

    QLabel *nickNameLabel;
};

#endif // GROUP_H
