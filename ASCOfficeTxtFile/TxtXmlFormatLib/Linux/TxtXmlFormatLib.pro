#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T13:41:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = TxtXmlFormatLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES += UNICODE \
           _UNICODE \
            LINUX \
           _LINUX_QT \
           _USE_LIBXML2_READER_ \
           _USE_XMLLITE_READER_ \
            USE_LITE_READER \
            BUILD_CONFIG_FULL_VERSION \
            DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += \
    ../../../../DesktopEditor/freetype-2.5.2/include

INCLUDEPATH += /usr/include/libxml2


HEADERS += \
    ../Source/TxtXmlEvent.h \
    ../Source/TxtXmlFile.h \
    ../Source/Common/Encoding.h \
    ../Source/Common/precompiled_utility.h \
    ../Source/Common/StlUtils.h \
    ../Source/Common/ToString.h \
    ../Source/Common/Utility.h \
    ../Source/TxtFormat/File.h \
    ../Source/TxtFormat/TxtFile.h \
    ../Source/TxtFormat/TxtFormat.h \
    ../Source/ConvertDocx2Txt.h \
    ../Source/ConvertTxt2Docx.h

SOURCES += \
    ../Source/TxtXmlFile.cpp \
    ../Source/Common/Encoding.cpp \
    ../Source/Common/ToString.cpp \
    ../Source/TxtFormat/File.cpp \
    ../Source/TxtFormat/TxtFile.cpp \
    ../Source/ConvertDocx2Txt.cpp \
    ../Source/ConvertTxt2Docx.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
