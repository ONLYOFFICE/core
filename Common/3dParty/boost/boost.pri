INCLUDEPATH += $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/include
CORE_BOOST_LIBS = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib

core_windows {
    BOOST_POSTFIX = vc140
    core_debug:BOOST_POSTFIX=$$BOOST_POSTFIX-mt-gd-1_58
    core_release:BOOST_POSTFIX=$$BOOST_POSTFIX-mt-1_58

    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -llibboost_system-$$BOOST_POSTFIX -llibboost_filesystem-$$BOOST_POSTFIX
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex-$$BOOST_POSTFIX
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -llibboost_date_time-$$BOOST_POSTFIX
} else {
    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -lboost_system -lboost_filesystem
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -lboost_regex
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -lboost_date_time
}
