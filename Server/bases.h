#ifndef BASES_H
#define BASES_H
#include <QtCore>
#include <QVector>
#include <QHostAddress>


/////////////////////////////////////////////////////////////////////
//
/////////////////////公共类型定义
//                   2T--2B的Type
/////////////////////////////////////////////////////////////////////
typedef quint16 PORT_2T;//端口号
typedef quint32 IP_4T;//IP
typedef quint16 CMD_2T;//命令
typedef quint16 ANY_2T;//任意2T
typedef quint32 ANY_4T;//任意4T
typedef ANY_2T USERID_2T;//ID
typedef ANY_2T FAILURE_SUCCEED_2T;//成败类型


#define TEMP_PORT_LOGIN  8888
#define TEMP_PORT_REGIST 7777
#define TEMP_PORT_UPDATE 6666



/////////////////////////////////////////////////////////////////////
//
/////////////////////命令类型
//
/////////////////////////////////////////////////////////////////////
//客户端命令(client to server)
typedef CMD_2T REGIST_CMD_2T_CTOS;//注册请求
typedef CMD_2T LOGIN_CMD_2T_CTOS;//登陆请求
typedef CMD_2T UPDATE_PERSONAL_INFORMATION_CMD_2T_CTOS;//更新个人信息请求
typedef CMD_2T LOGOUT_CMD_2T_CTOS;//登出告知服务器
typedef CMD_2T SEARCH_FRIEND_CMD_2T_CTOS;//推荐好友请求
typedef CMD_2T ADD_FRIEND_CMD_2T_CTOS;//添加好友请求

//客户端命令(client to client)
typedef CMD_2T SEND_MSG_CMD_2T_CTOC;//发送消息

//服务端命令(server to client)
typedef CMD_2T REGIST_CMD_2T_STOC;//注册成败
typedef CMD_2T LOGIN_CMD_2T_STOC;//登陆成败
typedef CMD_2T UPDATE_PERSONAL_INFORMATION_CMD_2T_STOC;//更新个人信息成败
typedef CMD_2T LOGOUT_CMD_2T_STOC;//登出告知好友们
typedef CMD_2T SEARCH_FRIEND_CMD_2T_STOC;//推荐好友
typedef CMD_2T ADD_FRIEND_CMD_2T_STOC;//添加好友
typedef CMD_2T LOGIN_TELL_FRIEND_CMD_2T_STOC;//登入告知好友们
typedef CMD_2T SYSMSG_CMD_2T_STOC;//系统消息
//typedef CMD_2T SOMEONE_WANT_ADD_YOU_CMD_2T_STOC;//有人想要添加你为好友







