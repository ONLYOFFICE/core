TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

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
    _LINUX \
    _LINUX_QT

    message(linux)
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
    ../freetype-2.5.2/include \

CONFIG(debug, debug|release) {
    LIBS += -L../../Qt_build/graphics/Debug/debug -lgraphics
    message(debug)
} else {
    LIBS += -L../../Qt_build/graphics/Release/release -lgraphics
    message(release)
}
