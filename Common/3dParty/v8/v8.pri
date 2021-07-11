CORE_V8_PATH_OVERRIDE=$$PWD
v8_version_87 {
    CONFIG += c++14
    CONFIG += use_v8_monolith
    DEFINES += V8_VERSION_87_PLUS
    DEFINES += V8_COMPRESS_POINTERS
}

CORE_V8_PATH_INCLUDE    = $$CORE_V8_PATH_OVERRIDE/v8
CORE_V8_PATH_LIBS       = $$CORE_V8_PATH_INCLUDE/out.gn/$$CORE_BUILDS_PLATFORM_PREFIX/obj

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

v8_use_inspector {
    #define
    DEFINES += V8_INSPECTOR

    #paths
    CORE_DIR_PATH = $$PWD/../../..
    V8_INSPECTOR_PATH = $$CORE_DIR_PATH/DesktopEditor/doctrenderer/js_internal/v8/inspector

    #inspector files
    HEADERS += \
            $$V8_INSPECTOR_PATH/channel.h \
            $$V8_INSPECTOR_PATH/client.h \
            $$V8_INSPECTOR_PATH/inspector.h \
            $$V8_INSPECTOR_PATH/singleconnectionserver.h \
            $$V8_INSPECTOR_PATH/singlethreadutils.h
    SOURCES += \
            $$V8_INSPECTOR_PATH/channel.cpp \
            $$V8_INSPECTOR_PATH/client.cpp \
            $$V8_INSPECTOR_PATH/inspector.cpp \
            $$V8_INSPECTOR_PATH/singleconnectionserver.cpp \
            $$V8_INSPECTOR_PATH/singlethreadutils.cpp

    #js base files for inspector
    HEADERS += \
        $$V8_INSPECTOR_PATH/../../js_base.h \
        $$V8_INSPECTOR_PATH/../v8_base.h
    SOURCES += \
        $$V8_INSPECTOR_PATH/../v8_base.cpp

    #to include inspector files
    INCLUDEPATH += \
        $$V8_INSPECTOR_PATH

    #inspector lib
    !use_v8_monolith {
        LIBS += -L$$CORE_V8_PATH_LIBS/src/inspector -linspector
    }

    #boost lib
    BOOST_PATH = $$CORE_DIR_PATH/Common/3dParty/boost/build/$$CORE_BUILDS_PLATFORM_PREFIX
    BOOST_LIB_PATH = $$BOOST_PATH/lib
    LIBS += -L$$BOOST_LIB_PATH -llibboost_date_time-vc140-mt-gd-x64-1_72

    #unicode converter
    ADD_DEPENDENCY(UnicodeConverter, kernel)

    #boost headers
    INCLUDEPATH += \
        $$BOOST_PATH/include
}
