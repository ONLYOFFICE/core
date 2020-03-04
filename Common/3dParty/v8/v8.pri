CORE_V8_PATH_INCLUDE    = $$PWD/v8
CORE_V8_PATH_LIBS       = $$CORE_V8_PATH_INCLUDE/out.gn/$$CORE_BUILDS_PLATFORM_PREFIX/obj

INCLUDEPATH += \
    $$CORE_V8_PATH_INCLUDE \
    $$CORE_V8_PATH_INCLUDE/include

core_windows {
    CORE_V8_PATH_LIBS = $$CORE_V8_PATH_INCLUDE/out.gn/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/obj

    LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -lv8_snapshot -lv8_libsampler
    LIBS += -L$$CORE_V8_PATH_LIBS/third_party/icu -licui18n -licuuc

    LIBS += -lwinmm
    LIBS += -ladvapi32
    LIBS += -lShell32

    LIBS += -lDbgHelp
    LIBS += -lShlwapi
}

core_linux {
    SNAPSHOT_LIB=v8_snapshot
    !exists($$CORE_V8_PATH_LIBS/libv8_snapshot.a) {
        SNAPSHOT_LIB=v8_nosnapshot
    }

    LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -l$$SNAPSHOT_LIB -lv8_libsampler
    LIBS += -L$$CORE_V8_PATH_LIBS/third_party/icu -licui18n -licuuc
}

core_mac {
    LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -lv8_snapshot -lv8_libsampler

    #LIBS += -L$$CORE_V8_PATH_LIBS/third_party/icu -licui18n -licuuc
    LIBS += $$CORE_V8_PATH_LIBS/third_party/icu/libicui18n.a
    LIBS += $$CORE_V8_PATH_LIBS/third_party/icu/libicuuc.a

    QMAKE_CXXFLAGS += -Wall -Wno-inconsistent-missing-override
    QMAKE_CFLAGS += -Wall -Wno-inconsistent-missing-override
}
