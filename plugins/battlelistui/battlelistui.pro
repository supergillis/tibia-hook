include(../plugins.pri)

INCLUDEPATH += ../battlelist

TARGET = battlelistui
SOURCES = \
    BattleListUIPlugin.cpp \
    BattleListWidget.cpp \
    BattleListTable.cpp \
    BattleListTableModel.cpp
HEADERS = \
    BattleListUIPlugin.h \
    BattleListWidget.h \
    BattleListTable.h \
    BattleListTableModel.h

FORMS += \
    BattleListWidget.ui
