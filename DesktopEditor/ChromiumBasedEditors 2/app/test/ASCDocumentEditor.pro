QT  += core gui widgets gui-private widgets-private core-private printsupport

TEMPLATE = app
CONFIG += app_bundle

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/windows/Debug
} else {
    DESTDIR = $$PWD/build/windows/Release
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

win32:contains(QMAKE_TARGET.arch, x86_64):{

DEFINES += JAS_WIN_MSVC_BUILD WIN32

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.02

CONFIG(debug, debug|release) {

    LIBS += -L../../cefbuilds/win64 -llibcef
    LIBS += -L../../../lib/build/Debug/debug -lascdocumentscore

} else {

    LIBS += -L../../cefbuilds/win64 -llibcef
    LIBS += -L../../../lib/build/Release/release -lascdocumentscore

}

    message(windows64)
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{

DEFINES += JAS_WIN_MSVC_BUILD WIN32 _WIN32

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

CONFIG(debug, debug|release) {

    LIBS += -L../../cefbuilds/win32 -llibcef
    LIBS += -L../../../lib/build/Debug/debug -lascdocumentscore

} else {

    LIBS += -L../../cefbuilds/win32 -llibcef
    LIBS += -L../../../lib/build/Release/release -lascdocumentscore

}

    message(windows32)
}
