QT += core gui

include(core.pri)

TEMPLATE = lib
TARGET = hook

INCLUDEPATH += include ../lib/libdetours/src ../lib/libqtjson/src
DESTDIR = ../bin

LIBS += -L../lib \
    -ldetours \
    -lqtjson

unix:LIBS += -lpthread

SOURCES += src/Application.cpp \
    src/DetourManager.cpp \
    src/Hook.cpp \
    src/Settings.cpp \
    src/UIManager.cpp \
    src/PluginManager.cpp \
    src/Memory.cpp

HEADERS += src/Application.h \
    src/DataQueue.h \
    src/DetourManager.h \
    src/DetourSender.h \
    src/Hook.h \
    src/Settings.h \
    src/UIManager.h \
    src/PluginManager.h \
    src/ProxyManager.h \
    src/Memory.h

unix:SOURCES += src/UnixEntry.cpp
win32:SOURCES += src/WindowsEntry.cpp

# Qt5 support
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
