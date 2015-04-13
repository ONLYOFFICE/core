#-------------------------------------------------
#
# Project created by QtCreator 2014-03-15T18:33:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = hunspell
TEMPLATE = lib

CONFIG += staticlib
#CONFIG += static
#CONFIG += shared
#CONFIG += plugin

DEFINES += _QT
DEFINES += HUNSPELL_STATIC
DEFINES += BUILDING_LIBHUNSPELL

linux-g++ | linux-g++-64 | linux-g++-32 {
    message(linux)
}

mac {
    message(mac)
}

win32 {
DEFINES += WIN32
DEFINES += _WIN32
    message(windows)
}

INCLUDEPATH += C:/Qt/Qt5.4.0/5.4/msvc2013_64_opengl/include

INCLUDEPATH += ../hunspell \
                ../win_api

SOURCES += \
    ../hunspell/affentry.cxx \
    ../hunspell/affixmgr.cxx \
    ../hunspell/csutil.cxx \
    ../hunspell/dictmgr.cxx \
    ../hunspell/filemgr.cxx \
    ../hunspell/hashmgr.cxx \
    ../hunspell/hunspell.cxx \
    ../hunspell/hunzip.cxx \
    ../hunspell/phonet.cxx \
    ../hunspell/replist.cxx \
    ../hunspell/suggestmgr.cxx

HEADERS += \    
    ../hunspell/affentry.hxx \
    ../hunspell/affixmgr.hxx \
    ../hunspell/atypes.hxx \
    ../hunspell/baseaffix.hxx \
    ../hunspell/csutil.hxx \
    ../hunspell/dictmgr.hxx \
    ../hunspell/filemgr.hxx \
    ../hunspell/hashmgr.hxx \
    ../hunspell/htypes.hxx \
    ../hunspell/hunspell.h \
    ../hunspell/hunspell.hxx \
    ../hunspell/hunzip.hxx \
    ../hunspell/langnum.hxx \
    ../hunspell/phonet.hxx \
    ../hunspell/replist.hxx \
    ../hunspell/suggestmgr.hxx \
    ../hunspell/w_char.hxx \
    ../win_api/config.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=
