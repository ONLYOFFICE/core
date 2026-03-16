use_system_harfbuzz {
    INCLUDEPATH += /usr/include/harfbuzz
    LIBS += -lharfbuzz
} else {
    include($$PWD/harfbuzz_sources.pri)
}
