#-------------------------------------------------
#
# Project created by QtCreator 2014-10-03T18:30:31
#
#-------------------------------------------------

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

include(X2tConverter.pri)

SOURCES += ../../src/main.cpp

###############################################################################################################

DESTDIR = $$PWD/../../../build/bin
core_win_32 {
    message("x2t windows 32 build")
    DESTDIR = $$DESTDIR/win_32
}
core_win_64 {
    message("x2t windows 64 build")
    DESTDIR = $$DESTDIR/win_64
}
core_linux_64 {
    message("x2t linux 64 build")
    DESTDIR = $$DESTDIR/linux_64
}
core_linux_32 {
    message("x2t linux 32 build")
    DESTDIR = $$DESTDIR/linux_32
}
core_mac {
    DESTDIR = $$DESTDIR/mac_64
}