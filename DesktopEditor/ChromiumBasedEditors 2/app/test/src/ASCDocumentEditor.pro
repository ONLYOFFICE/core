QT  += core gui widgets printsupport

TEMPLATE = app
CONFIG += app_bundle

CONFIG += c++11

CONFIG -= debug_and_release debug_and_release_target

win32:contains(QMAKE_TARGET.arch, x86_64):{
    PLATFORM_BUILD = win64
    message(windows64)
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
    PLATFORM_BUILD = win32
    message(windows32)
}
linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    PLATFORM_BUILD = linux64
    message(linux64)
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    PLATFORM_BUILD = linux32
    message(linux32)
}

CONFIG(debug, debug|release) {
    PLATFORM_BUILD2 = $$PLATFORM_BUILD/debug
    DESTDIR = $$PWD/build/$$PLATFORM_BUILD/Debug
    message(debug)
} else {
    PLATFORM_BUILD2 = $$PLATFORM_BUILD
    DESTDIR = $$PWD/build/$$PLATFORM_BUILD/Release
    message(release)
}

include($$PWD/AscDocumentEditor.pri)

SOURCES += \
    $$PWD/main_cp.cpp

win32 {
    LIBS += -L$$PWD/../../cefbuilds/$$PLATFORM_BUILD -llibcef
} else {
    LIBS += -L$$PWD/../../cefbuilds/$$PLATFORM_BUILD -lcef
}

LIBS += -L$$PWD/../../corebuilds/$$PLATFORM_BUILD2 -lascdocumentscore
