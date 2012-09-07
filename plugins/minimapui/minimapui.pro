include(../plugins.pri)

TARGET = minimapui
DESTDIR = $${DESTDIR}/minimapui

INCLUDEPATH += ../minimap/include
INCLUDEPATH += ../pathfinder/include

SOURCES = \
    src/MiniMapUIPlugin.cpp \
    src/MiniMapView.cpp \
    src/MiniMapModel.cpp
HEADERS = \
    src/MiniMapUIPlugin.h \
    src/MiniMapView.h \
    src/MiniMapModel.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
