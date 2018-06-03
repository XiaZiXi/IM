#ifndef SQLS
#define SQLS

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include <QTableView>
#include "bases.h"

//向服务器的界面中插入数据
static bool addMemberOnView(QSqlQueryModel *sqlQueryModel,QTableView *tableView,QSqlQuery *query,int id,QString nickName,QString ip,bool onLine)
{
//创建一个默认的连接然后打开它
    QString online = "";
    if(onLine) online = "是";else online = "否";

    QString sql = QString("insert into memberinfo_onview values(%1,'%2','%3','%4')").arg(id).arg(nickName).arg(ip).arg(online);
    bool queryok = query->exec(sql);

    qDebug() << queryok;
    //if(!queryok) return false;//如果执行语句没有成功的话,原因就是ID重复了(ID 是primary key的),应该向客户端回送系统信息(会员已经登录！).

    sqlQueryModel->setQuery("select *from memberinfo_onview");
    tableView->show();
    tableView->setColumnWidth(0,55);//这个函数必须要在addMemberOnView之后调用方可生效,这也是没有放在setupUI中的原因
    tableView->setColumnWidth(1,90);
    tableView->setColumnWidth(2,136);
    tableView->setColumnWidth(3,50);

#if 0
    query.exec("select * from memberinfo_onview");
    while(query.next())
    {
        int id = query.value(0).toInt();
        QString nickname = query.value(1).toString();
        QString ip = query.value(2).toString();
        QString online = query.value(3).toString();
        qDebug() << id << nickname << ip << online;
    }
#endif
    return true;
}

