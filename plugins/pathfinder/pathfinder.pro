include(../plugins.pri)

QMAKE_CXXFLAGS += -std=c++0x

TARGET = pathfinder
DESTDIR = $${DESTDIR}/pathfinder

INCLUDEPATH += include ../minimap/include

SOURCES = \
	src/PathFinderPlugin.cpp \
    src/JumpPointSearch.cpp \
    HpaCluster.cpp
HEADERS = \
	src/PathFinderPlugin.h \
    src/PriorityQueue.h \
    src/JumpPointSearch.h \
    HpaCluster.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
