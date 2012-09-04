include(../plugins.pri)

TARGET = battlelist
DESTDIR = $${DESTDIR}/battlelist

SOURCES = BattleListPlugin.cpp
HEADERS = BattleListPlugin.h \
    BattleList.h \
    BattleListPluginInterface.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
