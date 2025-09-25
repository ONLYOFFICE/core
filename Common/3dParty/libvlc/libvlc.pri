VLC_BUILD_DIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
VLC_LIB_DIR = $$VLC_BUILD_DIR/lib

INCLUDEPATH += $$VLC_BUILD_DIR/include

LIBS += -L$$VLC_LIB_DIR -lvlc
LIBS += -L$$VLC_LIB_DIR -lvlccore

core_windows:vs2019 {
	QMAKE_CXXFLAGS_RELEASE += -permissive-
	QMAKE_CXXFLAGS += -permissive-
}

apple_silicon:greaterThan(QT_MAJOR_VERSION, 5) {
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

INCLUDEPATH += $$PWD

HEADERS += \
	$$PWD/vlcinstance.h \
	$$PWD/vlcplayer.h \
	$$PWD/vlcmedia.h \
	$$PWD/libvlc_base.h

SOURCES += \
	$$PWD/vlcinstance.cpp \
	$$PWD/vlcplayer.cpp \
	$$PWD/vlcmedia.cpp
