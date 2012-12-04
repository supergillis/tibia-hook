include(../core/core.pri)

QT += core gui

TEMPLATE = lib
win32:CONFIG += dll

INCLUDEPATH += ../../core/include
DESTDIR = ../../bin/plugins
