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
    _LINUX \
    LINUX \
    _USE_XMLLITE_READER_ \
    USE_LITE_READER \
    PPTX_DEF\
    PPT_DEF\
    PPT_FORMAT\
    USE_AVSOFFICESTUDIO_XMLUTILS \
    SOLUTION_ASCOFFICEDOCXFILE2 \
    ENABLE_PPT_TO_PPTX_CONVERT \
    _PRESENTATION_WRITER_ \
    NODOCX \
    #DISABLE_FILE_DOWNLOADER \
    BUILD_CONFIG_FULL_VERSION \
    DONT_WRITE_EMBEDDED_FONTS \
    CXIMAGE_DONT_DECLARE_TCHAR

TARGET = ASCPptConverter

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    /usr/include/libxml2

SOURCES += \
    PptFileConverter.cpp

CONFIG(debug, debug|release){
    message(Debug)

    #pptformat
    unix:!macx: LIBS += -L$$PWD/../PPTFormatLib/Linux/build-PPTFormatLib-Desktop-Debug/ -lPptFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../PPTFormatLib/Linux/build-PPTFormatLib-Desktop-Debug/libPptFormatLib.a

    #pptxformat
    unix:!macx: LIBS += -L$$PWD/../../../ASCOfficePPTXFile/PPTXLib/Linux/build-PPTXFormatLib-Desktop-Debug/ -lPPTXFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCOfficePPTXFile/PPTXLib/Linux/build-PPTXFormatLib-Desktop-Debug/libPPTXFormatLib.a

    #docxfile2
    unix:!macx: LIBS += -L$$PWD/../../../ASCOfficeDocxFile2/build-ASCOfficeDocxFile2Lib-Desktop-Debug/ -lASCOfficeDocxFile2Lib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCOfficeDocxFile2/build-ASCOfficeDocxFile2Lib-Desktop-Debug/libASCOfficeDocxFile2Lib.a

    #docxformat
    unix:!macx: LIBS += -L$$PWD/../../../Common/DocxFormat/build-DocxFormatLib-Desktop-Debug/ -lDocxFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../Common/DocxFormat/build-DocxFormatLib-Desktop-Debug/libDocxFormatLib.a

    #htmlRenderer
    unix:!macx: LIBS += -L$$PWD/../../../ASCHTMLRenderer/build-ASCHTMLRendererLib-Desktop-Debug/ -lASCHTMLRendererLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCHTMLRenderer/build-ASCHTMLRendererLib-Desktop-Debug/libASCHTMLRendererLib.a

    #OfficeUtils
    unix:!macx: LIBS += -L$$PWD/../../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Debug/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Debug/libASCOfficeUtilsLib.a

    #Graphics
    unix:!macx: LIBS += -L$$PWD/../../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Debug/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Debug/libgraphics.a
}

CONFIG(release, debug|release){
message(Release)

    #pptformat
    unix:!macx: LIBS += -L$$PWD/../PPTFormatLib/Linux/build-PPTFormatLib-Desktop-Release/ -lPPTFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../PPTFormatLib/Linux/build-PPTFormatLib-Desktop-Release/libPPTFormatLib.a

    #pptxformat
    unix:!macx: LIBS += -L$$PWD/../../../ASCOfficePPTXFile/PPTXLib/Linux/build-PPTXFormatLib-Desktop-Release/ -lPPTXFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCOfficePPTXFile/PPTXLib/Linux/build-PPTXFormatLib-Desktop-Release/libPPTXFormatLib.a

    #docxfile2
    unix:!macx: LIBS += -L$$PWD/../../../ASCOfficeDocxFile2/build-ASCOfficeDocxFile2Lib-Desktop-Release/ -lASCOfficeDocxFile2Lib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCOfficeDocxFile2/build-ASCOfficeDocxFile2Lib-Desktop-Release/libASCOfficeDocxFile2Lib.a

    #docxformat
    unix:!macx: LIBS += -L$$PWD/../../../Common/DocxFormat/build-DocxFormatLib-Desktop-Release/ -lDocxFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../Common/DocxFormat/build-DocxFormatLib-Desktop-Release/libDocxFormatLib.a

    #htmlRenderer
    unix:!macx: LIBS += -L$$PWD/../../../ASCHTMLRenderer/build-ASCHTMLRendererLib-Desktop-Release/ -lASCHTMLRendererLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCHTMLRenderer/build-ASCHTMLRendererLib-Desktop-Release/libASCHTMLRendererLib.a

    #OfficeUtils
    unix:!macx: LIBS += -L$$PWD/../../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Release/ -lASCOfficeUtilsLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ASCOfficeUtils/ASCOfficeUtilsLib/build-ASCOfficeUtilsLib-Desktop-Release/libASCOfficeUtilsLib.a

    #Graphics
    unix:!macx: LIBS += -L$$PWD/../../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Release/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../DesktopEditor/Qt_build/graphics/build-graphics-Desktop-Release/libgraphics.a
}
unix:!macx: LIBS += -lxml2 -lcurl

#unix:!macx: LIBS += -L$$PWD/../../../../../../../../usr/lib/x86_64-linux-gnu/ -lcurl

#INCLUDEPATH += $$PWD/../../../../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../../../../usr/lib/x86_64-linux-gnu

#unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../../../usr/lib/x86_64-linux-gnu/libcurl.a
