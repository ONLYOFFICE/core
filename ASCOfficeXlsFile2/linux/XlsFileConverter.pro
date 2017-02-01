#-------------------------------------------------
#
# Project created by QtCreator 2014-12-16T18:25:28
#
#-------------------------------------------------

QT       -= core
QT       -= gui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES += UNICODE \
    _UNICODE \
     _LINUX_QT \
     LINUX 

CONFIG(debug, debug|release){

message(Debug)
DEFINES += _DEBUG
}
TARGET = ASCXlsConverter

CONFIG  += console
CONFIG  -= app_bundle
CONFIG  += c++11

TEMPLATE = app

INCLUDEPATH += \
    /usr/include/libxml2

SOURCES += \
    XlsFileConverter.cpp \
    ../../Common/DocxFormat/Source/Base/unicode_util.cpp \
    ../../Common/3dParty/pole/pole.cpp \
    ../../Common/DocxFormat/Source/SystemUtility/FileSystem/FilePosix.cpp \
    ../../Common/DocxFormat/Source/SystemUtility/FileSystem/DirectoryPosix.cpp \
    ../../Common/DocxFormat/Source/XML/stringcommon.cpp


############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../build/lib

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

linux-*:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-*:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}

############### destination path ###############

linux-*:contains(QMAKE_HOST.arch, x86_64):{

    message(64 bit)

    unix:!macx: LIBS += -L$$PWD/../../build/lib/linux_64/ -lXlsFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../build/lib/linux_64/libXlsFormatLib.a

    LIBS += -L$$PWD/../../build/lib/linux_64/ -lgraphics
    LIBS += -L$$PWD/../../build/lib/linux_64/ -lASCOfficeUtilsLib
  }

else{

    message(32 bit)

    unix:!macx: LIBS += -L$$PWD/../../build/lib/linux_32/ -lXlsFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../build/lib/linux_32/libXlsFormatLib.a

    LIBS += -L$$PWD/../../build/lib/linux_32/ -lgraphics
    LIBS += -L$$PWD/../../build/lib/linux_32/ -lASCOfficeUtilsLib
}

LIBS += -lboost_regex
LIBS += -lz
LIBS += -lxml2
LIBS += -lcurl

