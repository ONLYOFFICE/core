#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T19:17:07
#
#-------------------------------------------------

QT       -= core
QT       -= gui

linux-g++{
#64bit
    TARGET =  ASCPdfWriter
}
else{
#32bit
    TARGET =  ASCPdfWriter32
}
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES += UNICODE \
           _UNICODE \
            LINUX \
           _LINUX_QT \
           _USE_LIBXML2_READER_ \
           _USE_XMLLITE_READER_ \
            USE_LITE_READER \
            BUILD_CONFIG_FULL_VERSION \
            FILTER_FLATE_DECODE_ENABLED \
            DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += /usr/include/libxml2
INCLUDEPATH += ../../DesktopEditor/freetype-2.5.2/include
INCLUDEPATH += ../PdfWriterLib/Linux

SOURCES += PdfWriter.cpp


CONFIG(debug, debug|release){

    message(Debug)

linux-g++{

    message(64 bit)

    unix:!macx: LIBS += -L$$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop64-Debug/ -lPdfWriterLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop64-Debug/libPdfWriterLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Debug/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Debug/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Debug/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Debug/libgraphics.a
}

else{

    message(32 bit)

    unix:!macx: LIBS += -L$$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop-Debug/ -lPdfWriterLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop-Debug/libPdfWriterLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Debug/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Debug/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Debug/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Debug/libgraphics.a
}
}

CONFIG(release, debug|release){
    message(Release)
linux-g++{
    message(64 bit)

    unix:!macx: LIBS += -L$$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop64-Release/ -lPdfWriterLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop64-Release/libPdfWriterLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Release/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop64-Release/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Release/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop64-Release/libgraphics.a
}
else{
    message(32 bit)

    unix:!macx: LIBS += -L$$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop-Release/ -lPdfWriterLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../PdfWriterLib/Linux/build-PdfWriterLib-Desktop-Release/libPdfWriterLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Release/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Release/libASCOfficeUtilsLib.a

    unix:!macx: LIBS += -L$$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Release/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServerComponents/DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Release/libgraphics.a
}
}
unix:!macx: LIBS += -lxml2
