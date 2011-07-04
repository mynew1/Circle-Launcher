#-------------------------------------------------
#
# Project created by QtCreator 2011-07-01T09:52:16
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = launcher
TEMPLATE = app


SOURCES += main.cpp\
    src/httpdownload.cpp \
    src/gui/mainwindow.cpp \
    src/gui/addonswidget.cpp \
    src/parsers/parser.cpp \
    src/parsers/curse/cursesearchparser.cpp


RESOURCES += \
    rc.qrc

HEADERS += \
    src/httpdownload.h \
    src/gui/mainwindow.h \
    src/gui/addonswidget.h \
    src/parsers/parser.h \
    src/parsers/curse/cursesearchparser.h

FORMS += \
    src/gui/form/mainwindow.ui \
    src/gui/form/addonswidget.ui
