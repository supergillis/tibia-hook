TEMPLATE = app
TARGET = packets

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../core/include
DESTDIR = ../bin

SOURCES += \
    src/Test.cpp

HEADERS += \
    src/Serializer.h \
	src/Serializeable.h \
	src/Packets.h \
	src/ServerPlayerSkills.h \
	src/ServerTalk.h
