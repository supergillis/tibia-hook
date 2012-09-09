include(../plugins.pri)

TARGET = walker
DESTDIR = $${DESTDIR}/walker

INCLUDEPATH += include
INCLUDEPATH += ../channels/include
INCLUDEPATH += ../positiontracker/include

SOURCES = \
    src/WalkerPlugin.cpp
HEADERS = \
    src/WalkerPlugin.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
