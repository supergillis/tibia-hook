include(../plugins.pri)

QMAKE_CXXFLAGS += -std=c++0x

TARGET = channels
DESTDIR = $${DESTDIR}/channels

INCLUDEPATH += include

SOURCES = \
    src/ChannelsPlugin.cpp
HEADERS = \
    src/ChannelsPlugin.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