/////////////////////////////////////////////////////////////////////
//
/////////////////////命令的定义
//
/////////////////////////////////////////////////////////////////////
//客户端命令(client to  server)
const LOGIN_CMD_2T_CTOS LOGIN_CMD_2V_CTOS = (LOGIN_CMD_2T_CTOS)1;
const REGIST_CMD_2T_CTOS REGIST_CMD_2V_CTOS = (REGIST_CMD_2T_CTOS)2;
const UPDATE_PERSONAL_INFORMATION_CMD_2T_CTOS UPDATE_PERSONAL_INFORMATION_CMD_2V_CTOS = (UPDATE_PERSONAL_INFORMATION_CMD_2T_CTOS)3;
const LOGOUT_CMD_2T_CTOS LOGOUT_CMD_2V_CTOS = (LOGOUT_CMD_2T_CTOS)4;
const SEARCH_FRIEND_CMD_2T_CTOS SEARCH_FRIEND_CMD_2V_CTOS = (SEARCH_FRIEND_CMD_2T_CTOS)5;
const ADD_FRIEND_CMD_2T_CTOS ADD_FRIEND_CMD_2V_CTOS = (ADD_FRIEND_CMD_2T_CTOS)6;
//客户端命令(client to client)
const SEND_MSG_CMD_2T_CTOC SEND_MSG_CMD_2V_CTOC = (SEND_MSG_CMD_2T_CTOC)7;
//服务端命令(server to client)
const LOGIN_CMD_2T_STOC LOGIN_CMD_2V_STOC = (LOGIN_CMD_2T_STOC)8;
const REGIST_CMD_2T_STOC REGIST_CMD_2V_STOC = (REGIST_CMD_2T_STOC)9;
const UPDATE_PERSONAL_INFORMATION_CMD_2T_STOC UPDATE_PERSONAL_INFORMATION_CMD_2V_STOC = (UPDATE_PERSONAL_INFORMATION_CMD_2T_STOC)10;
const LOGOUT_CMD_2T_STOC LOGOUT_CMD_2V_STOC = (LOGOUT_CMD_2T_STOC)11;
const SEARCH_FRIEND_CMD_2T_STOC SEARCH_FRIEND_CMD_2V_STOC = (SEARCH_FRIEND_CMD_2T_STOC)12;
const ADD_FRIEND_CMD_2T_STOC ADD_FRIEND_CMD_2V_STOC = (ADD_FRIEND_CMD_2T_STOC)13;
//const SOMEONE_WANT_ADD_YOU_CMD_2T_STOC SOMEONE_WANT_ADD_YOU_CMD_2V_STOC = (SOMEONE_WANT_ADD_YOU_CMD_2T_STOC)14;
const LOGIN_TELL_FRIEND_CMD_2T_STOC LOGIN_TELL_FRIEND_CMD_2V_STOC = (LOGIN_TELL_FRIEND_CMD_2T_STOC)15;
const SYSMSG_CMD_2T_STOC SYSMSG_CMD_2V_STOC = (SYSMSG_CMD_2T_STOC)18;




//服务端常量
const FAILURE_SUCCEED_2T SUCCESSFUL_2V = (FAILURE_SUCCEED_2T)16;
const FAILURE_SUCCEED_2T FAILURE_2V = (FAILURE_SUCCEED_2T)17;
enum{OFFLINE = 0,ONLINE = 1};







/////////////////////////////////////////////////////////////////////
//
/////////////////////数据包头结构
//
/////////////////////////////////////////////////////////////////////
struct DataHead_18T//5-9
{
    CMD_2T cmd;    //命令类型
    USERID_2T id;  //id号同时也是端口号
    ANY_2T segment;//保留1
    ANY_4T reserve;//保留2
    IP_4T myip;//5-9
    IP_4T hisip;//5-9

    friend QDataStream& operator<<(QDataStream& os,DataHead_18T &data)
    {
        os << data.cmd << data.id << data.segment << data.reserve << data.myip << data.hisip;
        return os;
    }
    friend QDataStream& operator>>(QDataStream& is,DataHead_18T &data)
    {
        is >> data.cmd >> data.id >> data.segment >> data.reserve >> data.myip >> data.hisip;
        return is;
    }

    DataHead_18T() : segment(0),reserve(0),myip(0),hisip(0){}
    CMD_2T getcmd(){return this->cmd;}
    USERID_2T getid(){return this->id;}
    ANY_2T getsegment(){return this->segment;}
    ANY_4T getreserve(){return this->reserve;}
    IP_4T getmyip(){return this->myip;}
    IP_4T gethisip(){return this->hisip;}

    void setcmd(CMD_2T cmd){this->cmd = cmd;}
    void setid(USERID_2T id){this->id = id;}
    void setsegment(ANY_2T segment){this->segment = segment;}
    void setreserve(ANY_4T reserve){this->reserve = reserve;}
    void setmyip(IP_4T myip){this->myip = myip;}
    void sethisip(IP_4T hisip){this->hisip = hisip;}
};






/////////////////////////////////////////////////////////////////////
//
/////////////////////数据包体结构
//
/////////////////////////////////////////////////////////////////////
struct RecommandFriendInformation  //推荐好友时填写的信息
{
    USERID_2T memberid;    //账号
    QString nickname;      //昵称
    QString name;          //姓名
    QString age;           //年龄
    QString telephone;     //电话
    QString department;    //部门
    QString email;         //邮箱
    QString hometown;      //家乡

