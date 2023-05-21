core_mac {
    !use_v8:CONFIG += use_javascript_core
}
core_ios {
    CONFIG += use_javascript_core
}

INCLUDEPATH += $$PWD

use_javascript_core {

    QMAKE_OBJECTIVE_CFLAGS += -fobjc-arc -fobjc-weak

    LIBS += -framework Foundation
    LIBS += -framework JavaScriptCore

    DEFINES += JS_ENGINE_JAVASCRIPTCORE

}
