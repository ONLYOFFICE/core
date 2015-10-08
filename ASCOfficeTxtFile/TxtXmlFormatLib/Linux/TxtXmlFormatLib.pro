#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T13:41:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = TxtXmlFormatLib
TEMPLATE = lib
CONFIG += staticlib
win32 {
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
} else {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
}
############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++ | linux-g++-64 | linux-g++-32:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++ | linux-g++-64 | linux-g++-32:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}
############### destination path ###############

DEFINES += UNICODE \
           _UNICODE \
           _USE_LIBXML2_READER_ \
           _USE_XMLLITE_READER_ \
            USE_LITE_READER \
            BUILD_CONFIG_FULL_VERSION \
            DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += \
    ../../../DesktopEditor/freetype-2.5.2/include

#################### WINDOWS #####################
win32 {
    DEFINES += \
        LIBXML_READER_ENABLED

INCLUDEPATH += ../../../Common/DocxFormat/Source/XML/libxml2/XML/include
}
#################### WINDOWS #####################

#################### LINUX ########################
linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT

INCLUDEPATH += /usr/include/libxml2
}
#################### LINUX ########################


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