    RecommandFriendInformation(){}
    RecommandFriendInformation(USERID_2T memberid,
                               QString nickname,
                               QString name,
                               QString age,
                               QString telephone,
                               QString department,
                               QString email,
                               QString hometown) :
        memberid(memberid),nickname(nickname),name(name),
        age(age),telephone(telephone),
        department(department),email(email),hometown(hometown){}

    USERID_2T getmemberid(){return memberid;}
    QString getnickname(){return nickname;}
    QString getname(){return name;}
    QString getage(){return age;}
    QString gettelephone(){return telephone;}
    QString getdepartment(){return department;}
    QString getemail(){return email;}
    QString gethometown(){return hometown;}

    //重载输入输出操作符
    friend QDataStream &operator<<(QDataStream &os,const RecommandFriendInformation &pi)
    {
        os << pi.memberid << pi.nickname << pi.name
           << pi.age << pi.telephone
           << pi.department << pi.email << pi.hometown;
        return os;
    }
    friend QDataStream &operator>>(QDataStream &is,RecommandFriendInformation &pi)
    {
        is >> pi.memberid >> pi.nickname >> pi.name
           >> pi.age >> pi.telephone
           >> pi.department >> pi.email >> pi.hometown;
        return is;
    }

};


struct RegistRequest  //注册请求消息
{
    QString nickname;   //昵称
    QString name;       //姓名
    QString age;        //年龄
    QString password;   //密码
    QString telephone;  //电话
    QString department; //部门
    QString email;      //邮箱
    QString hometown;   //家乡

    RegistRequest(){}
    RegistRequest(QString nickname,
                  QString name,
                  QString age,
                  QString password,
                  QString telephone,
                  QString department,
                  QString email,
                  QString hometown) :
    nickname(nickname),name(name),age(age),
    password(password),telephone(telephone),
    department(department),email(email),hometown(hometown){}

    QString getnickname(){return nickname;}
    QString getname(){return name;}
    QString getage(){return age;}
    QString getpassword(){return password;}
    QString gettelephone(){return telephone;}
    QString getdepartment(){return department;}
    QString getemail(){return email;}
    QString gethometown(){return hometown;}

    //重载输入输出操作符
    friend QDataStream &operator<<(QDataStream &os,const RegistRequest &pi)
    {
        os << pi.nickname << pi.name
           << pi.age << pi.password<< pi.telephone
           << pi.department << pi.email << pi.hometown;
        return os;
    }
    friend QDataStream &operator>>(QDataStream &is,RegistRequest &pi)
    {
        is >> pi.nickname >> pi.name
           >> pi.age >> pi.password >> pi.telephone
           >> pi.department >> pi.email >> pi.hometown;
        return is;
    }
};

//客户端 和 服务端 都具有的结构体
struct UserInformation  //用户个人信息
{
    USERID_2T memberid; //账号
    QString nickname;   //昵称
    QString name;       //姓名
    QString age;        //年龄
    QString password;   //密码
    QString telephone;  //电话
    QString department; //部门
    QString email;      //邮箱
    QString hometown;   //家乡

    UserInformation(){}
    UserInformation(USERID_2T memberid,
                    QString nickname,
                    QString name,
                    QString age,
                    QString password,
                    QString telephone,
                    QString department,
                    QString email,
                    QString hometown) :
    memberid(memberid),nickname(nickname),name(name),
    age(age),password(password),telephone(telephone),
    department(department),email(email),hometown(hometown){}

    USERID_2T getmemberid(){return memberid;}
    QString getnickname(){return nickname;}
    QString getname(){return name;}
    QString getage(){return age;}
    QString getpassword(){return password;}
    QString gettelephone(){return telephone;}
    QString getdepartment(){return department;}
    QString getemail(){return email;}
    QString gethometown(){return hometown;}

