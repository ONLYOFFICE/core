#-------------------------------------------------
#
# Project created by QtCreator 2014-03-15T18:33:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = hunspell
TEMPLATE = lib

CONFIG += staticlib
CONFIG += building_hunspell

include($$PWD/hunspell.pri)

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}

mac {
    DESTDIR = $$DESTINATION_SDK_PATH/mac_64
}

################################################
