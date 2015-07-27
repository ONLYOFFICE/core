TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

############### destination path ###############
DESTDIR = $$PWD/../../SDK/bin/AllFontsGen

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
    TARGET = windows_64
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
    TARGET = windows_32
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    TARGET = linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    TARGET = linux_32
}

################################################

################# dependencies #################
DESTINATION_SDK_PATH = $$PWD/../../SDK/lib
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

LIBS += -L$$DESTINATION_SDK_PATH -lgraphics
LIBS += -L$$DESTINATION_SDK_PATH -lASCOfficeUtilsLib
linux-g++ | linux-g++-64 | linux-g++-32 {
    LIBS += -lz
}
################################################

DEFINES += \
    _QT \
    FT2_BUILD_LIBRARY \
    EXCLUDE_JPG_SUPPORT \
    MNG_SUPPORT_DISPLAY \
    MNG_SUPPORT_READ \
    MNG_SUPPORT_WRITE \
    MNG_ACCESS_CHUNKS \
    MNG_STORE_CHUNKS\
    MNG_ERROR_TELLTALE

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    HAVE_UNISTD_H \
    LINUX \
    _LINUX \
    _LINUX_QT

    message(linux)

    QMAKE_CXXFLAGS += -std=c++0x -pthread
    LIBS += -pthread
}

mac {
    DEFINES += \
    HAVE_UNISTD_H \
    _LINUX \
    _LINUX_QT \
    _MAC \
    QT_MAC

    message(mac)
}

win32 {
    DEFINES += \
    JAS_WIN_MSVC_BUILD \
    WIN32

    DEFINES -= UNICODE
    DEFINES -= _DEBUG

    message(windows)

    LIBS += -lAdvapi32
    LIBS += -lShell32
    LIBS += -lGdi32
    LIBS += -lUser32
}

SOURCES += main.cpp

INCLUDEPATH += \
    ../agg-2.4/include \
    ../freetype-2.5.2/include
