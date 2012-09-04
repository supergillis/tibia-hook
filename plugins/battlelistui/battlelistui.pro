include(../plugins.pri)

TARGET = battlelistui
DESTDIR = $${DESTDIR}/battlelistui

INCLUDEPATH += ../battlelist

SOURCES = \
    BattleListUIPlugin.cpp \
    BattleListWidget.cpp \
    BattleListView.cpp \
    BattleListModel.cpp
HEADERS = \
    BattleListUIPlugin.h \
    BattleListWidget.h \
    BattleListView.h \
    BattleListModel.h

FORMS += \
    BattleListWidget.ui

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
