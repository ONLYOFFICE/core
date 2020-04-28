INCLUDEPATH += $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/include
CORE_BOOST_LIBS = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib

core_windows {
    core_debug:CORE_BOOST_LIBS += /debug

    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -llibboost_system -llibboost_filesystem
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -llibboost_date_time
} else {
    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -lboost_system -lboost_filesystem
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -lboost_regex
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -lboost_date_time
}
