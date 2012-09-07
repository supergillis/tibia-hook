QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../../core/include
DESTDIR = ../../bin/plugins
