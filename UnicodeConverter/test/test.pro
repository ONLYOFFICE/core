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

DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY
CONFIG -= debug_and_release debug_and_release_target

CONFIG += c++11
TEMPLATE = app

QMAKE_LFLAGS += -Wl,--rpath=./

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../SDK/lib

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
    #LIBS += -L$$PWD/../icubuilds/linux64/usr/local/lib -licuuc -licudata
    LIBS        += $$PWD/../icubuilds/linux64/usr/local/lib/libicuuc.so.55
    LIBS        += $$PWD/../icubuilds/linux64/usr/local/lib/libicudata.so.55
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_32
}

LIBS += -L$$DESTINATION_SDK_PATH -lUnicodeConverter

SOURCES += main.cpp

unix {
    LD_LIBRARY_PATH = ./
}
