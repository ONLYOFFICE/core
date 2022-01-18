#-------------------------------------------------
#
# Project created by QtCreator 2014-10-03T18:30:31
#
#-------------------------------------------------

QT       -= core
QT       -= gui

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include(../../../Common/base.pri)

core_windows {
    QMAKE_LFLAGS += /INCREMENTAL:NO
} else {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY

core_windows {
	DEFINES += _RWSTD_NO_SETRLIMIT
}
CORE_BUILDS_LIBRARIES_PATH = $$CORE_ROOT_DIR/build/lib/linux_64/debug
#CORE_BUILDS_LIBRARIES_PATH = $$CORE_ROOT_DIR/build/lib/win_64/debug windows
#kernel
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel
#kernel_network
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel_network
#ADD_DEPENDENCY(kernel, kernel_network)

#####################################################
# внешнее подключение сторонних библиотек

#BOOST
CONFIG += core_boost_regex
include($$PWD/../../../Common/3dParty/boost/boost.pri)

core_windows {
    LIBS += -lAdvapi32
}
########################################################

