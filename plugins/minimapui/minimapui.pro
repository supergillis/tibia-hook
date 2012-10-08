include(../plugins.pri)

TARGET = minimapui
DESTDIR = $${DESTDIR}/minimapui

INCLUDEPATH += include
INCLUDEPATH += ../minimap/include
INCLUDEPATH += ../pathfinder/include
INCLUDEPATH += ../positiontracker/include
INCLUDEPATH += ../walker/include

SOURCES = \
    src/MiniMapUIPlugin.cpp \
    src/MiniMapView.cpp \
    src/HpaStarCluster.cpp \
    src/HpaStarNode.cpp \
    src/HpaStarEdge.cpp \
    src/HpaStar.cpp
HEADERS = \
    src/MiniMapUIPlugin.h \
    src/MiniMapView.h \
    src/HpaStarCluster.h \
    src/HpaStarNode.h \
    src/HpaStarEdge.h \
    src/HpaStar.h

OTHER_FILES += \
    meta.js \
    config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
