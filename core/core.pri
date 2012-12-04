win32 {
    CONFIG += exceptions
    DEFINES += WIN32
    DEFINES -= UNICODE
    QMAKE_CXXFLAGS += -U__STRICT_ANSI__
}

# Qt5 support
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
