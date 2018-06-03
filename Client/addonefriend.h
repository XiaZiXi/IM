#ifndef ADDONEFRIEND_H
#define ADDONEFRIEND_H

#include <QAbstractSocket>
#include <QHostAddress>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QUdpSocket>

class AddOneFriend : public QWidget
{
    Q_OBJECT

public:
    explicit AddOneFriend(QWidget *parent,QString serverip,quint16 serverport,QString myip,quint16 myport);
private :
    QPushButton  *sendButton;
	QPushButton *cancelButton;
    QLineEdit *idEdit;
private :
    QPoint point;//由于无边框不可移动，通过mousePressEvent和mouseMoveEvent两个虚函数实现移动
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//在父类中是一个虚函数
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;//同上
public :
    QString serverip;
    quint16 serverport;
    QString myip;
    quint16 myport;
    QUdpSocket *udpsocket;
private:


private slots:
    void on_cancelButton_clicked();
    void on_sendButton_clicked();
    void readMsg();
};

#endif // ADDONEFRIEND_H
