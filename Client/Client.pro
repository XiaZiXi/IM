#-------------------------------------------------
#
# Project created by QtCreator 2016-02-01T12:10:38
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp \
    login.cpp \
    logincheck.cpp \
    setserver.cpp \
    regist.cpp \
    friendinfo.cpp \
    chat.cpp \
    scrawl.cpp \
    mainwindow.cpp \
    onehuman.cpp \
    about.cpp \
    fileTcpClient.cpp \
    fileTcpServer.cpp \
    updatepersonalinfo.cpp \
    addonefriend.cpp \
    const.cpp \
    scrawlclient.cpp \
    scrawlserver.cpp \
    myimage.cpp

HEADERS  += \
    login.h \
    setserver.h \
    regist.h \
    friendinfo.h \
    logincheck.h \
    chat.h \
    scrawl.h \
    mainwindow.h \
    onehuman.h \
    about.h \
    fileTcpClient.h \
    fileTcpServer.h \
    sqls.h \
    updatepersonalinfo.h \
    bases.h \
    addonefriend.h \
    const.h \
    scrawlclient.h \
    scrawlserver.h \
    myimage.h

RESOURCES += \
    images/images.qrc

DISTFILES += \
    images/Scrawl.jpg \
    images/about.png \
    images/addGroup.png \
    images/addOne.png
