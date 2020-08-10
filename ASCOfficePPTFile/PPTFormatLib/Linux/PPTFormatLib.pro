#-------------------------------------------------
#
# Project created by QtCreator 2014-12-16T18:28:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = PptFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../Common/base.pri)

#BOOST
include($$PWD/../../../Common/3dParty/boost/boost.pri)

DEFINES +=  UNICODE \
        _UNICODE \
        _PRESENTATION_WRITER_ \
        _SVG_CONVERT_TO_IMAGE_  \
        DONT_WRITE_EMBEDDED_FONTS
        #DISABLE_FILE_DOWNLOADER

HEADERS += \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Animations.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Attributes.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/AudioOverlay.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Document.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Effects.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Interactive.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Metric.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/SlideShow.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Structures.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Timing.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/XmlStringWriter.h \
    ../PPTFormatLib.h \
    ../Reader/ClassesAtom.h \
    ../Reader/CommonZLib.h \
    ../Reader/PPTDocumentInfo.h \
    ../Reader/PPTDocumentInfoOneUser.h \
    ../Reader/PPTFileDefines.h \
    ../Reader/PPTFileReader.h \
    ../Reader/ReadStructures.h \
    ../Reader/Records.h \
    ../Reader/Slide.h \
    ../Reader/SlideInfo.h \
    ../Reader/SlidePersist.h \
    ../PPTXWriter/Converter.h \
    ../PPTXWriter/ImageManager.h \
    ../PPTXWriter/ShapeWriter.h \
    ../PPTXWriter/StylesWriter.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Element.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Elements.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Layout.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Slide.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/TextAttributesEx.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/TextStructures.h \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Theme.h \
    ../../../Common/3dParty/pole/pole.h  \
    ../Records/Enums/_includer.h \
    ../Records/Enums/enums.h \
    ../Records/FStreams_FStorages/_includer.h \
    ../Records/RecordsIncluder.h \
    ../Records/SCommon/_includer.h \
    ../Records/TAnimation/_includer.h \
    ../Records/TBasic/_includer.h \
    ../Records/TDocument/_includer.h \
    ../Records/TExternalObject/_includer.h \
    ../Records/TFStructure/_includer.h \
    ../Records/TOther/_includer.h \
    ../Records/TShape/_includer.h \
    ../Records/TSlide/_includer.h \
    ../Records/TSlideShow/_includer.h \
    ../Records/TText/_includer.h

core_release {
SOURCES += \
    pptformatlib_logic.cpp
}

core_debug {
SOURCES += \
    ../Reader/ReadStructures.cpp \
    ../Reader/PPTDocumentInfoOneUser.cpp \
    ../Reader/Records.cpp \
    ../Reader/PPTFileReader.cpp \
    ../Reader/SlidePersist.cpp \
    ../PPTXWriter/Converter.cpp \
    ../PPTXWriter/ShapeWriter.cpp
}

SOURCES += \
    ../PPTFormatLib.cpp \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Elements.cpp \
    ../../../ASCOfficePPTXFile/Editor/Drawing/TextAttributesEx.cpp \
    ../../../Common/3dParty/pole/pole.cpp

