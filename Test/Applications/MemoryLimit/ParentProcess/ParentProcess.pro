TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

win32 {
	DEFINES += _RWSTD_NO_SETRLIMIT=1
}
