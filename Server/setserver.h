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
    SetServer(QDialog *parent = 0);
    ~SetServer();
#if 1
public :
    QLabel *serverIPLabel;
    QLabel *serverPORTLabel;

    QLineEdit *serverIP;
    QLineEdit *serverPORT;

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

#endif // SETSERVER_H
