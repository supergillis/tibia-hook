include(../plugins.pri)

QMAKE_CXXFLAGS += -std=c++0x

TARGET = pathfinder
DESTDIR = $${DESTDIR}/pathfinder

INCLUDEPATH += include
INCLUDEPATH += ../minimap/include

SOURCES = \
	src/PathFinderPlugin.cpp \
	src/AStar.cpp \
        src/AStarNode.cpp
HEADERS = \
	src/PathFinderPlugin.h \
	src/PriorityQueue.h \
        src/AStar.h \
        src/AStarNode.h

OTHER_FILES += \
	meta.js \
	config.js

settings.path = $${DESTDIR}
settings.files = $${OTHER_FILES}

INSTALLS += settings
