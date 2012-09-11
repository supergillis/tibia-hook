QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = lib
TARGET = hook

INCLUDEPATH += include ../lib/libdetours ../lib/libqtjson
DESTDIR = ../bin

LIBS += -L../lib
LIBS += -ldetours -lqtjson

SOURCES = src/Application.cpp \
	src/DetourManager.cpp \
	src/Hook.cpp \
	src/Main.cpp \
	src/Settings.cpp \
    src/UIManager.cpp \
    src/PluginManager.cpp \
    src/ProxyManager.cpp \
	src/Position.cpp

HEADERS = src/Application.h \
	src/DataQueue.h \
	src/DetourManager.h \
	src/DetourSender.h \
	src/Hook.h \
	src/Settings.h \
    src/UIManager.h \
    src/PluginManager.h \
	src/ProxyManager.h \