    //重载输入输出操作符
    friend QDataStream &operator<<(QDataStream &os,const UserInformation &pi)
    {
        os << pi.memberid << pi.nickname << pi.name
           << pi.age << pi.password<< pi.telephone
           << pi.department << pi.email << pi.hometown;
        return os;
    }
    friend QDataStream &operator>>(QDataStream &is,UserInformation &pi)
    {
        is >> pi.memberid >> pi.nickname >> pi.name
           >> pi.age >> pi.password >> pi.telephone
           >> pi.department >> pi.email >> pi.hometown;
        return is;
    }
};

//服务端结构体

//客户端结构体

struct LoginInformation     //登录信息
{
    USERID_2T memberid;
    QString password;

    LoginInformation(){}
    LoginInformation(USERID_2T memberid,
                     QString password) :
    memberid(memberid),password(password){}

    USERID_2T getmemberid(){return memberid;}
    QString getpassword(){return password;}

    friend QDataStream & operator<<(QDataStream &os,const LoginInformation &pi)
    {
        os << pi.memberid << pi.password;
        return os;
    }
    friend QDataStream & operator>>(QDataStream &is,LoginInformation &pi)
    {
        is >> pi.memberid >> pi.password;
        return is;
    }
};






/////////////////////////////////////////////////////////////////////
//
/////////////////////数据包结构
//
/////////////////////////////////////////////////////////////////////
//消息 c--to--c
typedef struct _SEN_MSG//“发送消息”消息
{
    DataHead_18T head;
    QString body;
}SEND_MSG_CTOC;
//消息 c--to--s
typedef struct _REQUEST_REGIST//“发送消息”消息
{
    DataHead_18T head;
    RegistRequest body;
}REQUEST_REGIST_CTOS;
typedef struct _LOGIN_REQUEST//“登录请求”消息
{
    DataHead_18T head;
    QString body;
}LOGIN_REQUEST_CTOS;
typedef struct _UPDATE_INFORMATION//“更新个人信息请求”消息
{
    DataHead_18T head;
    RegistRequest body;
}UPDATE_INFORMATION_CTOS;
typedef struct _LOGOUT//“登出告知服务器”消息
{
    DataHead_18T head;
    QString body;
}LOGOUT_CTOS;
typedef struct _RECOMMAND_REQUEST//“推荐好友请求”消息
{
    DataHead_18T head;
    QString body;
}RECOMMAND_REQUEST_CTOS;
typedef struct _ADD_FRIEND_REQUEST//“添加好友请求”消息
{
    DataHead_18T head;
    QString body;
}ADD_FRIEND_REQUEST_CTOS;
//消息 s--to--c
typedef struct _REGIST_SUCC_OR_FAIL//“注册成败”消息
{
    DataHead_18T head;
    QString body;//成功则为空，失败则为失败原因
}REGIST_SUCC_OR_FAIL_STOC;
typedef struct _SYSMSG//"系统消息"
{
    DataHead_18T head;
    QString body;//系统消息
}SYSMSG_STOC;
typedef struct _LOGIN_SUCC_OR_FAIL//“登录成败”消息
{
    DataHead_18T head;
    RegistRequest body;//成功则返回个人信息，失败则为空
}LOGIN_SUCC_OR_FAIL_STOC;
typedef struct _UPDATE_SUCC_OR_FAIL//“更新个人信息成败”消息
{
    DataHead_18T head;
    QString body;//成功则为空，失败则为失败原因
}UPDATE_SUCC_OR_FAIL_STOC;
typedef struct _LOGOUT_TELL_FRIEND//“登出告知好友们”
{
    DataHead_18T head;
    QString body;
}LOGOUT_TELL_FRIEND_STOC;
//因为推荐好友的个数不确定,暂定位最多10个
typedef struct _RECOMMAND_FRIENDS//“推荐好友”消息
{
    DataHead_18T head;
    RecommandFriendInformation body;
}RECOMMAND_FRIEND_STOC;
typedef struct _ADDFRIEND_SUCC_OR_FAIL//“添加好友成败”消息
{
    DataHead_18T head;
    QString body[2];//成功第一个则为空，第二个为好友昵称，失败则第一个为失败原因
}ADDFRIEND_SUCC_OR_FAIL_STOC;
typedef struct _LOGIN_TELLFRIEND//“登入告知好友们”消息
{
    DataHead_18T head;
    QString body;
}LOGIN_TELLFRIEND_STOC;





