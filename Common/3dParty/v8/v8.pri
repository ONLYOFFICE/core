CORE_V8_PATH_OVERRIDE=$$PWD
!v8_version_60:CONFIG += v8_version_89

v8_version_89 {
    CONFIG += c++14
    CONFIG += use_v8_monolith
    DEFINES += V8_VERSION_89_PLUS

    core_win_32:CONFIG += build_platform_32
    core_linux_32:CONFIG += build_platform_32

    core_android {
        isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_armv7):CONFIG += build_platform_32
        isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_x86):CONFIG += build_platform_32
    }

    !build_platform_32:DEFINES += V8_COMPRESS_POINTERS

    CORE_V8_PATH_OVERRIDE = $$PWD/../v8_89
}

CORE_V8_PATH_INCLUDE    = $$CORE_V8_PATH_OVERRIDE/v8
CORE_V8_PATH_LIBS       = $$CORE_V8_PATH_INCLUDE/out.gn/$$CORE_BUILDS_PLATFORM_PREFIX/obj

core_android {
    CORE_V8_PATH_INCLUDE = $$PWD/android/v8
    CORE_V8_PATH_LIBS = $$PWD/android/build

    isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_arm64_v8a):CORE_V8_PATH_LIBS=$$CORE_V8_PATH_LIBS/arm64-v8a
    isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_armv7):    CORE_V8_PATH_LIBS=$$CORE_V8_PATH_LIBS/armeabi-v7a
    isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_x86):      CORE_V8_PATH_LIBS=$$CORE_V8_PATH_LIBS/x86
    isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_x86_64):   CORE_V8_PATH_LIBS=$$CORE_V8_PATH_LIBS/x86_64
}

INCLUDEPATH += \
    $$CORE_V8_PATH_INCLUDE \
    $$CORE_V8_PATH_INCLUDE/include

core_windows {
    CORE_V8_PATH_LIBS = $$CORE_V8_PATH_INCLUDE/out.gn/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/obj

    use_v8_monolith {
        LIBS += -L$$CORE_V8_PATH_LIBS -lv8_monolith
    } else {
        LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -lv8_snapshot -lv8_libsampler
        LIBS += -L$$CORE_V8_PATH_LIBS/third_party/icu -licui18n -licuuc
    }

    LIBS += -lwinmm
    LIBS += -ladvapi32
    LIBS += -lShell32

    LIBS += -lDbgHelp
    LIBS += -lShlwapi
}

core_linux {
    use_v8_monolith {
        LIBS += -L$$CORE_V8_PATH_LIBS -lv8_monolith
    } else {
        SNAPSHOT_LIB=v8_snapshot
        !exists($$CORE_V8_PATH_LIBS/libv8_snapshot.a) {
            SNAPSHOT_LIB=v8_nosnapshot
        }

        LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -l$$SNAPSHOT_LIB -lv8_libsampler
        LIBS += -L$$CORE_V8_PATH_LIBS/third_party/icu -licui18n -licuuc
    }
}

core_mac {

    use_v8_monolith {
        LIBS += -L$$CORE_V8_PATH_LIBS -lv8_monolith
    } else {
        LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -lv8_snapshot -lv8_libsampler
        LIBS += $$CORE_V8_PATH_LIBS/third_party/icu/libicui18n.a
        LIBS += $$CORE_V8_PATH_LIBS/third_party/icu/libicuuc.a
    }

    QMAKE_CXXFLAGS += -Wall -Wno-inconsistent-missing-override
    QMAKE_CFLAGS += -Wall -Wno-inconsistent-missing-override
}

core_android {
    LIBS += -L$$CORE_V8_PATH_LIBS -lv8_monolith
}
