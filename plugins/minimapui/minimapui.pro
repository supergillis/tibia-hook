include(../plugins.pri)

INCLUDEPATH += ../minimap

TARGET = minimapui
SOURCES = \
    MiniMapUIPlugin.cpp \
    MiniMapView.cpp \
    MiniMapModel.cpp
HEADERS = \
    MiniMapUIPlugin.h \
    MiniMapView.h \
    MiniMapModel.h

FORMS +=
