#ifndef LOGINCHECK_H
#define LOGINCHECK_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QPoint>
#include <QTimer>

class LoginCheck : public QWidget
{
    Q_OBJECT
public:
    LoginCheck(QWidget *parent = 0);
    ~LoginCheck();
protected :
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
signals:
private :
    QLabel *shows;

    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
    QLabel * label4;

    QSpacerItem *spacer1;
    QSpacerItem *spacer2;

    QPoint point;

    QTimer *timer;
private :
    void setupUI();
    void setupActions();
    void startAnimation();
public slots:
    void update();
};

#endif // LOGINCHECK_H
