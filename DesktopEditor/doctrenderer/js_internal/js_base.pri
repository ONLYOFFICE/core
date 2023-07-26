include($$PWD/js_base_embed.pri)

HEADERS += \
    $$PWD/js_base.h \
    $$PWD/js_embed.h

SOURCES += $$PWD/js_base.cpp

HEADERS += $$PWD/js_base_p.h

HEADERS += $$PWD/js_logger.h
SOURCES += $$PWD/js_logger.cpp

!use_javascript_core {

    HEADERS += $$PWD/v8/v8_base.h
    SOURCES += $$PWD/v8/v8_base.cpp

    core_mac:CONFIG += disable_v8_use_inspector
    core_android:CONFIG += disable_v8_use_inspector
    linux_arm64:CONFIG += disable_v8_use_inspector
    build_xp::CONFIG += disable_v8_use_inspector

    !disable_v8_use_inspector:CONFIG += v8_use_inspector

    !build_xp {
        include($$PWD/../../../Common/3dParty/v8/v8.pri)
    } else {
        DEFINES += V8_OS_XP
        include($$PWD/../../../Common/3dParty/v8/v8_xp/v8.pri)
    }

    v8_use_inspector {
        core_windows:DEFINES += WIN32_LEAN_AND_MEAN
        DEFINES += V8_INSPECTOR

        #paths
        V8_INSPECTOR_PATH = $$PWD/v8/inspector

        #inspector files
        SOURCES += \
            $$V8_INSPECTOR_PATH/inspector_pool.cpp \
            $$V8_INSPECTOR_PATH/inspector.cpp \
            $$V8_INSPECTOR_PATH/utils.cpp \
            $$V8_INSPECTOR_PATH/v8_inspector_channel.cpp \
            $$V8_INSPECTOR_PATH/v8_inspector_client.cpp \
            $$V8_INSPECTOR_PATH/websocket_server.cpp

        HEADERS += \
            $$V8_INSPECTOR_PATH/inspector.h \
            $$V8_INSPECTOR_PATH/inspector_pool.h \
            $$V8_INSPECTOR_PATH/utils.h\
            $$V8_INSPECTOR_PATH/v8_inspector_channel.h \
            $$V8_INSPECTOR_PATH/v8_inspector_client.h \
            $$V8_INSPECTOR_PATH/websocket_server.h


        #to include inspector files
        INCLUDEPATH += \
            $$V8_INSPECTOR_PATH

        #inspector lib
        !use_v8_monolith {
            LIBS += -L$$CORE_V8_PATH_LIBS/src/inspector -linspector
        }

        core_linux {
            LIBS += -lpthread
        }

        #boost
        CONFIG += core_boost_date_time
        include($$PWD/../../../Common/3dParty/boost/boost.pri)
    }

}

use_javascript_core {
    HEADERS += $$PWD/jsc/jsc_base.h
    OBJECTIVE_SOURCES += $$PWD/jsc/jsc_base.mm
}
