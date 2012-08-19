QT += core gui script
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
	PacketBuilder.cpp \
	Settings.cpp

HEADERS = Application.h \
	Connector.h \
	DataQueue.h \
	DetourManager.h \
	DetourSender.h \
	Hook.h \
    Packet.h \
    PacketBuilder.h \
	PacketReader.h \
	Settings.h \
	StringException.h
