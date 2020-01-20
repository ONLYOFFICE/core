INCLUDEPATH += $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/include

MSVC = vc140

CORE_BOOST_LIBS = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib

core_boost_libs {

core_windows {
core_debug {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_system-$$MSVC-mt-gd-1_58 -llibboost_filesystem-$$MSVC-mt-gd-1_58
}
core_release {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_system-$$MSVC-mt-1_58 -llibboost_filesystem-$$MSVC-mt-1_58
}
} else {
    LIBS += -L$$CORE_BOOST_LIBS -lboost_system -lboost_filesystem
}

}

core_boost_regex {

core_windows {
core_debug {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex-$$MSVC-mt-gd-1_58
}
core_release {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex-$$MSVC-mt-1_58
}
} else {
    LIBS += -L$$CORE_BOOST_LIBS -lboost_regex
}

}

core_boost_date_time {

core_windows {
core_debug {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_date_time-$$MSVC-mt-gd-1_58
}
core_release {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_date_time-$$MSVC-mt-1_58
}
} else {
    LIBS += -L$$CORE_BOOST_LIBS -lboost_date_time
}

}
