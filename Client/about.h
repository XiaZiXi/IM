#ifndef ABOUT_H
#define ABOUT_H
#include <QTextEdit>
#include <QDialog>
#include <QPushButton>
#include <QPoint>
#include <QSpacerItem>
#include <QTextBrowser>

class About : public QDialog
{
    Q_OBJECT
public:
    About(QDialog *parent = 0);
    ~About();
#if 1
public :
    QTextBrowser *sysMsg;

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

#endif // ABOUT_H
