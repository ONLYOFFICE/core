QT		-= core
QT		-= gui

TARGET	 = test
CONFIG	+= console
CONFIG	-= app_bundle

TEMPLATE = app

CONFIG	+= core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../../../../core
CORE_3DPARTY_DIR = $$CORE_ROOT_DIR/Common/3dParty
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

# Comment to inspect simple usage example
# Uncomment to run google tests
CONFIG += json_google_test

json_google_test {
	include($$CORE_3DPARTY_DIR/googletest/googletest.pri)
	DEFINES += JSON_GOOGLE_TEST
}

DESTDIR = $$PWD/build

INCLUDEPATH += ../..

ADD_DEPENDENCY(doctrenderer)

core_linux {
	LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
	LIBS += -ldl
}

SOURCES += \
	main.cpp
