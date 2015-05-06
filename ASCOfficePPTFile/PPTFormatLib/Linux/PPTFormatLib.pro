#-------------------------------------------------
#
# Project created by QtCreator 2014-12-16T18:28:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = PptFormatLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
DEFINES +=  UNICODE \
        _UNICODE \
         LINUX \
        _LINUX_QT\
         USE_ATL_CSTRING \
        PPTX_DEF \
        PPT_DEF \
        ENABLE_PPT_TO_PPTX_CONVERT \
        PPT_FORMAT \
        _USE_LIBXML2_READER_ \
        LIBXML_READER_ENABLED \
        USE_LITE_READER \
        _USE_XMLLITE_READER_ \
        _PRESENTATION_WRITER_ \
        _SVG_CONVERT_TO_IMAGE_  \
        BUILD_CONFIG_FULL_VERSION \
        DONT_WRITE_EMBEDDED_FONTS
        #DISABLE_FILE_DOWNLOADER


INCLUDEPATH += \
    ../../../Common/DocxFormat/Source/XML/libxml2/XML/include

INCLUDEPATH += \
    /usr/include/libxml2 \

INCLUDEPATH += \
    ../../../DesktopEditor/freetype-2.5.2/include

LIBS += -lxml2


unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    ../Reader/AudioOverlay.h \
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
    ../Records/DateTimeMCAtom.h \
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
    ../Records/GenericDateMCAtom.h \
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
    ../Records/SlideNumberMCAtom.h \
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
    ../Records/Drawing/BitmapBlip.h \
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
    ../../../ASCPresentationEditor/PPTXWriter/Converter.h \
    ../../../ASCPresentationEditor/PPTXWriter/CSS.h \
    ../../../ASCPresentationEditor/PPTXWriter/ImageManager.h \
    ../../../ASCPresentationEditor/PPTXWriter/ShapeWriter.h \
    ../../../ASCPresentationEditor/PPTXWriter/StylesWriter.h \
    ../../../ASCPresentationEditor/OfficeDrawing/Element.h \
    ../../../ASCPresentationEditor/OfficeDrawing/ElementProperties.h \
    ../../../ASCPresentationEditor/OfficeDrawing/Elements.h \
    ../../../ASCPresentationEditor/OfficeDrawing/Layout.h \
    ../../../ASCPresentationEditor/OfficeDrawing/Slide.h \
    ../../../ASCPresentationEditor/OfficeDrawing/SVGConverter.h \
    ../../../ASCPresentationEditor/OfficeDrawing/SVGFramework.h \
    ../../../ASCPresentationEditor/OfficeDrawing/SVGTransformer.h \
    ../../../ASCPresentationEditor/OfficeDrawing/TextAttributesEx.h \
    ../../../ASCPresentationEditor/OfficeDrawing/TextStructures.h \
    ../../../ASCPresentationEditor/OfficeDrawing/Theme.h \
    ../PPTFormatLib.h \
    ../../../Common/3dParty/pole/pole.h

SOURCES += \
    ../Reader/PPTDocumentInfoOneUser.cpp \
    ../Reader/Records.cpp \
    ../Reader/SlidePersist.cpp \
    ../Records/Animations/AnimationTypes.cpp \
    ../../../ASCPresentationEditor/PPTXWriter/Converter.cpp \
    ../../../ASCPresentationEditor/PPTXWriter/ShapeWriter.cpp \
    ../../../ASCPresentationEditor/OfficeDrawing/Elements.cpp \
    ../../../ASCPresentationEditor/OfficeDrawing/Layout.cpp \
    ../../../ASCPresentationEditor/OfficeDrawing/SVGConverter.cpp \
    ../../../ASCPresentationEditor/OfficeDrawing/SVGFramework.cpp \
    ../../../ASCPresentationEditor/OfficeDrawing/SVGTransformer.cpp \
    ../../../ASCPresentationEditor/OfficeDrawing/TextAttributesEx.cpp \
    ../../../ASCPresentationEditor/OfficeDrawing/Theme.cpp \
    ../PPTFormatLib.cpp \
    ../../../Common/3dParty/pole/pole.cpp
