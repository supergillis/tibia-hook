include(../plugins.pri)

QMAKE_CXXFLAGS += -std=c++0x

TARGET = positiontracker
DESTDIR = $${DESTDIR}/positiontracker

INCLUDEPATH += include

SOURCES = \
    src/PositionTrackerPlugin.cpp
HEADERS = \
    src/PositionTrackerPlugin.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
