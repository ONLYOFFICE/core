QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    $$PWD/../lib/goo \
	$$PWD/../lib/fofi \
	$$PWD/../lib/splash \
	$$PWD/../lib

PWD_ROOT_DIR = $$PWD
CORE_ROOT_DIR = $$PWD/../../../core

include($$CORE_ROOT_DIR/Common/base.pri)


ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)
ADD_DEPENDENCY(PdfReader)
ADD_DEPENDENCY(HtmlRenderer)

core_linux:include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
