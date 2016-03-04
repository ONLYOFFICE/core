#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11

INCLUDEPATH += \
    ../../agg-2.4/include \
    ../../freetype-2.5.2/include

TEMPLATE = app

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_32
}

LIBS += -L$$DESTINATION_SDK_PATH -lgraphics -lOfficeUtils

win32 {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

linux-g++ | linux-g++-64 | linux-g++-32 {
}

SOURCES += main.cpp
