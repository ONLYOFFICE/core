QT  += core gui widgets gui-private widgets-private core-private printsupport

TEMPLATE = app
CONFIG += app_bundle

CONFIG -= debug_and_release debug_and_release_target

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/windows/Debug
} else {
    DESTDIR = $$PWD/build/windows/Release
}

win32:contains(QMAKE_TARGET.arch, x86_64):{
    PLATFORM_BUILD = win64
    message(windows64)
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
    PLATFORM_BUILD = win32
    message(windows32)
}

CONFIG(debug, debug|release) {
    PLATFORM_BUILD2 = $$PLATFORM_BUILD/debug
    message(debug)
} else {
    PLATFORM_BUILD2 = $$PLATFORM_BUILD
}

HEADERS += \
    mainwindow.h \
    qmainpanel.h \
    qwinhost.h \
    qwinwidget.h \
    asctabwidget.h \
    ../../lib/qcefview/qcefview.h \
    QAscApplicationManager.h \
    ../../lib/include/qascprinter.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qmainpanel.cpp \
    qwinhost.cpp \
    qwinwidget.cpp \
    ../../lib/qcefview/qcefview.cpp

RESOURCES += \
    Icons.qrc

LIBS += -lwininet \
        -ldnsapi \
        -lversion \
        -lmsimg32 \
        -lws2_32 \
        -lusp10 \
        -lpsapi \
        -ldbghelp \
        -lwinmm \
        -lshlwapi \
        -lkernel32 \
        -lgdi32 \
        -lwinspool \
        -lcomdlg32 \
        -ladvapi32 \
        -lshell32 \
        -lole32 \
        -loleaut32 \
        -luser32 \
        -luuid \
        -lodbc32 \
        -lodbccp32 \
        -ldelayimp \
        -lcredui \
        -lnetapi32 \
        -lcomctl32 \
        -lrpcrt4 \
        -ldwmapi \
        -lOpenGL32

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.02

LIBS += -L$$PWD/../cefbuilds/$$PLATFORM_BUILD -llibcef
LIBS += -L$$PWD/../corebuilds/$$PLATFORM_BUILD2 -lascdocumentscore
