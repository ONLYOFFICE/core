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
    ../Records/AnimationInfoAtom.h \
    ../Records/BlipEntityAtom.h \
    ../Records/BookmarkEntityAtom.h \
    ../Records/BookmarkSeedAtom.h \
    ../Records/ColorSchemeAtom.h \
    ../Records/CString.h \
    ../Records/CurrentUserAtom.h \
    ../Records/DocRoutingSlipAtom.h \
    ../Records/DocumentAtom.h \
    ../Records/DocumentRecords.h \
    ../Records/DocumentTextInfo.h \
    ../Records/EndDocument.h \
    ../Records/ExCDAudioContainer.h \
    ../Records/ExControlAtom.h \
    ../Records/ExHyperlinkAtom.h \
    ../Records/ExMediaAtom.h \
    ../Records/ExMIDIAudioContainer.h \
    ../Records/ExObjListAtom.h \
    ../Records/ExObjListContainer.h \
    ../Records/ExObjRefAtom.h \
    ../Records/ExOleEmbedAtom.h \
    ../Records/ExOleLinkAtom.h \
    ../Records/ExOleObjAtom.h \
    ../Records/ExVideoContainer.h \
    ../Records/ExWAVAudioEmbeddedContainer.h \
    ../Records/ExWAVAudioLinkContainer.h \
    ../Records/FontEntityAtom.h \
    ../Records/GridSpacing10Atom.h \
    ../Records/GuideAtom.h \
    ../Records/HeadersFootersAtom.h \
    ../Records/InteractiveInfoAtom.h \
    ../Records/KinsokuAtom.h \
    ../Records/MasterPersistAtom.h \
    ../Records/MasterTextPropAtom.h \
    ../Records/MetafileBlob.h \
    ../Records/MouseTextInteractiveInfoAtom.h \
    ../Records/NamedShowSlidesAtom.h \
    ../Records/NormalViewSetInfoAtom.h \
    ../Records/NotesAtom.h \
    ../Records/NotesPersistAtom.h \
    ../Records/NoZoomViewInfoAtom.h \
    ../Records/OutlineTextRefAtom.h \
    ../Records/PersistDirectoryAtom.h \
    ../Records/PlaceHolderAtom.h \
    ../Records/RecordsIncluder.h \
    ../Records/RoundTripColorMappingAtom.h \
    ../Records/RoundTripCompositeMainMasterId12Atom.h \
    ../Records/RoundTripNewPlaceholderId12Atom.h \
    ../Records/RoundTripOriginalMainMasterId12Atom.h \
    ../Records/RoundTripThemeAtom.h \
    ../Records/ShapeFlags10Atom.h \
    ../Records/ShapeFlagsAtom.h \
    ../Records/SlideAtom.h \
    ../Records/SlideColorSchemeAtom.h \
    ../Records/SlideContainer.h \
    ../Records/SlideListWithText.h \
    ../Records/SlidePersistAtom.h \
    ../Records/SlideViewInfoAtom.h \
    ../Records/SoundCollAtom.h \
    ../Records/SoundCollectionContainer.h \
    ../Records/SoundContainer.h \
    ../Records/SoundDataBlob.h \
    ../Records/SSDocInfoAtom.h \
    ../Records/SSlideLayoutAtom.h \
    ../Records/SSSlideInfoAtom.h \
    ../Records/StyleTextPropAtom.h \
    ../Records/TextBookmarkAtom.h \
    ../Records/TextBytesAtom.h \
    ../Records/TextCFExceptionAtom.h \
    ../Records/TextCharsAtom.h \
    ../Records/TextFullSettings.h \
    ../Records/TextHeaderAtom.h \
    ../Records/TextInteractiveInfoAtom.h \
    ../Records/TextMasterStyleAtom.h \
    ../Records/TextPFExceptionAtom.h \
    ../Records/TextRulerAtom.h \
    ../Records/TextSIExceptionAtom.h \
    ../Records/TextSpecInfoAtom.h \
    ../Records/UserEditAtom.h \
    ../Records/VBAInfoAtom.h \
    ../Records/ViewInfoAtom.h \
    ../Records/ZoomViewInfoAtom.h \
    ../Records/Animations/AnimationTypes.h \
    ../Records/Drawing/BlipStoreContainer.h \
    ../Records/Drawing/BlipStoreEntry.h \
    ../Records/Drawing/ChildAnchor.h \
    ../Records/Drawing/ClientAnchor.h \
    ../Records/Drawing/DrawingContainer.h \
    ../Records/Drawing/DrawingGroup.h \
    ../Records/Drawing/DrawingRecord.h \
    ../Records/Drawing/GroupShape.h \
    ../Records/Drawing/GroupShapeContainer.h \
    ../Records/Drawing/RegGroupItems.h \
    ../Records/Drawing/Shape.h \
    ../Records/Drawing/ShapeContainer.h \
    ../Records/Drawing/ShapeProperties.h \
    ../Records/Drawing/TextBox.h \
    ../Records/Drawing/ArtBlip.h \
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
    ../../../Common/3dParty/pole/pole.h 

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
    ../Records/Animations/AnimationTypes.cpp \
    ../Records/Drawing/ArtBlip.cpp \
    ../Records/Drawing/ShapeContainer.cpp \
    ../PPTXWriter/Converter.cpp \
    ../PPTXWriter/ShapeWriter.cpp
}

SOURCES += \
    ../PPTFormatLib.cpp \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Elements.cpp \
    ../../../ASCOfficePPTXFile/Editor/Drawing/TextAttributesEx.cpp \
    ../../../Common/3dParty/pole/pole.cpp

