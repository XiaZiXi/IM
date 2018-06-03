#ifndef SCRAWL_H
#define SCRAWL_H

#include <QWidget>
#include <QPoint>
#include <QColor>
#include <QImage>
#include <QLabel>
#include <QPoint>
#include <QList>
#include <QMainWindow>
#include <QToolButton>

class ScrawlArea : public QWidget
{
    Q_OBJECT

public:
    ScrawlArea(QWidget *parent = 0);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;
};


class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QMainWindow *parent = 0);
private slots:
    void open();
    void save();
    void penColor();
    void penWidth();

public :
    ScrawlArea *scrawlArea;
    bool maybeSave();
public :
    QString sendScrawName;

public:
    bool saveFile(const QByteArray &fileFormat);
};

class Scrawl : public QWidget
{
    Q_OBJECT
public :
    Scrawl(QString nickName,QString friendip,int friendid,QString myip,int myid,QWidget *parent = 0);
    ~Scrawl();

    void setupUI();
    void setupActions();
protected :
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
public :
    Window *window;

    QString sendScrawlName;//要发送的涂鸦的名字

    QPoint point;

    QLabel *nickNameLabel;

    QToolButton *scrawlColor;//颜色
    QToolButton *scrawlWidth;//宽度
    QToolButton *scrawlClear;//清屏
    QToolButton *scrawlOpen; //打开
    QToolButton *scrawlExit; //退出
    QToolButton *scrawlSend; //发送

    QString friendip;
    int friendid;
    QString myip;
    int myid;
    QString nickName;

public slots:
    void sendSlot();
public :
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

};
#endif // SCRAWL_H
