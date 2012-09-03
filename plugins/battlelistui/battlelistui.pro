include(../plugins.pri)

INCLUDEPATH += ../battlelist

TARGET = battlelistui
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
