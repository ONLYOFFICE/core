#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T14:14:33
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = docbuilder
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG -= debug_and_release debug_and_release_target

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../SDK/lib

win32 {
DEFINES += WIN32
}

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/linux_32
}

mac {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/mac_64
}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/Debug
} else {
    DESTDIR = $$PWD/Release
}

################################################

LIBS += -L$$DESTINATION_SDK_PATH_DOCTRENDERER -ldoctrenderer

SOURCES += main.cpp
