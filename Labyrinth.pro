#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T13:35:22
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Labyrinth
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    labyrinth.cpp \
    item.cpp \
    treasure.cpp \
    player.cpp \
    dsu.cpp \
    labyrinthobject.cpp \
    direction.cpp \
    gamelog.cpp \
    labyrinthdata.cpp \
    joinserver.cpp \
    menuwindow.cpp \
    hostserver.cpp \
    gamewindow.cpp \
    serverdata.cpp \
    server.cpp \
    network.cpp \
    splitter.cpp \
    labyrinthobjectfactory.cpp\
    trap.cpp \
    client.cpp

HEADERS  += labyrinth.h \
    item.h \
    treasure.h \
    player.h \
    dsu.h \
    labyrinthobject.h \
    direction.h \
    gamelog.h \
    labyrinthdata.h \
    joinserver.h \
    menuwindow.h \
    hostserver.h \
    gamewindow.h \
    serverdata.h \
    server.h \
    network.h \
    splitter.h \
    labyrinthobjectfactory.h \
    trap.h \
    client.h

FORMS    +=    joinserver.ui \
    menuwindow.ui \
    hostserver.ui \
    gamewindow.ui

RESOURCES +=   resources.qrc

