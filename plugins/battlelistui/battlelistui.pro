include(../plugins.pri)

TARGET = battlelistui
DESTDIR = $${DESTDIR}/battlelistui

INCLUDEPATH += ../battlelist/include

SOURCES = \
    src/BattleListUIPlugin.cpp \
    src/BattleListWidget.cpp \
    src/BattleListView.cpp \
    src/BattleListModel.cpp
HEADERS = \
    src/BattleListUIPlugin.h \
    src/BattleListWidget.h \
    src/BattleListView.h \
    src/BattleListModel.h

FORMS += \
    src/BattleListWidget.ui

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
