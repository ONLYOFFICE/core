use_system_googletest {
    LIBS += -lgtest
    !core_no_gtest_main:LIBS += -lgtest_main
    core_gmock:LIBS += -lgmock
} else {
    CORE_GTEST_PATH=$$PWD/googletest/googletest

    CONFIG += c++14

    CORE_GTEST_PATH_INCLUDE = $$CORE_GTEST_PATH/include

    INCLUDEPATH += $$CORE_GTEST_PATH
    INCLUDEPATH += $$CORE_GTEST_PATH_INCLUDE

    SOURCES += \
        $$CORE_GTEST_PATH/src/gtest-all.cc
    !core_no_gtest_main:SOURCES += $$CORE_GTEST_PATH/src/gtest_main.cc

    core_gmock {
        CORE_GMOCK_PATH=$$PWD/googletest/googlemock

        INCLUDEPATH += $$CORE_GMOCK_PATH
        INCLUDEPATH += $$CORE_GMOCK_PATH/include

        SOURCES += $$CORE_GMOCK_PATH/src/gmock-all.cc
    }
}
