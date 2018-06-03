#ifndef MYIMAGE_H
#define MYIMAGE_H
#include <QTextEdit>
#include <QDialog>
#include <QPushButton>
#include <QPoint>
#include <QSpacerItem>
#include <QTextBrowser>
#include <QLabel>

class MyImage : public QDialog
{
    Q_OBJECT
public:
    MyImage(QString name,QDialog *parent = 0);
public :
    QLabel *image;

    QPushButton *okBtn;

    QPoint point;
    QTimer *timer;

    QSpacerItem *spacer1;
    QSpacerItem *spacer2;
    QSpacerItem *spacer3;
    QSpacerItem *spacer4;
    QSpacerItem *spacer5;
    QSpacerItem *spacer6;

    QString name;
protected :
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
private :
    void setupUI();
    void setupActions();
public slots :
    void on_okBtn_clicked();
    void handle_timer();
};

#endif // MYIMAGE_H
