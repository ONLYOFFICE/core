QT += core gui widgets

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

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)
ADD_DEPENDENCY(PdfReader)
ADD_DEPENDENCY(HtmlRenderer)
core_linux:include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel -lgraphics

