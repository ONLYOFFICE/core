core_mac {
    !use_v8:CONFIG += use_javascript_core
}
core_ios {
    CONFIG += use_javascript_core
}

INCLUDEPATH += $$PWD

HEADERS += $$PWD/js_base.h

HEADERS += $$PWD/js_logger.h
SOURCES += $$PWD/js_logger.cpp

!use_javascript_core {

    HEADERS += $$PWD/v8/v8_base.h
    SOURCES += $$PWD/v8/v8_base.cpp

    !build_xp {
        include($$PWD/../../../Common/3dParty/v8/v8.pri)
    } else {
        DEFINES += V8_OS_XP
        include($$PWD/../../../Common/3dParty/v8/v8_xp/v8.pri)
    }

    v8_use_inspector {
        #define
        DEFINES += V8_INSPECTOR

        #paths
        V8_INSPECTOR_PATH = $$PWD/v8/inspector

        #inspector files
        HEADERS += \
            $$V8_INSPECTOR_PATH/channel.h \
            $$V8_INSPECTOR_PATH/client.h \
            $$V8_INSPECTOR_PATH/inspector_impl.h \
            $$V8_INSPECTOR_PATH/singleconnectionserver.h \
            $$V8_INSPECTOR_PATH/singlethreadutils.h \
            $$V8_INSPECTOR_PATH/inspector_pool.h \
            $$V8_INSPECTOR_PATH/inspector_interface.h

        SOURCES += \
            $$V8_INSPECTOR_PATH/channel.cpp \
            $$V8_INSPECTOR_PATH/client.cpp \
            $$V8_INSPECTOR_PATH/inspector_impl.cpp \
            $$V8_INSPECTOR_PATH/singleconnectionserver.cpp \
            $$V8_INSPECTOR_PATH/singlethreadutils.cpp \
            $$V8_INSPECTOR_PATH/inspector_pool.cpp \
            $$V8_INSPECTOR_PATH/inspector_interface.cpp


        #to include inspector files
        INCLUDEPATH += \
            $$V8_INSPECTOR_PATH

        #inspector lib
        !use_v8_monolith {
            LIBS += -L$$CORE_V8_PATH_LIBS/src/inspector -linspector
        }

        #boost
        CONFIG += core_boost_date_time
        include($$PWD/../../../Common/3dParty/boost/boost.pri)
    }

}

use_javascript_core {

    HEADERS += $$PWD/jsc/jsc_base.h
    OBJECTIVE_SOURCES += $$PWD/jsc/jsc_base.mm
    QMAKE_OBJECTIVE_CFLAGS += -fobjc-arc -fobjc-weak

    LIBS += -framework JavaScriptCore

    DEFINES += JS_ENGINE_JAVASCRIPTCORE

}
