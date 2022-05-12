OPENJPEG_VERSION=2.4.0
OPENJPEG_PATH = $$PWD/openjpeg-$$OPENJPEG_VERSION
OPENJPEG_LIB_JP2 = $$OPENJPEG_PATH/src/lib/openjp2

INCLUDEPATH += \
    $$OPENJPEG_LIB_JP2 \
	$$PWD

DEFINES += USE_JPIP
DEFINES += OPJ_STATIC

#CONFIG += use_all_in_one
!use_all_in_one {
    # uncomment for developing
    HEADERS += $$files($$OPENJPEG_LIB_JP2/*.h, false)
    HEADERS += $$PWD/opj_config.h $$PWD/opj_config_private.h $$PWD/opj_apps_config.h

    SOURCES += $$files($$OPENJPEG_LIB_JP2/*.c, false)
    SOURCES -= $$files($$OPENJPEG_LIB_JP2/bench_*, false)
    SOURCES -= $$files($$OPENJPEG_LIB_JP2/test_*, false)
    SOURCES -= $$files($$OPENJPEG_LIB_JP2/*_generate_*, false)
} else {
    SOURCES += $$PWD/opj_engine.cpp
}

HEADERS += $$PWD/opj_bgraframe.h
SOURCES += $$PWD/opj_bgraframe.cpp
