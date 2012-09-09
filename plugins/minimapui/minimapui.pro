include(../plugins.pri)

TARGET = minimapui
DESTDIR = $${DESTDIR}/minimapui

INCLUDEPATH += ../minimap/include
INCLUDEPATH += ../pathfinder/include
INCLUDEPATH += ../positiontracker/include
INCLUDEPATH += ../walker/include

SOURCES = \
    src/MiniMapUIPlugin.cpp \
    src/MiniMapView.cpp
HEADERS = \
    src/MiniMapUIPlugin.h \
    src/MiniMapView.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
