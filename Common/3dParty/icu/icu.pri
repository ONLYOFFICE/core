core_windows {
    message($$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/include)
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/include
}

core_win_64 {
    LIBS        += -L$$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/lib64 -licuuc
}
core_win_32 {
    message($$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu/lib)
    LIBS        += -L$$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/lib -licuuc
}

core_linux {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/usr/local/include

    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicuuc.so.60.2
    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicudata.so.60.2
}

core_mac {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu/source/common

    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicuuc.60.2.dylib
    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicudata.60.2.dylib
}
