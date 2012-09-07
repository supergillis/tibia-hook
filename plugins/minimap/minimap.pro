include(../plugins.pri)

TARGET = minimap
DESTDIR = $${DESTDIR}/minimap

INCLUDEPATH += include

SOURCES = \
    src/MiniMapPlugin.cpp \
    src/MiniMapPart.cpp \
    src/MiniMapFloor.cpp
HEADERS = \
    src/MiniMapPlugin.h \
    src/MiniMapPart.h \
    src/MiniMapFloor.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
