QT += core gui widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

CORE_ROOT_DIR = $$PWD/../../../../../core
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)
core_linux:include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
