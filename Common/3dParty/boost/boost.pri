INCLUDEPATH += $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/include
CORE_BOOST_LIBS = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib

core_android {
    INCLUDEPATH += $$PWD/build/android/include
    CORE_BOOST_LIBS = $$PWD/build/android/lib/$$CORE_BUILDS_PLATFORM_PREFIX
}

bundle_xcframeworks {
    xcframework_platform_ios_simulator {
        CORE_BOOST_LIBS = $$PWD/build/ios_xcframework/ios_simulator/lib/$$CORE_BUILDS_PLATFORM_PREFIX
    } else {
        CORE_BOOST_LIBS = $$PWD/build/ios_xcframework/ios/lib/$$CORE_BUILDS_PLATFORM_PREFIX
    }
}

core_windows {
    VS_VERSION=140
    VS_DEBUG=
    VS_ARCH=x64
    core_debug:VS_DEBUG=gd-
    core_win_32:VS_ARCH=x32
    vs2019:VS_VERSION=142

    BOOST_POSTFIX = -vc$${VS_VERSION}-mt-$${VS_DEBUG}$${VS_ARCH}-1_72

    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -llibboost_system$$BOOST_POSTFIX -llibboost_filesystem$$BOOST_POSTFIX
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex$$BOOST_POSTFIX
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -llibboost_date_time$$BOOST_POSTFIX
} else {
    core_boost_libs:LIBS += -L$$CORE_BOOST_LIBS -lboost_system -lboost_filesystem
    core_boost_regex:LIBS += -L$$CORE_BOOST_LIBS -lboost_regex
    core_boost_date_time:LIBS += -L$$CORE_BOOST_LIBS -lboost_date_time
}
