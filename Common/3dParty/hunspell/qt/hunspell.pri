hunspell_build_static {
DEFINES += HUNSPELL_STATIC
}

building_hunspell {
    DEFINES += BUILDING_LIBHUNSPELL
}

INCLUDEPATH += \
    $$PWD/../hunspell/src \
     $$PWD/../wasm/src

SOURCES += \
    $$PWD/../hunspell/src/hunspell/affentry.cxx \
    $$PWD/../hunspell/src/hunspell/affixmgr.cxx \
    $$PWD/../hunspell/src/hunspell/csutil.cxx \
    $$PWD/../hunspell/src/hunspell/filemgr.cxx \
    $$PWD/../hunspell/src/hunspell/hashmgr.cxx \
    $$PWD/../hunspell/src/hunspell/hunspell.cxx \
    $$PWD/../hunspell/src/hunspell/hunzip.cxx \
    $$PWD/../hunspell/src/hunspell/phonet.cxx \
    $$PWD/../hunspell/src/hunspell/replist.cxx \
    $$PWD/../hunspell/src/hunspell/suggestmgr.cxx

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
    $$PWD/../hunspell/w_char.hxx 
