include(../plugins.pri)

TARGET = minimap
DESTDIR = $${DESTDIR}/minimap

SOURCES = \
    MiniMapPlugin.cpp \
    MiniMapPart.cpp \
    MiniMapFloor.cpp
HEADERS = \
    MiniMapPlugin.h \
    MiniMapPluginInterface.h \
    MiniMapInterface.h \
    MiniMapPartInterface.h \
    MiniMapFloorInterface.h \
    MiniMapPart.h \
    MiniMapFloor.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
