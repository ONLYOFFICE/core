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
    ../Enums/RecordType.h \
    ../Enums/_includer.h \
    ../Enums/enums.h \
    ../PPTFormatLib.h \
    ../PPTXWriter/Animation.h \
    ../PPTXWriter/BulletsConverter.h \
    ../PPTXWriter/TableWriter.h \
    ../PPTXWriter/TxBodyConverter.h \
    ../Reader/ClassesAtom.h \
    ../Reader/CommonZLib.h \
    ../Reader/PPTDocumentInfo.h \
    ../Reader/PPTDocumentInfoOneUser.h \
    ../Reader/PPTFileDefines.h \
    ../Reader/PPTFileReader.h \
    ../Reader/ReadStructures.h \
    ../Reader/Records.h \
    ../Reader/RoundTripExtractor.h \
    ../Reader/Slide.h \
    ../Reader/SlideInfo.h \
    ../Reader/SlidePersist.h \
    ../Records/Animations/AnimationInfoAtom.h \
    ../Records/Animations/AnimationInfoContainer.h \
    ../Records/Animations/BuildAtom.h \
    ../Records/Animations/BuildListContainer.h \
    ../Records/Animations/ChartBuildAtom.h \
    ../Records/Animations/ChartBuildContainer.h \
    ../Records/Animations/ClientVisualElementContainer.h \
    ../Records/Animations/DiagramBuildAtom.h \
    ../Records/Animations/DiagramBuildContainer.h \
    ../Records/Animations/ExtTimeNodeContainer.h \
    ../Records/Animations/HashCode10Atom.h \
    ../Records/Animations/LevelInfoAtom.h \
    ../Records/Animations/ParaBuildAtom.h \
    ../Records/Animations/ParaBuildContainer.h \
    ../Records/Animations/ParaBuildLevel.h \
    ../Records/Animations/SubEffectContainer.h \
    ../Records/Animations/TimeAnimateBehaviorAtom.h \
    ../Records/Animations/TimeAnimateBehaviorContainer.h \
    ../Records/Animations/TimeAnimateColor.h \
    ../Records/Animations/TimeAnimateColorBy.h \
    ../Records/Animations/TimeAnimationValueAtom.h \
    ../Records/Animations/TimeAnimationValueListContainer.h \
    ../Records/Animations/TimeAnimationValueListEntry.h \
    ../Records/Animations/TimeBehaviorAtom.h \
    ../Records/Animations/TimeBehaviorContainer.h \
    ../Records/Animations/TimeColorBehaviorAtom.h \
    ../Records/Animations/TimeColorBehaviorContainer.h \
    ../Records/Animations/TimeCommandBehaviorAtom.h \
    ../Records/Animations/TimeCommandBehaviorContainer.h \
    ../Records/Animations/TimeConditionAtom.h \
    ../Records/Animations/TimeConditionContainer.h \
    ../Records/Animations/TimeDisplayType.h \
    ../Records/Animations/TimeEffectBehaviorAtom.h \
    ../Records/Animations/TimeEffectBehaviorContainer.h \
    ../Records/Animations/TimeEffectID.h \
    ../Records/Animations/TimeEffectNodeType.h \
    ../Records/Animations/TimeEffectType.h \
    ../Records/Animations/TimeGroupID.h \
    ../Records/Animations/TimeIterateDataAtom.h \
    ../Records/Animations/TimeMasterRelType.h \
    ../Records/Animations/TimeModifierAtom.h \
    ../Records/Animations/TimeMotionBehaviorAtom.h \
    ../Records/Animations/TimeMotionBehaviorContainer.h \
    ../Records/Animations/TimeNodeAtom.h \
    ../Records/Animations/TimeNodeTimeFilter.h \
    ../Records/Animations/TimePropertyList4TimeBehavior.h \
    ../Records/Animations/TimePropertyList4TimeNodeContainer.h \
    ../Records/Animations/TimeRotationBehaviorAtom.h \
    ../Records/Animations/TimeRotationBehaviorContainer.h \
    ../Records/Animations/TimeScaleBehaviorAtom.h \
    ../Records/Animations/TimeScaleBehaviorContainer.h \
    ../Records/Animations/TimeSequenceDataAtom.h \
    ../Records/Animations/TimeSetBehaviorAtom.h \
    ../Records/Animations/TimeSetBehaviorContainer.h \
    ../Records/Animations/TimeStringListContainer.h \
    ../Records/Animations/TimeVariant.h \
    ../Records/Animations/TimeVariant4Behavior.h \
    ../Records/Animations/VisualPageAtom.h \
    ../Records/Animations/VisualShapeAtom.h \
    ../Records/Animations/VisualShapeChartElementAtom.h \
    ../Records/Animations/VisualShapeGeneralAtom.h \
    ../Records/Animations/VisualSoundAtom.h \
    ../Records/Animations/_includer.h \
    ../Records/BlipCollection9Container.h \
    ../Records/BlipEntityAtom.h \
    ../Records/BookmarkEntityAtom.h \
    ../Records/BookmarkSeedAtom.h \
    ../Records/BuildAtom.h \
    ../Records/DocInfoListContainer.h \
    ../Records/DocProgTagsContainer.h \
    ../Records/MouseInteractiveInfoContainer.h \
    ../Records/OfficeArtClientData.h \
    ../Records/OfficeArtClientTextbox.h \
    ../Records/PlaceHolderAtom.h \
    ../Records/RoundTrip.h \
    ../Records/ShapeProgBinaryTagSubContainerOrAtom.h \
    ../Records/CFMasks.h \
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
    ../Records/KinsokuContainer.h \
    ../Records/KinsokuFollowingAtom.h \
    ../Records/KinsokuLeadingAtom.h \
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
    ../Records/FontCollection10Container.h \
    ../Records/FontEmbedFlags10Atom.h \
    ../Records/OutlineTextProps9Container.h \
    ../Records/OutlineTextPropsHeaderExAtom.h \
    ../Records/PFMasks.h \
    ../Records/PersistDirectoryAtom.h \
    ../Records/PlaceHolderAtom.h \
    ../Records/ProgStringTagContainer.h \
    ../Records/RecordsIncluder.h \
    ../Records/ShapeFlags10Atom.h \
    ../Records/ShapeFlagsAtom.h \
    ../Records/Comment10Container.h \
    ../Records/LinkedShape10Atom.h \
    ../Records/LinkedSlide10Atom.h \
    ../Records/SlideFlags10Atom.h \
    ../Records/SlideProgTagsContainer.h \
    ../Records/SlideTime10Atom.h \
    ../Records/SlideAtom.h \
    ../Records/SlideColorSchemeAtom.h \
    ../Records/SlideContainer.h \
    ../Records/SlideListWithText.h \
    ../Records/SlidePersistAtom.h \
    ../Records/SlideProgTagsContainer.h \
    ../Records/SlideViewInfoAtom.h \
    ../Records/SoundCollAtom.h \
    ../Records/SoundCollectionContainer.h \
    ../Records/SoundContainer.h \
    ../Records/SoundDataBlob.h \
    ../Records/SSDocInfoAtom.h \
    ../Records/SSlideLayoutAtom.h \
    ../Records/SSSlideInfoAtom.h \
    ../Records/StyleTextPropAtom.h \
    ../Records/StyleTextProp10Atom.h \
    ../Records/StyleTextProp11Atom.h \
    ../Records/StyleTextProp9Atom.h \
    ../Records/TextCFException10.h \
    ../Records/TextCFException9.h \
    ../Records/TextDefaults9Atom.h \
    ../Records/TextMasterStyle9Atom.h \
    ../Records/TextMasterStyle9Level.h \
    ../Records/TextSIException.h \
    ../Records/TextBookmarkAtom.h \
    ../Records/TextBytesAtom.h \
    ../Records/TextCFExceptionAtom.h \
    ../Records/TextCharsAtom.h \
    ../Records/TextFullSettings.h \
    ../Records/TextHeaderAtom.h \
    ../Records/TextInteractiveInfoAtom.h \
    ../Records/TextMasterStyle9Atom.h \
    ../Records/TextMasterStyle9Level.h \
    ../Records/TextMasterStyleAtom.h \
    ../Records/TextPFException9.h \
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
    ../../../Common/3dParty/pole/pole.h  \
    ../Structures/ColorIndex.h \
    ../Structures/ColorIndexStruct.h \
    ../Structures/ColorStruct.h \
    ../Structures/DateTimeStruct.h \
    ../Structures/IStruct.h \
    ../Structures/PointStruct.h \
    ../Structures/RatioStruct.h \
    ../Structures/RectStruct.h \
    ../Structures/ScalingStruct.h \
    ../Structures/SmallRectStruct.h \
    ../Structures/TmsfTimeStruct.h \
    ../Structures/WideColorStruct.h \
    ../Structures/_includer.h

core_release {
SOURCES += \
    pptformatlib_logic.cpp
}

core_debug {
SOURCES += \
    ../Enums/RecordType.cpp \
    ../PPTFormatLib.cpp \
    ../Reader/ReadStructures.cpp \
    ../Reader/RoundTripExtractor.cpp \
    ../Reader/PPTDocumentInfoOneUser.cpp \
    ../Reader/Records.cpp \
    ../Reader/PPTFileReader.cpp \
    ../Reader/SlidePersist.cpp \
    ../PPTXWriter/Converter.cpp \
    ../PPTXWriter/ShapeWriter.cpp \
    ../PPTXWriter/Animation.cpp \
    ../PPTXWriter/TableWriter.cpp \
    ../PPTXWriter/TxBodyConverter.cpp \
    ../Records/Drawing/ArtBlip.cpp \
    ../Records/Drawing/ShapeContainer.cpp \
    ../Records/Animations/TimeVariant.cpp \
    ../Records/BlipEntityAtom.cpp
}

SOURCES += \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Elements.cpp \
    ../../../ASCOfficePPTXFile/Editor/Drawing/TextAttributesEx.cpp \
    ../../../Common/3dParty/pole/pole.cpp \
    ../PPTXWriter/BulletsConverter.cpp

