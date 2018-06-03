#include <QApplication>
#include "login.h"
#include "logincheck.h"
//#include "setserver.h"
//#include "regist.h"
#include "friendinfo.h"
#include "chat.h"
#include "scrawl.h"
#include "mainwindow.h"
#include "onehuman.h"
#include "groupchat.h"
#include "fileTcpClient.h"
#include "fileTcpServer.h"
#include "updatepersonalinfo.h"
#include <QTimer>
#include "addonefriend.h"

#if 1
#include "about.h"
#include "addonefriend.h"
#include "myimage.h"
#include "regist.h"
#include "scrawlclient.h"
#include "scrawlserver.h"
#include "setserver.h"
#include "updatepersonalinfo.h"
#endif

void test()
{
#if 0
    About a;
    a.show();

    AddOneFriend add(NULL,"",1,"",1);
    add.show();

    Chat c("","",1,"",1);
    c.show();

    FileTcpClient cli;
    cli.show();

    FileTcpServer ser("",1);
    ser.show();

    FriendInfo inf;
    inf.show();

    Login login;
    login.show();

    MainWindow m("",1,1,"",RegistRequest("","","","","","","",""));
    m.show();

    MyImage mm("");
    mm.show();

    OneHuman one(1,"",1,"",1,NULL);
    one.show();

    Regist r(NULL,"",1,"");
    r.show();

    Scrawl s("","",1,"",1);
    s.show();

    ScrawlServer serv(NULL,"",1);
    serv.show();

    ScrawlClient clie(NULL,QHostAddress::LocalHost,1,"");
    clie.show();

    SetServer sets;
    sets.show();
#endif
}

#if 1
int main(int argc, char *argv[])
{
    QApplication application(argc,argv);

#if 0
    QThread thread;
    thread.start();

    LoginCheck login;
    login.show();
    QTimer timer;
    timer.start(3000);
    QObject::connect(&timer,SIGNAL(timeout()),&login,SLOT(hide()));

    login.moveToThread(&thread);
#endif
    Login login;
    login.show();

    //test();
    //OneHuman one(10001,"ddd",10001,"ddd",NULL);
    //one.show();
    //AddOneFriend *addOneFriend = new AddOneFriend(NULL,"",11,"",2);//这个地方写死了,需要升级的地方
    //addOneFriend -> show();

    return application.exec();
}
#endif


#if 0
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Chat w(NULL,"kang");
    //w.show();

    //SetServer w;
    //w.show();

    //Regist w;

    //MainWindow w;
    //w.show();

    //Human w;
    //w.show();

    //MainWindow w;
    //w.show();

    //GroupChat w;
    //w.show();

    return a.exec();
}
#endif

#if 0
int main(int argc, char *argv[])
{
    QApplication application(argc,argv);

    FileTcpServer server;
    server.show();

    return application.exec();
}
#endif

#if 0
int main(int argc, char *argv[])
{
    QApplication application(argc,argv);

    QTimer *timer = new QTimer;
    timer->start(4000);

    LoginCheck *check = new LoginCheck;
    check->show();
    QObject::connect(timer,SIGNAL(timeout()),check,SLOT(hide()));

    return application.exec();
}
#endif

#if 0
int main(int argc, char *argv[])
{
    QApplication application(argc,argv);

    SetServer s;
    s.show();

    return application.exec();
}
#endif
