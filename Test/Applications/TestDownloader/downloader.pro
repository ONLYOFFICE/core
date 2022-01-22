QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += console

CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

core_windows {
    QMAKE_LFLAGS += /INCREMENTAL:NO
} else {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY

core_windows {
        DEFINES += _RWSTD_NO_SETRLIMIT
}

#CORE_BUILDS_LIBRARIES_PATH = $$CORE_ROOT_DIR/build/lib/linux_64/debug #linux
CORE_BUILDS_LIBRARIES_PATH = $$CORE_ROOT_DIR/build/lib/win_64/debug #windows
#kernel
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel
#kernel_network
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel_network
#ADD_DEPENDENCY(kernel, kernel_network)

#####################################################
# внешнее подключение сторонних библиотек

#BOOST
CONFIG += core_boost_regex
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

core_windows {
    LIBS += -lAdvapi32
}
########################################################



SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
