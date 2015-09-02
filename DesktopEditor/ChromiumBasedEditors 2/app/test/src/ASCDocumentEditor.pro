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

mac {
    PLATFORM_BUILD = mac
    message(mac)
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
}

linux {
    LIBS += -L$$PWD/../../cefbuilds/$$PLATFORM_BUILD -lcef
}

mac {

DEFINES += _MAC

#PROJECT_PATH = $$PWD
PROJECT_PATH = ..

QMAKE_LFLAGS += -F$${PROJECT_PATH}/../../cefbuilds/mac
LIBS += -framework "Chromium Embedded Framework"

BUNDLE_TARGET = $${PROJECT_PATH}/build/mac/Debug/$${TARGET}.app

QMAKE_POST_LINK += mkdir -p $${BUNDLE_TARGET}/Contents/Frameworks;
QMAKE_POST_LINK += cp -R $${PROJECT_PATH}/../../cefbuilds/mac/ASCDocumentEditor\ Helper.app $${BUNDLE_TARGET}/Contents/Frameworks;
QMAKE_POST_LINK += cp -R $${PROJECT_PATH}/../../cefbuilds/mac/Chromium\ Embedded\ Framework.framework $${BUNDLE_TARGET}/Contents/Frameworks;
QMAKE_POST_LINK += cp -R $${PROJECT_PATH}/../../corebuilds/mac/debug/libascdocumentscore.dylib $${BUNDLE_TARGET}/Contents/Frameworks;

QMAKE_POST_LINK += install_name_tool \
-change \
@executable_path/Chromium\ Embedded\ Framework \
@executable_path/../Frameworks/Chromium\ Embedded\ Framework.framework/Chromium\ Embedded\ Framework \
$${BUNDLE_TARGET}/Contents/MacOS/ASCDocumentEditor;

QMAKE_POST_LINK += install_name_tool \
-change \
libascdocumentscore.dylib \
@executable_path/../Frameworks/libascdocumentscore.dylib \
$${BUNDLE_TARGET}/Contents/MacOS/ASCDocumentEditor;

QMAKE_POST_LINK += install_name_tool \
-change \
@executable_path/Chromium\ Embedded\ Framework \
@executable_path/../Frameworks/Chromium\ Embedded\ Framework.framework/Chromium\ Embedded\ Framework \
$${BUNDLE_TARGET}/Contents/Frameworks/libascdocumentscore.dylib

}

LIBS += -L$$PWD/../../corebuilds/$$PLATFORM_BUILD2 -lascdocumentscore