/////////////////////////////////////////////////////////////////////
//
/////////////////////构造数据包
//
/////////////////////////////////////////////////////////////////////
//c-to-c
static SEND_MSG_CTOC getSEND_MSG_CTOC(USERID_2T id,QString content)//“发送消息”消息
{
    SEND_MSG_CTOC ret;

    ret.head.setcmd(SEND_MSG_CMD_2V_CTOC);
    ret.head.setid(id);
    ret.body = content;

    return ret;
}
//c-to-s
static REQUEST_REGIST_CTOS  getREQUEST_REGIST_CTOS(USERID_2T id,
                                            QString nickname,QString name,
                                            QString age,QString password,
                                            QString tel,QString department,
                                            QString email,QString hometown)//“注册请求”消息
{
    REQUEST_REGIST_CTOS ret;

    ret.head.setcmd(REGIST_CMD_2V_CTOS);
    ret.head.setid(id);
    ret.body = RegistRequest(nickname,name,age,password,tel,department,email,hometown);

    return ret;
}
static LOGIN_REQUEST_CTOS  getLOGIN_REQUEST_CTOS(USERID_2T id,QString password)//“登录请求”消息
{
    LOGIN_REQUEST_CTOS ret;

    ret.head.setcmd(LOGIN_CMD_2V_CTOS);
    ret.head.setid(id);
    ret.body = password;

    return ret;
}
static UPDATE_INFORMATION_CTOS  getUPDATE_INFORMATION_CTOS(USERID_2T id,
                                                    QString nickname,QString name,
                                                    QString age,QString password,
                                                    QString tel,QString department,
                                                    QString email,QString hometown)//“更新个人信息请求”消息
{
    UPDATE_INFORMATION_CTOS ret;

    ret.head.setcmd(UPDATE_PERSONAL_INFORMATION_CMD_2V_CTOS);
    ret.head.setid(id);
    ret.body = RegistRequest(nickname,name,age,password,tel,department,email,hometown);

    return ret;
}
static LOGOUT_CTOS  getLOGOUT_CTOS(USERID_2T id)//“登出告知服务器”消息
{
    LOGOUT_CTOS ret;

    ret.head.setcmd(LOGOUT_CMD_2V_CTOS);
    ret.head.setid(id);

    return ret;
}
static RECOMMAND_REQUEST_CTOS  getRECOMMAND_REQUEST_CTOS(USERID_2T id)//“推荐好友请求”消息
{
    RECOMMAND_REQUEST_CTOS ret;

    ret.head.setcmd(SEARCH_FRIEND_CMD_2V_CTOS);
    ret.head.setid(id);

    return ret;
}
static ADD_FRIEND_REQUEST_CTOS  getADD_FRIEND_REQUEST_CTOS(USERID_2T id,USERID_2T friendID)//“添加好友请求”消息
{
    ADD_FRIEND_REQUEST_CTOS ret;

    ret.head.setcmd(ADD_FRIEND_CMD_2V_CTOS);
    ret.head.setid(id);
    ret.head.setsegment(friendID);

    return ret;
}
//s-to-c
static REGIST_SUCC_OR_FAIL_STOC  getREGIST_SUCC_OR_FAIL_STOC(USERID_2T id,bool isSuccessful,QString msg = "default error to regist")
{                                                     //“注册成败”消息
    REGIST_SUCC_OR_FAIL_STOC ret;

    ret.head.setcmd(REGIST_CMD_2V_STOC);
    ret.head.setid(id);
    if(isSuccessful)
    {
        ret.head.setsegment(SUCCESSFUL_2V);
    }
    else
    {
        ret.head.setsegment(FAILURE_2V);
        ret.body = msg;
    }
    return ret;
}
static SYSMSG_STOC  getSYSMSG_STOC(QString msg = "default error to getsysmsg")
{                                                     //“发送系统”消息
    SYSMSG_STOC ret;

    ret.head.setcmd(SYSMSG_CMD_2V_STOC);
    ret.body = msg;

    return ret;
}
static LOGIN_SUCC_OR_FAIL_STOC  getLOGIN_SUCC_OR_FAIL_STOC(USERID_2T id,bool isSuccessful)
{                                                   //“登录成败”消息
    LOGIN_SUCC_OR_FAIL_STOC ret;

    ret.head.setcmd(LOGIN_CMD_2V_STOC);
    ret.head.setid(id);
    if(isSuccessful)
    {
        ret.head.setsegment(SUCCESSFUL_2V);
    }
    else
    {
        ret.head.setsegment(FAILURE_2V);
        ret.body = RegistRequest("登录失败","","","","","","","");
    }
    return ret;
}
static UPDATE_SUCC_OR_FAIL_STOC  getUPDATE_SUCC_OR_FAIL_STOC(USERID_2T id,bool isSuccessful,QString msg = "default error to updatePersonalInformation")
{                                                     //“更新个人信息成败”消息
    UPDATE_SUCC_OR_FAIL_STOC ret;

    ret.head.setcmd(UPDATE_PERSONAL_INFORMATION_CMD_2V_STOC);
    ret.head.setid(id);
    if(isSuccessful)
    {
        ret.head.setsegment(SUCCESSFUL_2V);
    }
    else
    {
        ret.head.setsegment(FAILURE_2V);
        ret.body = msg;
    }
    return ret;
}
static LOGOUT_TELL_FRIEND_STOC  getLOGOUT_TELL_FRIEND_STOC(USERID_2T id,USERID_2T friendID)//“登出告知好友们”消息
{
    LOGOUT_TELL_FRIEND_STOC ret;

    ret.head.setcmd(LOGOUT_CMD_2V_STOC);
    ret.head.setid(id);
    ret.head.setsegment(friendID);

    return ret;
}

