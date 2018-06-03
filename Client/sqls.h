#ifndef SQLS_H
#define SQLS_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include "bases.h"

//CREATE TABLE friendlist(myid integer not null,friendid integer not null,friendip text not null,friendnickname text not null);

//CREATE TABLE selfinfo(myid integer,nickname text not null,name text not null,
//age text not null,password text not null,telephone text,department text,email text,hometown text);

static bool friendIsExist(QSqlQuery *query,int friendid)
{
    bool ret = true;
    //CREATE TABLE friendlist(myid integer not null,friendid integer not null,friendip text not null,friendnickname text not null);
    query->exec(QString("select friendid from friendlist where friendid=%1").arg(friendid));
    query->next();
    if(query->value(0).toString().isEmpty())
        ret = false;
    if(ret){qDebug() << friendid << "friend is exist";}else qDebug() << friendid <<"friend is not exist";
    return ret;
}

static bool addSelfInfoInDB(QSqlQuery *query,int myid,QString nickname,QString name,QString age,QString password,QString telephone,QString department,QString email,QString hometown)
{
    if(query->exec(QString("insert into selfinfo values(%1,'%2','%3','%4','%5','%6','%7','%8','%9')").arg(myid).arg(nickname).arg(name).arg(age).arg(password).arg(telephone).arg(department).arg(email).arg(hometown)))//虽然填入的是null，但是由于表的设置，使得该字段是自动增长的
        qDebug() << "insert selfinfo ok";
    else
        qDebug() << "insert selfinfo failed";
}

static bool addFriendListInDB(QSqlQuery *query,int myid,int friendid,QString friendip,QString friendnickname)
{
    if(query->exec(QString("insert into friendlist values(%1,%2,'%3','%4')").arg(myid).arg(friendid).arg(friendip).arg(friendnickname)))
        qDebug() << "insert friendlist ok";
    else
        qDebug() << "insert friendlist failed";
}


static RegistRequest getSelfInfo(QSqlQuery *query,int id)//获取自身的信息
{
    //"create table memberinfo(memberid integer primary key,nickname text not null,name text not null,age text not null,password text not null,telephont text,department text,email text,hometown text)"))//自动增长id字段
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

static QString getNickname(QSqlQuery *query,USERID_2T memberid)
{
    //"create table selfinfo(myid integer,nickname text not null,name text not null,age text not null,password text not null,telephone text,department text,email text,hometown text)"))//自动增长id字段
    query->exec(QString("select nickname from selfinfo where myid=%1").arg(memberid));
    QString ret;
    query->next();//必须要有这个东东！！！！！！！！！！！！！！！醉了！！！！！！！！！！！！！！！！！！
    ret = query->value(0).toString();
    qDebug() << "获取到的值是" << ret;
    return ret;
}
static QString getFriendNickname(QSqlQuery *query,USERID_2T friendid)
{
    //CREATE TABLE friendlist(myid integer not null,friendid integer not null,friendip text not null,friendnickname text not null);
    query->exec(QString("select friendnickname from friendlist where friendid=%1").arg(friendid));
    QString ret;
    query->next();//必须要有这个东东！！！！！！！！！！！！！！！醉了！！！！！！！！！！！！！！！！！！
    ret = query->value(0).toString();
    qDebug() << "获取到的值是" << ret;
    return ret;
}
static QString getFriendIP(QSqlQuery *query,USERID_2T friendid)
{
    //CREATE TABLE friendlist(myid integer not null,friendid integer not null,friendip text not null,friendnickname text not null);
    query->exec(QString("select friendip from friendlist where friendid=%1").arg(friendid));
    QString ret;
    query->next();//必须要有这个东东！！！！！！！！！！！！！！！醉了！！！！！！！！！！！！！！！！！！
    ret = query->value(0).toString();
    qDebug() << "获取到的值是" << ret;
    return ret;
}


static bool addOneFriendOnFriendList(QSqlQuery *query,USERID_2T myid,ANY_2T friendid,QString friendip,QString friendnickname)
{
    //"create table friendlist(myid integer not null,friendid integer not null,friendip text not null,friendnickname text not null)"

    if(query->exec(QString("insert into friendlist values(%1,%2,'%3','%4')").arg(myid).arg(friendid).arg(friendip).arg(friendnickname)))
        qDebug() << "insert friendlist ok";
    else
        qDebug() << "insert friendlist failed";
}
static bool clientIsExist(QSqlQuery *query,int userid)
{
    bool ret = true;
    //create table selfinfo(myid integer,nickname text not null,name text not null,age text not null,password text not null,telephone text,department text,email text,hometown text)"))//自动增长id字段
    query->exec(QString("select myid from selfinfo where myid=%1").arg(userid));
    query->next();
    if(query->value(0).toString().isEmpty())
        ret = false;
    if(ret){qDebug() << userid << "is exist";}else qDebug() << userid <<" is not exist";
    return ret;
}
static void updateSelfInfoTable(QString age,
                                QString department,
                                QString email,
                                QString hometown,
                                QString name,
                                QString nickname,
                                QString password,
                                QString telephony,
                                QSqlQuery *query,
                                int id)
{
    //create table selfinfo(myid integer,nickname text not null,name text not null,age text not null,password text not null,telephone text,department text,email text,hometown text)"))//自动增长id字段
    query->exec(QString("update selfinfo set nickname='%1',name='%2',age='%3',password='%4',telephone='%5',"
                           "department='%6',email='%7',hometown='%8' where myid=%9").arg(nickname).arg(name)
                   .arg(age).arg(password).arg(telephony).arg(department).arg(email).arg(hometown).arg(id));
}

#endif // SQLS
