#-------------------------------------------------
#
# Project created by QtCreator 2011-08-31T19:59:51
#
#-------------------------------------------------

QT       += core gui

TARGET = robojournal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    config.cpp \
    entrysearch.cpp \
    buffer.cpp \
    editor.cpp \
    dblogin.cpp \
    configmanager.cpp \
    mysqlcore.cpp \
    psqlcore.cpp \
    sqlitecore.cpp \
    aboutrj.cpp \
    firstrun.cpp \
    journalcreator.cpp \
    tagger.cpp \
    journalselector.cpp \
    entryexporter.cpp

HEADERS  += mainwindow.h \
    config.h \
    entrysearch.h \
    buffer.h \
    editor.h \
    dblogin.h \
    configmanager.h \
    mysqlcore.h \
    psqlcore.h \
    sqlitecore.h \
    aboutrj.h \
    firstrun.h \
    journalcreator.h \
    tagger.h \
    journalselector.h \
    entryexporter.h

FORMS    += mainwindow.ui \
    config.ui \
    entrysearch.ui \
    editor.ui \
    dblogin.ui \
    aboutrj.ui \
    firstrun.ui \
    journalcreator.ui \
    tagger.ui \
    journalselector.ui \
    entryexporter.ui

RESOURCES  = images.qrc

QT += sql


RC_FILE = icon.rc

target.path = /usr/local/bin
INSTALLS += target

win32{
	CONFIG += qt release
}