#if 0
static bool addMemberOnView(int id,QString nickName,QString ip,bool onLine)
{
//创建一个默认的连接然后打开它
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");

    bool ok = db.open();
    if(!ok){qDebug() << "error to open db!because " + db.lastError().text();return false;}

    QSqlQuery query(db);
    query.exec("create table memberinfo_onview(ID int primary key,nickname text,IP text,online text)");

    QString online = "";
    if(onLine) online = "是";else online = "否";

    QString sql = QString("insert into memberinfo_onview values(%1,'%2','%3','%4')").arg(id).arg(nickName).arg(ip).arg(online);
    bool queryok = query.exec(sql);

    qDebug() << queryok;
    if(!queryok) return false;//如果执行语句没有成功的话,原因就是ID重复了(ID 是primary key的),应该向客户端回送系统信息(会员已经登录！).
#if 0
    query.exec("select * from memberinfo_onview");
    while(query.next())
    {
        int id = query.value(0).toInt();
        QString nickname = query.value(1).toString();
        QString ip = query.value(2).toString();
        QString online = query.value(3).toString();
        qDebug() << id << nickname << ip << online;
    }
#endif
    return true;

#endif



static bool ifOnline(QSqlQuery *query,USERID_2T id)
{
    //"create table loginfo(memberid integer not null,ifonline integer not null)"
    query->exec(QString("select ifonline from loginfo where memberid=%1").arg(id));
    bool ret = false;
    query->next();
    int i = query->value(0).toInt();
    if(0 == i)
        ret = false;
    else
        ret = true;
    return ret;
}

static QString getNickname(QSqlQuery *query,USERID_2T memberid)
{
    query->exec(QString("select nickname from memberinfo where memberid=%1").arg(memberid));
    QString ret;
    query->next();//必须要有这个东东！！！！！！！！！！！！！！！醉了！！！！！！！！！！！！！！！！！！
    ret = query->value(0).toString();
    qDebug() << "获取到的值是" << ret;
    return ret;
}

//向loginfo表中插入数据
static bool addLogInfoInDB(QSqlQuery *query,int memberid,int ifonline)
{
    if(query->exec(QString("insert into loginfo values(%1,%2)").arg(memberid).arg(ifonline)))
    {
        qDebug() << "insert loginfo ok";
        return true;
    }
    else
    {
        qDebug() << "insert loginfo failed";
        return false;
    }
}
static QString getFriendIP(QSqlQuery *query,int any2)
{
    //"create table memberinfo_onview(ID int primary key,nickname text,IP text,online text)"
    query->exec(QString("select IP from memberinfo_onview where ID=%1").arg(any2));//尼玛！！！下面这五行代码搞了老子2小时，fuckk.
    QString ret;
    query->next();//必须要有这个东东！！！！！！！！！！！！！！！醉了！！！！！！！！！！！！！！！！！！
    ret = query->value(0).toString();
    qDebug() << "获取到的值是" << ret;
    return ret;
}

static bool updateMyFriendRelationship(QSqlQuery *query,int userid,int any2)
{
    //"create table friendrelationship(memberid integer not null,friendid integer not null)"
    if(query->exec(QString("insert into friendrelationship values(%1,%2)").arg(userid).arg(any2)))
    {
        qDebug() << "insert friendrelationship ok";
        return true;
    }
    else
    {
        qDebug() << "insert friendrelationship failed";
        return false;
    }
}


//更新memberfo表中的数据
static bool updateMemberInfoInDB(QSqlQuery *query,int memberid,RegistRequest registrequest)
{
    QString nickname = registrequest.getnickname();
    QString name = registrequest.getname();
    QString age = registrequest.getage();
    QString password = registrequest.getpassword();
    QString telephone = registrequest.gettelephone();
    QString department = registrequest.getdepartment();
    QString email = registrequest.getemail();
    QString hometown = registrequest.gethometown();

    qDebug()<<memberid<<nickname<<name<<age<<password<<telephone<<department<<email<<hometown;
    //"create table memberinfo(memberid integer primary key,
    //nickname text not null,name text not null,age text not null,
    //password text not null,telephone text,department text,email text,hometown text)"
    if(query->exec(QString("update memberinfo set nickname='%1',name='%2',age='%3',password='%4',telephone='%5',"
                           "department='%6',email='%7',hometown='%8' where memberid=%9").arg(nickname).arg(name)
                   .arg(age).arg(password).arg(telephone).arg(department).arg(email).arg(hometown).arg(memberid)))
    {
        qDebug() << "update memberinfo is ok!!!!!!!!!!!!!!!!!!!!!!!!!";
        return true;
    }
    else
    {
        qDebug() << "update memberinfo is failed!!!!!!!!!!!!!!!!!!!!!!!!!";
        return false;
    }

}

//向memberinfo表中插入数据
static bool addMemberInDB(QSqlQuery *query,int memberid,QString nickname,QString name,QString age,QString password,QString telephone,QString department,QString email,QString hometown,int memberip)
{
    if(query->exec(QString("insert into memberinfo values(null,'%1','%2','%3','%4','%5','%6','%7','%8')").arg(nickname).arg(name).arg(age).arg(password).arg(telephone).arg(department).arg(email).arg(hometown)))//虽然填入的是null，但是由于表的设置，使得该字段是自动增长的
    {
        qDebug() << "query addMemberInDB is ok!!!!!!!!!!!!!!!!!!!!!!!!!";
        qDebug() << query->exec("select max(memberid) from memberinfo");//尼玛！！！！！！！！！！！
        //这个份地方居然返回的是ture！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
        //为毛没有返回一个值啊！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
        qDebug() << "insert memberinfo ok";

        //添加成功后，就可以写入loginfo表了
        bool addlog =  addLogInfoInDB(query,memberid+1,0);
        if(addlog){qDebug() << "addlog ok!";}else qDebug() << "addlog failed!";

        return true;
    }
    else
    {
        qDebug() << "insert memberinfo failed";
        return false;
    }
}


static bool addFriendRelationshipInDB(QSqlQuery *query,int memberid,int friendid)//向friendrelationship表中插入数据
{
    if(query->exec(QString("insert into friendrelationship values(%1,%2)").arg(memberid).arg(friendid)))
    {
        qDebug() << "insert friendrelationship ok";
        return true;
    }
    else
    {
        qDebug() << "insert friendrelationship failed";
        return false;
    }
}


static int getMemberInfoTableLargestID(QSqlQuery *query)//获取表中的最大id
{
    //return query->exec("select max(memberid) from memberinfo");//只是这样子是不行的哦，因为它返回的是true或者是false而不是一个值
    query->exec("select max(memberid) from memberinfo");//尼玛！！！下面这五行代码搞了老子2小时，fuckk.
    int ret;
    query->next();//必须要有这个东东！！！！！！！！！！！！！！！醉了！！！！！！！！！！！！！！！！！！
    ret = query->value(0).toInt();
    qDebug() << "获取到的值是" << ret;
    return ret;
}

static int returnToClienAnID(QSqlQuery *query)//给客户端返回一个注册id
{
    return 1 + getMemberInfoTableLargestID(query);
}

static bool friendIsExist(QSqlQuery *query,int friendid)
{
    bool ret = true;
    //"create table friendrelationship(memberid integer not null,friendid integer not null)"))
    query->exec(QString("select friendid from friendrelationship where friendid=%1").arg(friendid));
    query->next();
    if(query->value(0).toString().isEmpty())
        ret = false;
    if(ret){qDebug() << friendid << "friend is exist";}else qDebug() << friendid <<"friend is not exist";
    return ret;
}
static bool clientIsExist(QSqlQuery *query,int userid)
{
    bool ret = true;
    //"create table loginfo(memberid integer not null,ifonline integer not null)"
    query->exec(QString("select ifonline from loginfo where memberid=%1").arg(userid));
    query->next();
    if(query->value(0).toString().isEmpty())
        ret = false;
    if(ret){qDebug() << userid << "is exist";}else qDebug() << userid <<" is not exist";
    return ret;
}

static bool clientIsCorrectPassword(QSqlQuery *query,int userid,QString password)
{
    bool ret = false;

    //"insert into memberinfo(memberid,nickname,name,age,password) values(10000,'root','root','0','root')";
    query->exec(QString("select password from memberinfo where memberid=%1").arg(userid));
    query->next();
    QString pass = query->value(0).toString();
    qDebug() << "password = " << password << userid << " pass = " << pass;
    if(password == pass)
    {
        qDebug() << userid << "has correct password!!";
        ret = true;
    }
    else
    {
        qDebug() << userid << "has not correct password!!";
    }
    return ret;
}

static bool clientIsOK(QSqlQuery *query,int userid,QString password)//判定某个id是否非法
{
    bool ret = false;
    if(clientIsExist(query,userid) && clientIsCorrectPassword(query,userid,password))
        ret = true;
    if(!ret){qDebug() << userid << "和" << password << "是不匹配的";} else qDebug() << userid << "和" << password << "是匹配的" ;
    return ret;
}

static bool clientIsLogin(QSqlQuery *query,int userid)//判定某个id是否已经登录
{
    bool ret = false;
    query->exec(QString("select ifonline from loginfo where memberid=%1").arg(userid));
    query->next();
    if(1 == query->value(0).toInt())
        ret = true;
    if(ret){qDebug() << userid << "is login!";}else qDebug() << userid << "is not login!";
    return ret;
}


static void updateLoginfoLetItOnline(QSqlQuery* query,int userid)//更新log表，让该id在线
{
    //"create table loginfo(memberid integer not null,ifonline integer not null)"
    query->exec(QString("update loginfo set ifonline=1 where memberid=%1").arg(userid));
    qDebug() << "我经更新了log表";
}

static void setOneUserOffLine(QSqlQuery *query,USERID_2T id)//设置某个人下线
{
    query->exec(QString("update loginfo set ifonline=0 where memberid=%1").arg(id));
}


static void setOneUserOffLineOnView(QSqlQueryModel *sqlQueryModel,QTableView *tableView,QSqlQuery *query,USERID_2T id)
{
    //"create table memberinfo_onview(ID int primary key,nickname text,IP text,online text)"
    QString sql = QString("update memberinfo_onview set online='%1' where ID=%2").arg("否").arg(id);
    bool queryok = query->exec(sql);

    qDebug() << "setOneUserOffLineOnView()" << sql << queryok;
    //if(!queryok) return false;//如果执行语句没有成功的话,原因就是ID重复了(ID 是primary key的),应该向客户端回送系统信息(会员已经登录！).

    sqlQueryModel->setQuery("select *from memberinfo_onview");
    tableView->show();
    tableView->setColumnWidth(0,55);//这个函数必须要在addMemberOnView之后调用方可生效,这也是没有放在setupUI中的原因
    tableView->setColumnWidth(1,90);
    tableView->setColumnWidth(2,136);
    tableView->setColumnWidth(3,50);

}

static void setOneUserOnLineOnView(QSqlQueryModel *sqlQueryModel,QTableView *tableView,QSqlQuery *query,USERID_2T id,QString ip)
{
    //"create table memberinfo_onview(ID int primary key,nickname text,IP text,online text)"
    QString sql = QString("update memberinfo_onview set online='%1' where ID=%2").arg("是").arg(id);
    bool queryok = query->exec(sql);

    qDebug() << "setOneUserOffLineOnView()" << sql << queryok;
    //if(!queryok) return false;//如果执行语句没有成功的话,原因就是ID重复了(ID 是primary key的),应该向客户端回送系统信息(会员已经登录！).

    sqlQueryModel->setQuery("select *from memberinfo_onview");
    tableView->show();
    tableView->setColumnWidth(0,55);//这个函数必须要在addMemberOnView之后调用方可生效,这也是没有放在setupUI中的原因
    tableView->setColumnWidth(1,90);
    tableView->setColumnWidth(2,136);
    tableView->setColumnWidth(3,50);

}

//更新该id的UI信息,其实只是更新了nicknam而已
static void updateOneUserInfoOnView(QSqlQueryModel *sqlQueryModel,QTableView *tableView,QSqlQuery *query,USERID_2T id,QString ip)
{
    //"create table memberinfo_onview(ID int primary key,nickname text,IP text,online text)"
    QString nickname = getNickname(query,id);
    QString sql = QString("update memberinfo_onview set nickname='%1' where ID=%2").arg(nickname).arg(id);
    bool queryok = query->exec(sql);

    qDebug() << "updateOneUserInfoOnView()" << sql << queryok;
    //if(!queryok) return false;//如果执行语句没有成功的话,原因就是ID重复了(ID 是primary key的),应该向客户端回送系统信息(会员已经登录！).

    sqlQueryModel->setQuery("select *from memberinfo_onview");
    tableView->show();
    tableView->setColumnWidth(0,55);//这个函数必须要在addMemberOnView之后调用方可生效,这也是没有放在setupUI中的原因
    tableView->setColumnWidth(1,90);
    tableView->setColumnWidth(2,136);
    tableView->setColumnWidth(3,50);

}
static void setAllUserOffLine(QSqlQuery *query)//设置所有人下线
{
    qDebug() << "设置所有人下线！！！！！";
    int maxid = getMemberInfoTableLargestID(query);
    for(int i = 10001;i <= maxid;++ i)
        query->exec(QString("update loginfo set ifonline=0 where memberid=%1").arg(i));
}
static void setAllUserOffLineOnView(QSqlQueryModel *sqlQueryModel,QTableView *tableView,QSqlQuery *query)
{
    qDebug() << "在VIEW中显示所有人下线！！！！！！";
    int maxid = getMemberInfoTableLargestID(query);
    for(int j = 10001;j <= maxid;++ j)
        setOneUserOffLineOnView(sqlQueryModel,tableView,query,j);
}

static RegistRequest getSelfInfo(QSqlQuery *query,int id)//获取自身的信息
{
    //"create table memberinfo(memberid integer primary key,nickname text not null,name text not null,age text not null,password text not null,telephone text,department text,email text,hometown text)"))//自动增长id字段
    query->exec(QString("select * from memberinfo where memberid=%1").arg(id));
    query->next();//呵呵，这个地方的next只有一次，这些东西还没有搞懂啊！！！！！！！！！！！！
    int memberid = query->record().value(0).toInt();
    QString nickname = query->record().value(1).toString();
    QString name = query->record().value(2).toString();
    QString age = query->record().value(3).toString();
    QString password = query->record().value(4).toString();
    QString telephone = query->record().value(5).toString();
    QString department = query->record().value(6).toString();
    QString email = query->record().value(7).toString();
    QString hometown = query->record().value(8).toString();

    qDebug() << nickname << name << age << password << telephone << department << email << hometown;
    RegistRequest ret(nickname,name,age,password,telephone,department,email,hometown);
    return ret;
}

//这个有点。。。shao
static int getFriendID(QSqlQuery *query,int any2)
{
    //"create table friendrelationship(memberid integer not null,friendid integer not null)"))
    query->exec(QString("select friendid from friendrelationship where memberid=%1").arg(any2));//尼玛！！！下面这五行代码搞了老子2小时，fuckk.
    int ret;
    query->next();//必须要有这个东东！！！！！！！！！！！！！！！醉了！！！！！！！！！！！！！！！！！！
    ret = query->value(0).toInt();
    qDebug() << "获取到的值是" << ret;
    return ret;
}

#endif // SQLS

