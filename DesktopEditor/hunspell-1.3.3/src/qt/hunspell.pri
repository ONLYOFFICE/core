hunspell_build_static {
DEFINES += HUNSPELL_STATIC
}

building_hunspell {
    DEFINES += BUILDING_LIBHUNSPELL
}

INCLUDEPATH += \
    $$PWD/../hunspell \
    $$PWD/../win_api

SOURCES += \
    $$PWD/../hunspell/affentry.cxx \
    $$PWD/../hunspell/affixmgr.cxx \
    $$PWD/../hunspell/csutil.cxx \
    $$PWD/../hunspell/dictmgr.cxx \
    $$PWD/../hunspell/filemgr.cxx \
    $$PWD/../hunspell/hashmgr.cxx \
    $$PWD/../hunspell/hunspell.cxx \
    $$PWD/../hunspell/hunzip.cxx \
    $$PWD/../hunspell/phonet.cxx \
    $$PWD/../hunspell/replist.cxx \
    $$PWD/../hunspell/suggestmgr.cxx

HEADERS += \    
    $$PWD/../hunspell/affentry.hxx \
    $$PWD/../hunspell/affixmgr.hxx \
    $$PWD/../hunspell/atypes.hxx \
    $$PWD/../hunspell/baseaffix.hxx \
    $$PWD/../hunspell/csutil.hxx \
    $$PWD/../hunspell/dictmgr.hxx \
    $$PWD/../hunspell/filemgr.hxx \
    $$PWD/../hunspell/hashmgr.hxx \
    $$PWD/../hunspell/htypes.hxx \
    $$PWD/../hunspell/hunspell.h \
    $$PWD/../hunspell/hunspell.hxx \
    $$PWD/../hunspell/hunzip.hxx \
    $$PWD/../hunspell/langnum.hxx \
    $$PWD/../hunspell/phonet.hxx \
    $$PWD/../hunspell/replist.hxx \
    $$PWD/../hunspell/suggestmgr.hxx \
    $$PWD/../hunspell/w_char.hxx \
    $$PWD/../win_api/config.h
