ICU_MAJOR_VER = 58

core_windows {
    exists($$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu) {
        INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/icu/include
    } else {
        INCLUDEPATH += $$PWD/icu/include
    }

    LIBS        += -L$$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build -licuuc
}

core_linux {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/include

    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicuuc.so.$$ICU_MAJOR_VER
    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicudata.so.$$ICU_MAJOR_VER
}

core_mac {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/include

    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicuuc.$${ICU_MAJOR_VER}.dylib
    LIBS        += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/libicudata.$${ICU_MAJOR_VER}.dylib
}

core_ios {
    INCLUDEPATH += $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build/include

    ICU_LIBS_PATH_IOS = $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build
    bundle_xcframeworks {
        xcframework_platform_ios_simulator {
            ICU_LIBS_PATH_IOS = $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build_xc/simulator
        } else {
            ICU_LIBS_PATH_IOS = $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX/build_xc
        }
    }

    LIBS += $$ICU_LIBS_PATH_IOS/libicudata.a
    LIBS += $$ICU_LIBS_PATH_IOS/libicui18n.a
    LIBS += $$ICU_LIBS_PATH_IOS/libicuuc.a
}

core_android {
    INCLUDEPATH += $$PWD/android/build/include
    ICU_LIBS_PATH = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "android_", "")

    LIBS        += $$PWD/android/build/$$ICU_LIBS_PATH/libicuuc.a
    LIBS        += $$PWD/android/build/$$ICU_LIBS_PATH/libicudata.a
}
