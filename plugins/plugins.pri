QT += core gui

include(../core/core.pri)

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../../core/include
DESTDIR = ../../bin/plugins

# Qt5 support
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}
