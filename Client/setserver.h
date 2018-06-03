#ifndef SETSERVER_H
#define SETSERVER_H
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QPoint>
#include <QSpacerItem>
class SetServer : public QDialog
{
    Q_OBJECT
public:
    SetServer();
    ~SetServer();
#if 1
public :
    QLabel *serverIPLabel;
    QLabel *serverPORTLabel;
    QLabel *myIPLabel;

    QLineEdit *serverIP;
    QLineEdit *serverPORT;
    QLineEdit *myIP;//5-9

    QPushButton *okBtn;

    QPoint point;

    QSpacerItem *spacer1;
    QSpacerItem *spacer2;
    QSpacerItem *spacer3;
    QSpacerItem *spacer4;
    QSpacerItem *spacer5;
    QSpacerItem *spacer6;
    QSpacerItem *spacer11;
    QSpacerItem *spacer33;
#endif
protected :
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
private :
    void setupUI();
    void setupActions();
public slots :
    void myhide();
private :
    QString serverip;
    QString serverport;
    QString myip;
public :
    QString getServerIP();
    QString getServerPORT();
    QString getMyIP();
};

#endif // SETSERVER_H
