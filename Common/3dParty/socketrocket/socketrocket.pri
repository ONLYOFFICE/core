core_ios {

    INCLUDEPATH += $$PWD

    LIBS += $$PWD/build/ios/lib/libSoсketRocket.a
}

core_mac {

    INCLUDEPATH += $$PWD 

    LIBS += $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib/libSoсketRocket.a
}
