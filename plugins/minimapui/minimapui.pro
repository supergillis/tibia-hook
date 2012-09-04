include(../plugins.pri)

TARGET = minimapui
DESTDIR = $${DESTDIR}/minimapui

INCLUDEPATH += ../minimap

SOURCES = \
    MiniMapUIPlugin.cpp \
    MiniMapView.cpp \
    MiniMapModel.cpp
HEADERS = \
    MiniMapUIPlugin.h \
    MiniMapView.h \
    MiniMapModel.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
