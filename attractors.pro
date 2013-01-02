#-------------------------------------------------
#
# Project created by QtCreator 2012-07-08T14:02:30
#
#-------------------------------------------------

QT       += core gui opengl xml

TARGET = attractors
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    strangeattractor.cpp \
    lorenzattractor.cpp \
    xmlhandler.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    strangeattractor.h \
    lorenzattractor.h \
    xmlhandler.h

FORMS    += mainwindow.ui

LIBS += -lglut -lGLU
