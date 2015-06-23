#-------------------------------------------------
#
# Project created by QtCreator 2015-06-09T16:03:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = QPermuta
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    getrowcoldialog.cpp \
    getanalysesdialog.cpp

HEADERS  += mainwindow.h \
    getrowcoldialog.h \
    getanalysesdialog.h

FORMS    += mainwindow.ui \
    getrowcoldialog.ui \
    getanalysesdialog.ui
