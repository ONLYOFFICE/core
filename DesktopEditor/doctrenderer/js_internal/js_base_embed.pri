core_mac {
    !use_v8:CONFIG += use_javascript_core
}
core_ios {
    CONFIG += use_javascript_core
}

INCLUDEPATH += $$PWD

!use_javascript_core {
    core_android:CONFIG += v8_version_60
    !build_xp {
        include($$PWD/../../../Common/3dParty/v8/v8.pri)
    } else {
        DEFINES += V8_OS_XP
        include($$PWD/../../../Common/3dParty/v8/v8_xp/v8.pri)
    }
}

use_javascript_core {
    QMAKE_OBJECTIVE_CFLAGS += -fobjc-arc -fobjc-weak

    LIBS += -framework Foundation
    LIBS += -framework JavaScriptCore

    DEFINES += JS_ENGINE_JAVASCRIPTCORE
}

defineTest(ADD_FILES_FOR_EMBED_CLASS_HEADER) {
    name_splitted = $$split(ARGS, .)
    name = $$first(name_splitted)
    SOURCES += $$join(name,, embed/common_, .cpp)
    use_javascript_core {
        OBJECTIVE_SOURCES += $$join(name,, embed/jsc_, .mm)
    } else {
        SOURCES += $$join(name,, embed/v8_, .cpp)
    }
    export(SOURCES)
    export(OBJECTIVE_SOURCES)
}
