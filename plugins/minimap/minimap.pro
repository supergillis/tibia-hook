include(../plugins.pri)

TARGET = minimap
DESTDIR = $${DESTDIR}/minimap

SOURCES = \
    MiniMapPlugin.cpp
HEADERS = \
    MiniMapPlugin.h \
    MiniMapPluginInterface.h \
    MiniMapInterface.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
