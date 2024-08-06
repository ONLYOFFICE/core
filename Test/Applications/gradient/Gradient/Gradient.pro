QT += core gui widgets

QMAKE_CXXFLAGS += /permissive-

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

PWD_ROOT_DIR = $$PWD
CORE_ROOT_DIR = $$PWD/../../../../../core
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter, HtmlRenderer, PdfFile)
core_linux:include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
