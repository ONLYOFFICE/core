CORE_V8_PATH_INCLUDE    = $$CORE_ROOT_DIR/Common/3dParty/v8/v8
CORE_V8_PATH_LIBS       = $$CORE_ROOT_DIR/Common/3dParty/v8/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

INCLUDEPATH += \
    $$CORE_V8_PATH_INCLUDE \
    $$CORE_V8_PATH_INCLUDE/include

core_windows {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu/include
}

core_win_64 {
    LIBS        += -L$$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu/lib64 -licuuc
}
core_win_32 {
    LIBS        += -L$$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu/lib -licuuc
}

core_linux {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/usr/local/include

    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicuuc.so.55
    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicudata.so.55
}

core_mac {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu/source/common

    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicuuc.55.1.dylib
    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicudata.55.1.dylib
}
