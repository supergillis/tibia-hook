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
    src/Memory.h \
    include/UIManagerInterface.h \
    include/SettingsInterface.h \
    include/Serialization.h \
    include/SenderInterface.h \
    include/ReceiverInterface.h \
    include/ReadOnlyProxyInterface.h \
    include/ProxyInterface.h \
    include/Position.h \
    include/PluginManagerInterface.h \
    include/PluginLoader.h \
    include/PluginInterface.h \
    include/PacketReader.h \
    include/PacketCodes.h \
    include/PacketBuilder.h \
    include/Packet.h \
    include/MemoryInterface.h \
    include/HookInterface.h \
    include/Direction.h \
    include/Constants.h

unix:SOURCES += src/UnixEntry.cpp
win32:SOURCES += src/WindowsEntry.cpp

# Qt5 support
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
