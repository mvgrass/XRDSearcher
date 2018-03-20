#-------------------------------------------------
#
# Project created by QtCreator 2017-07-10T21:33:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XRDSearcher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    worker.cpp \
    simpletime.cpp

HEADERS  += mainwindow.h \
    worker.h \
    simpletime.h

FORMS    += mainwindow.ui
