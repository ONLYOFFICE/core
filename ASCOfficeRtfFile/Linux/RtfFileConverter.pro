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
    _USE_LIBXML2_READER_ \
    _LINUX_QT \
     LINUX \
    _USE_XMLLITE_READER_ \
    USE_LITE_READER \

TARGET = ASCRtfConverter

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    /usr/include/libxml2

SOURCES += \
    RtfFileConverter.cpp

LIBS += -lxml2

INCLUDEPATH += \
    /usr/include/libxml2

CONFIG(debug, debug|release){

message(Debug)

linux-g++{

    message(64 bit)

    unix:!macx: LIBS += -L$$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop64-Debug/ -lRtfFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop64-Debug/libRtfFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Debug/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Debug/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop64-Debug/ -lDocxFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop64-Debug/libDocxFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Debug/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Debug/libgraphics.a
}

else{

    message(32 bit)

    unix:!macx: LIBS += -L$$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop-Debug/ -lRtfFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop-Debug/libRtfFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Debug/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Debug/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop-Debug/ -lDocxFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop-Debug/libDocxFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Debug/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Debug/libgraphics.a
}
}

CONFIG(release, debug|release){

message(Release)

linux-g++{

    message(64 bit)

    unix:!macx: LIBS += -L$$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop64-Release/ -lRtfFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop64-Release/libRtfFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Release/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Release/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop64-Release/ -lDocxFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop64-Release/libDocxFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Release/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Release/libgraphics.a
}

else{

    message(32 bit)

    unix:!macx: LIBS += -L$$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop-Release/ -lRtfFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../RtfFormatLib/Linux/build-RtfFormatLib-Desktop-Release/libRtfFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Release/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Release/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop-Release/ -lDocxFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../Common/DocxFormat/build-DocxFormatLib-Desktop-Release/libDocxFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Release/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Release/libgraphics.a
}
}


#####################################################
# внешнее подключение сторонних библиотек

unix:!macx: LIBS += -lxml2

unix:!macx: LIBS += -L$$PWD/../../../../../../../usr/local/lib/ -lboost_locale
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../../usr/local/lib/libboost_locale.a

unix:!macx: LIBS += -L$$PWD/../../../../../../../usr/local/lib/ -lboost_system
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../../usr/local/lib/libboost_system.a

########################################################
