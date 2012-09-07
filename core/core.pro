QT += core gui
TEMPLATE = lib
INCLUDEPATH += ../include ../lib/mologie-detours ../lib/qt-json
DESTDIR = ../bin
TARGET = hook
QMAKE_CXXFLAGS += -std=gnu++0x

SOURCES = Application.cpp \
	DataQueue.cpp \
	DetourManager.cpp \
	Hook.cpp \
	Main.cpp \
    Packet.cpp \
	Settings.cpp \
    UIManager.cpp \
    PluginManager.cpp \
    ProxyManager.cpp

HEADERS = Application.h \
	DataQueue.h \
	DetourManager.h \
	DetourSender.h \
	Hook.h \
    Packet.h \
    PacketBuilder.h \
	Settings.h \
    UIManager.h \
    PluginManager.h \
    ProxyManager.h \
    PacketReader.h
