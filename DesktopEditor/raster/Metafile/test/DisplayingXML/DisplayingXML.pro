QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CItemWidget.cpp \
    CMapModel.cpp \
    CMetafileTreeView.cpp \
    CStatisticsWidget.cpp \
    CTextEditDelegate.cpp \
    CXMLHighlighter.cpp \
    MainWindow.cpp \
    CCustomView.cpp \
    main.cpp

HEADERS += \
    CItemWidget.h \
    CMapModel.h \
    CMetafileTreeView.h \
    CStatisticsWidget.h \
    CTextEditDelegate.h \
    CXMLHighlighter.h \
    MainWindow.h \
    CCustomView.h

FORMS += \
    MainWindow.ui \
    Statistics.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

SOURCES += ../../../../fontengine/ApplicationFontsWorker.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