static RECOMMAND_FRIEND_STOC  getRECOMMAND_FRIEND_STOC(USERID_2T id,
                                                USERID_2T memberid, QString nickname,
                                                QString name,QString age,
                                                QString tel,QString department,
                                                QString email,QString hometown)
{                                               //“推荐好友”消息
    RECOMMAND_FRIEND_STOC ret;

    ret.head.setcmd(SEARCH_FRIEND_CMD_2V_STOC);
    ret.head.setid(id);
    ret.body = RecommandFriendInformation(memberid,nickname,name,age,tel,department,email,hometown);

    return ret;
}

//这个和上面其他不太一样，他没有标志位，因此需要在接收端进行自检测
static ADDFRIEND_SUCC_OR_FAIL_STOC  getADDFRIEND_SUCC_OR_FAIL_STOC(USERID_2T id,bool isSuccessful,QString msg = "default error to addfriend")
{                                                           //“添加好友成败”消息
    ADDFRIEND_SUCC_OR_FAIL_STOC ret;

    ret.head.setcmd(ADD_FRIEND_CMD_2V_STOC);
    ret.head.setid(id);
    if(!isSuccessful)
    {
        ret.body[0] = msg;
    }
    else
    {
        ret.body[0] = "";
        ret.body[1] = "";
    }
    return ret;
}
static LOGIN_TELLFRIEND_STOC  getLOGIN_TELLFRIEND_STOC(USERID_2T id,USERID_2T friendID)//“登入告知好友们”消息
{
    LOGIN_TELLFRIEND_STOC ret;

    ret.head.setcmd(LOGIN_TELL_FRIEND_CMD_2V_STOC);
    ret.head.setid(id);
    ret.head.setsegment(friendID);

    return ret;
}

static QString getIPFromInt(IP_4T ip)
{
    QHostAddress addr = QHostAddress(ip);
    return addr.toString();
}
static IP_4T getIPFromString(QString ip)
{
    QHostAddress addr = QHostAddress(ip);
    return addr.toIPv4Address();
}


#endif // BASES_H

