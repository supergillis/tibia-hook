QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../../core/include
DESTDIR = ../../bin/plugins
