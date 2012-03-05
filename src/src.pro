QT += core gui script
TEMPLATE = lib
INCLUDEPATH += ../include ../lib/mologie-detours ../lib/qt-json
DESTDIR = ../bin
TARGET = hook

SOURCES = Application.cpp \
	DataQueue.cpp \
	DetourManager.cpp \
	JsonConfig.cpp \
	Main.cpp \
	Packet.cpp \
	ReadOnlyPacket.cpp \
	ReadWritePacket.cpp \
	ScriptHook.cpp \
	../lib/mologie-detours/hde.cpp \
	../lib/qt-json/json.cpp

HEADERS = Application.h \
	Connector.h \
	DataQueue.h \
	DetourManager.h \
	DetourSender.h \
	JsonConfig.h \
	Packet.h \
	ReadOnlyPacket.h \
	ReadOnlyPacketProxy.h \
	ReadWritePacket.h \
	ReadWritePacketProxy.h \
	ScriptHook.h \
	../lib/mologie-detours/hde.h \
	../lib/qt-json/json.h
