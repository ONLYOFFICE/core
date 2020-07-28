INCLUDEPATH += $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/include
CORE_BOOST_LIBS = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib

core_windows {
    core_win_64:BOOST_POSTFIX = -vc140-mt-x64-1_72
    core_win_32:BOOST_POSTFIX = -vc140-mt-x32-1_72
    core_debug {
        core_win_64:BOOST_POSTFIX = -vc140-mt-gd-x64-1_72
        core_win_32:BOOST_POSTFIX = -vc140-mt-gd-x32-1_72
    }

    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -llibboost_system$$BOOST_POSTFIX -llibboost_filesystem$$BOOST_POSTFIX
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex$$BOOST_POSTFIX
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -llibboost_date_time$$BOOST_POSTFIX
} else {
    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -lboost_system -lboost_filesystem
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -lboost_regex
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -lboost_date_time
}
