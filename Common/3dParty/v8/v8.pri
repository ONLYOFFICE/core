CORE_V8_PATH_INCLUDE    = $$CORE_ROOT_DIR/Common/3dParty/v8/v8
CORE_V8_PATH_LIBS       = $$CORE_ROOT_DIR/Common/3dParty/v8/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

INCLUDEPATH += \
    $$CORE_V8_PATH_INCLUDE \
    $$CORE_V8_PATH_INCLUDE/include

core_windows {
    LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base_0 -lv8_base_1 -lv8_base_2 -lv8_base_3 -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$CORE_V8_PATH_LIBS -licui18n -licuuc

    LIBS += -lwinmm
    LIBS += -ladvapi32
    LIBS += -lShell32
}

core_linux {
    LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$CORE_V8_PATH_LIBS -licui18n -licuuc -licudata
}

core_mac {
    LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$CORE_V8_PATH_LIBS -licui18n -licuuc -licudata

    QMAKE_CXXFLAGS += -Wall -Wno-inconsistent-missing-override
    QMAKE_CFLAGS += -Wall -Wno-inconsistent-missing-override
}

DEFINES += NEW_V8_ENGINE
