#ifndef FRIENDINFO_H
#define FRIENDINFO_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>

//这个类用于     查看好友的详细信息
class FriendInfo : public QWidget
{
    Q_OBJECT

public:
    FriendInfo(QWidget *parent = 0,
               QString name = "空",
               QString age = "空",
               QString nickName = "空",
               QString identify = "空",
               QString telephone = "空",
               QString email = "空",
               QString department = "空",
               QString hometown = "空");
    ~FriendInfo();
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上
private slots:
private :
    void setupUI();
    void setupActions();
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
public :
    QVBoxLayout *mainlayout;

    QLabel *nickNameLabel;
    QLabel *nickNamePicture;
    QLabel *nameLabel;
    QLabel *ageLabel;
    QLabel *identifyLabel;
    QLabel *nicknameLabel;
    QLabel *telephoneLabel;
    QLabel *emailLabel;
    QLabel *departmentLabel;
    QLabel *hometownLabel;

    QLabel *nickName;
    QLabel *name;
    QLabel *age;
    QLabel *identify;
    QLabel *nickname;
    QLabel *telephone;
    QLabel *email;
    QLabel *department;
    QLabel *hometown;
};

#endif // FRIENDINFO_H
