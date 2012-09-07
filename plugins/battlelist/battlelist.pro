include(../plugins.pri)

TARGET = battlelist
DESTDIR = $${DESTDIR}/battlelist

INCLUDEPATH += include

SOURCES = src/BattleListPlugin.cpp
HEADERS = src/BattleListPlugin.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
