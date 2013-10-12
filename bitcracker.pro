#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T12:21:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bitcracker
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    plotview.cpp \
    signal.cpp \
    seqlenanalysis.cpp \
    condition.cpp

HEADERS  += mainwindow.h \
    plotview.h \
    signal.h \
    seqlenanalysis.h \
    condition.h

FORMS    += mainwindow.ui \
    seqlenanalysis.ui
