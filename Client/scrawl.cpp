#include "scrawl.h"
#include <QtWidgets>
#include <QSpacerItem>
#include "scrawlclient.h"

ScrawlArea::ScrawlArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 8;
    myPenColor = Qt::red;
}

bool ScrawlArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool ScrawlArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void ScrawlArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScrawlArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScrawlArea::clearImage()
{
    image.fill(qRgb(250, 250, 250));
    modified = true;
    update();
}

void ScrawlArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ScrawlArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void ScrawlArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void ScrawlArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScrawlArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScrawlArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ScrawlArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}





Window::Window(QMainWindow *parent) : QMainWindow(parent)
{
    scrawlArea = new ScrawlArea;
    setCentralWidget(scrawlArea);
    resize(350, 530);


    //this->setFixedSize(350,500);//设置固定大小(黄金分割！370:600但是不好看)
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/Scrawl.jpg"));//设置窗口图标
}
void Window::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   "打开文件", QString("C:\\"));
        if (!fileName.isEmpty())
            scrawlArea->openImage(fileName);
    }
}

void Window::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void Window::penColor()
{
    QColor newColor = QColorDialog::getColor(scrawlArea->penColor());
    if (newColor.isValid())
        scrawlArea->setPenColor(newColor);
}

void Window::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, "涂鸦板",
                                        tr("选择画笔宽度:"),
                                        scrawlArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scrawlArea->setPenWidth(newWidth);
}

bool Window::maybeSave()
{
#if 0
    if (scrawlArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this,"涂鸦板",
                          "涂鸦已改变.\n你想要保存你的涂鸦吗?",
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
#endif
    return true;
}

bool Window::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = "C:/";
    QString currentTime = QDateTime::currentDateTime().toString("yyMMddhhmmss");
    QString fileName = initialPath + currentTime + ".png";
    qDebug() << fileName;
    sendScrawName = fileName;
    return scrawlArea->saveImage(fileName,"png");
}






Scrawl::Scrawl(QString nickName,QString friendip,int friendid,QString myip,int myid,QWidget *parent)
    :nickName(nickName),friendip(friendip),friendid(friendid),myip(myip),myid(myid),QWidget(parent)
{
    setupUI();
    nickNameLabel->setText(nickName);
    setupActions();
}
Scrawl::~Scrawl()
{}

void Scrawl::setupUI()
{
    this->resize(360,530);
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗口没有边框
    this->setWindowIcon(QIcon(":/Scrawl.jpg"));//设置窗口图标

//0.添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QColor(0x04,0x9f,0xf1,0xff)));//背景色（青绿色）
    this->setPalette(palette);
//0.交流对象
    nickNameLabel = new QLabel(this);//这里的this起了很大的作用哈，不然的话，就没有了交流对象的昵称在最上面的布局了
    nickNameLabel->setAlignment(Qt::AlignCenter);
    nickNameLabel->setAutoFillBackground(true);
    nickNameLabel->setPalette(QPalette(QColor(Qt::blue)));
    nickNameLabel->setFont(QFont("汉仪南宫体简",15,255));
    nickNameLabel->setFixedSize(400,30);
//1.spacer
    QSpacerItem *spacer = new QSpacerItem(360,30);

//2.window
    window = new Window;

//3.hDown
    //3.1 hOpenExit
    scrawlOpen = new QToolButton;
    scrawlOpen->setToolTip("打开");
    scrawlOpen->setIcon(QIcon(":/scrawlOpen.png"));
    scrawlOpen->setFixedSize(50,40);
    scrawlExit = new QToolButton;
    scrawlExit->setToolTip("退出");
    scrawlExit->setIcon(QIcon(":/scrawlExit.png"));
    scrawlExit->setFixedSize(50,40);
    QSpacerItem * item1 = new QSpacerItem(20,40);
    QHBoxLayout * hOpenExit = new QHBoxLayout;

    hOpenExit->addWidget(scrawlOpen);
    hOpenExit->addWidget(scrawlExit);
    hOpenExit->addItem(item1);

    //3.2 hColorWidthClear
    scrawlColor = new QToolButton;
    scrawlColor->setToolTip("颜色");
    scrawlColor->setIcon(QIcon(":/scrawlColor.png"));
    scrawlColor->setFixedSize(50,40);
    scrawlWidth = new QToolButton;
    scrawlWidth->setToolTip("宽度");
    scrawlWidth->setIcon(QIcon(":/scrawlWidth.png"));
    scrawlWidth->setFixedSize(50,40);
    scrawlClear =  new QToolButton;
    scrawlClear->setToolTip("清屏");
    scrawlClear->setIcon(QIcon(":/scrawlClear.png"));
    scrawlClear->setFixedSize(50,40);
    QSpacerItem * item2 = new QSpacerItem(20,40);
    QHBoxLayout * hColorWidthClear = new QHBoxLayout;

    hColorWidthClear->addWidget(scrawlColor);
    hColorWidthClear->addWidget(scrawlWidth);
    hColorWidthClear->addWidget(scrawlClear);
    hColorWidthClear->addItem(item2);

    //3.3 hButtons   hOpenExit hColorWidthClear
    QHBoxLayout *hButtons = new QHBoxLayout;

    hButtons->addLayout(hColorWidthClear);
    hButtons->addLayout(hOpenExit);

    //hDown   hButtons scrawlSend
    scrawlSend = new QToolButton;
    scrawlSend->setToolTip("发送");
    scrawlSend->setIcon(QIcon(":/scrawlSend.png"));
    scrawlSend->setFixedSize(50,40);
    QHBoxLayout *hDown = new QHBoxLayout;

    hDown->addLayout(hButtons);
    hDown->addWidget(scrawlSend);


//mainlayout     spacer window hDown
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setAlignment(Qt::AlignCenter);
    mainlayout->addItem(spacer);
    mainlayout->addWidget(window);
    mainlayout->addLayout(hDown);
}

void Scrawl::setupActions()
{
    connect(scrawlColor,SIGNAL(clicked(bool)),window,SLOT(penColor()));//颜色
    connect(scrawlWidth,SIGNAL(clicked(bool)),window,SLOT(penWidth()));//宽度
    connect(scrawlClear,SIGNAL(clicked(bool)),window->scrawlArea,SLOT(clearImage()));//清屏
    connect(scrawlExit,SIGNAL(clicked(bool)),this,SLOT(close()));//退出
    connect(scrawlOpen,SIGNAL(clicked(bool)),window,SLOT(open()));//打开
    connect(scrawlSend,SIGNAL(clicked(bool)),this,SLOT(sendSlot()));//发送
}

void Scrawl::closeEvent(QCloseEvent *event)
{
#if 0
    if (window->maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scrawl::sendSlot()
{
    //最开始，我们先保存scrawl，名称为当前的时间
    //然后获取需要发送的scrawl的名称，可从Scrawl类的成员变量中找到
    //调用传送文件的函数来发送这个scrawl即可
    window->saveFile("png");
    this->sendScrawlName = window->sendScrawName;
    qDebug() << "sendSlot()" << friendip << friendid << sendScrawlName;
    ScrawlClient *client = new ScrawlClient(NULL,QHostAddress(friendip),friendid+100,sendScrawlName);
    this->window->scrawlArea->clearImage();//清屏
    this->close();
    QMessageBox::information(this,"提示",QString("向")+nickName+QString("发送涂鸦成功！"));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scrawl::mousePressEvent(QMouseEvent *e)
{
    point = e->pos();
}
void Scrawl::mouseMoveEvent(QMouseEvent *e)
{
    move(e->globalPos() - point);
}
