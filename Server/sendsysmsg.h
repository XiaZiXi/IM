#ifndef SENDSYSMSG_H
#define SENDSYSMSG_H
#include <QTextEdit>
#include <QDialog>
#include <QPushButton>
#include <QPoint>
#include <QSpacerItem>
class SendSysMsg : public QDialog
{
    Q_OBJECT
public:
    SendSysMsg(QDialog *parent = 0);
    ~SendSysMsg();
#if 1
public :
    QTextEdit *sysMsg;

    QPushButton *okBtn;

    QPoint point;

    QSpacerItem *spacer1;
    QSpacerItem *spacer2;
    QSpacerItem *spacer3;
    QSpacerItem *spacer4;
    QSpacerItem *spacer5;
    QSpacerItem *spacer6;
#endif
protected :
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
private :
    void setupUI();
    void setupActions();
public slots :
    void on_okBtn_clicked();
};

#endif // SENDSYSMSG_H
