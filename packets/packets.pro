TEMPLATE = app
TARGET = packets

QMAKE_CXXFLAGS += -std=c++0x

win32 {
    DEFINES += WIN32
    DEFINES -= UNICODE
    QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}

INCLUDEPATH += ../core/include
DESTDIR = ../bin

SOURCES +=

HEADERS += \
    src/ServerPlayerSkills.h \
    src/ServerTalk.h
