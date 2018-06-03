#include "server.h"
#include "setserver.h"
#include "sendsysmsg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server w;
    w.show();

    //SetServer w;
    //w.show();

    //SendSysMsg s;
    //s.show();
    return a.exec();
}
