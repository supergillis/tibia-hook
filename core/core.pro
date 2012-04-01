QT += core gui script
TEMPLATE = lib
INCLUDEPATH += ../include ../lib/mologie-detours ../lib/qt-json
DESTDIR = ../bin
TARGET = hook
QMAKE_CXXFLAGS += -std=gnu++0x

SOURCES = Application.cpp \
	DataQueue.cpp \
	DetourManager.cpp \
	Main.cpp \
	ReadOnlyPacket.cpp \
	ReadWritePacket.cpp \
    Hook.cpp \
    Settings.cpp \
    messages/ServerPlayerStatus.cpp \
    Message.cpp

HEADERS = Application.h \
	Connector.h \
	DataQueue.h \
	DetourManager.h \
	DetourSender.h \
	ReadWritePacket.h \
	Hook.h \
	Settings.h \
    StringException.h \
	Message.h \
	Serializer.h \
	messages/ServerPlayerStatus.h \
    ReadOnlyPacket.h \
    Packet.h \
    ReadOnlyPacketProxy.h \
    ReadWritePacketProxy.h \
	MessageData.h
