#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T21:20:07
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    setserver.cpp \
    sendsysmsg.cpp

HEADERS  += \
    server.h \
    setserver.h \
    sendsysmsg.h \
    sqls.h \
    bases.h

RESOURCES += \
    images.qrc
