QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

TEMPLATE = lib
TARGET = hook

INCLUDEPATH += include ../lib/mologie-detours ../lib/qt-json
DESTDIR = ../bin

SOURCES = src/Application.cpp \
	src/DetourManager.cpp \
	src/Hook.cpp \
	src/Main.cpp \
	src/Settings.cpp \
    src/UIManager.cpp \
    src/PluginManager.cpp \
    src/ProxyManager.cpp

HEADERS = src/Application.h \
	src/DataQueue.h \
	src/DetourManager.h \
	src/DetourSender.h \
	src/Hook.h \
    src/PacketBuilder.h \
	src/Settings.h \
    src/UIManager.h \
    src/PluginManager.h \
    src/ProxyManager.h \
    src/PacketReader.h \
    src/Packet.h \
    include/Position.h \
    include/Constants.h \
    include/Direction.h
