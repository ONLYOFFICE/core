QT       -= core gui

TARGET = OfficeUtils
TEMPLATE = lib
CONFIG += staticlib

win32 {
CONFIG -= debug_and_release debug_and_release_target
CONFIG += build_all_zlib
}

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../SDK/lib

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

include($$PWD/OfficeUtils.pri)
