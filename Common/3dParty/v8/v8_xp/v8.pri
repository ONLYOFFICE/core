CORE_V8_PATH_INCLUDE    = $$PWD/v8
CORE_V8_PATH_LIBS       = $$PWD/$$CORE_BUILDS_PLATFORM_PREFIX

INCLUDEPATH += \
    $$CORE_V8_PATH_INCLUDE \
    $$CORE_V8_PATH_INCLUDE/include

core_windows {
    CORE_V8_PATH_LIBS = $$CORE_V8_PATH_LIBS/$$CORE_BUILDS_CONFIGURATION_PREFIX

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
    LIBS += -L$$CORE_V8_PATH_LIBS -licui18n -licuuc -lv8_libsampler

    QMAKE_CXXFLAGS += -Wall -Wno-inconsistent-missing-override
    QMAKE_CFLAGS += -Wall -Wno-inconsistent-missing-override
}
