#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = PptFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../../Common/base.pri)

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

DEFINES +=  UNICODE \
        _UNICODE \
        _PRESENTATION_WRITER_ \
        _SVG_CONVERT_TO_IMAGE_  \
        DONT_WRITE_EMBEDDED_FONTS
        #DISABLE_FILE_DOWNLOADER

HEADERS += \
	../../../PptFile/Main/PPTFormatLib.h \
	../../../PptFile/Converter/Animation/AnimationParser.h \
	../../../PptFile/Converter/Animation/Animation_1995.h \
	../../../PptFile/Converter/Animation/TimingExeption.h \
	../../../PptFile/Converter/Animation/TimingUtils.h \
	../../../PptFile/Converter/Animation/Timing_1995.h \
	../../../PptFile/Converter/Animation/Timing_2010.h \
	../../../PptFile/Converter/Animation/hashcode10.h \
	../../../PptFile/Converter/Animation/intermediate_anim.h \
	../../../PptFile/Converter/timing.h \
	../../../PptFile/Converter/transition.h \
	../../../PptFile/Enums/RecordType.h \
	../../../PptFile/Enums/_includer.h \
	../../../PptFile/Enums/enums.h \
	../../../PptFile/PPTFormatLib.h \
	../../../PptFile/PPTXWriter/BulletsConverter.h \
	../../../PptFile/PPTXWriter/TableWriter.h \
	../../../PptFile/PPTXWriter/TxBodyConverter.h \
	../../../PptFile/Reader/ClassesAtom.h \
	../../../PptFile/Reader/CommonZLib.h \
        ../../../PptFile/Reader/ExtXmlUtils.h \
	../../../PptFile/Reader/PPTDocumentInfo.h \
	../../../PptFile/Reader/PPTDocumentInfoOneUser.h \
	../../../PptFile/Reader/PPTFileDefines.h \
	../../../PptFile/Reader/PPTFileReader.h \
	../../../PptFile/Reader/ReadStructures.h \
	../../../PptFile/Reader/Records.h \
	../../../PptFile/Reader/RoundTripExtractor.h \
	../../../PptFile/Reader/Slide.h \
	../../../PptFile/Reader/SlideInfo.h \
	../../../PptFile/Reader/SlidePersist.h \
	../../../PptFile/Records/Animations/AnimationInfoAtom.h \
	../../../PptFile/Records/Animations/AnimationInfoContainer.h \
	../../../PptFile/Records/Animations/BuildAtom.h \
	../../../PptFile/Records/Animations/BuildListContainer.h \
	../../../PptFile/Records/Animations/BuildListSubContainer.h \
	../../../PptFile/Records/Animations/ChartBuildAtom.h \
	../../../PptFile/Records/Animations/ChartBuildContainer.h \
	../../../PptFile/Records/Animations/ClientVisualElementContainer.h \
	../../../PptFile/Records/Animations/DiagramBuildAtom.h \
	../../../PptFile/Records/Animations/DiagramBuildContainer.h \
	../../../PptFile/Records/Animations/ExtTimeNodeContainer.h \
	../../../PptFile/Records/Animations/HashCode10Atom.h \
	../../../PptFile/Records/Animations/LevelInfoAtom.h \
	../../../PptFile/Records/Animations/ParaBuildAtom.h \
	../../../PptFile/Records/Animations/ParaBuildContainer.h \
	../../../PptFile/Records/Animations/ParaBuildLevel.h \
	../../../PptFile/Records/Animations/SubEffectContainer.h \
	../../../PptFile/Records/Animations/TimeAnimateBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeAnimateBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeAnimateColor.h \
	../../../PptFile/Records/Animations/TimeAnimateColorBy.h \
	../../../PptFile/Records/Animations/TimeAnimationValueAtom.h \
	../../../PptFile/Records/Animations/TimeAnimationValueListContainer.h \
	../../../PptFile/Records/Animations/TimeAnimationValueListEntry.h \
	../../../PptFile/Records/Animations/TimeBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeColorBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeColorBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeCommandBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeCommandBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeConditionAtom.h \
	../../../PptFile/Records/Animations/TimeConditionContainer.h \
	../../../PptFile/Records/Animations/TimeDisplayType.h \
	../../../PptFile/Records/Animations/TimeEffectBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeEffectBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeEffectID.h \
	../../../PptFile/Records/Animations/TimeEffectNodeType.h \
	../../../PptFile/Records/Animations/TimeEffectType.h \
	../../../PptFile/Records/Animations/TimeGroupID.h \
	../../../PptFile/Records/Animations/TimeIterateDataAtom.h \
	../../../PptFile/Records/Animations/TimeMasterRelType.h \
	../../../PptFile/Records/Animations/TimeModifierAtom.h \
	../../../PptFile/Records/Animations/TimeMotionBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeMotionBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeNodeAtom.h \
	../../../PptFile/Records/Animations/TimeNodeTimeFilter.h \
	../../../PptFile/Records/Animations/TimePropertyList4TimeBehavior.h \
	../../../PptFile/Records/Animations/TimePropertyList4TimeNodeContainer.h \
	../../../PptFile/Records/Animations/TimeRotationBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeRotationBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeScaleBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeScaleBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeSequenceDataAtom.h \
	../../../PptFile/Records/Animations/TimeSetBehaviorAtom.h \
	../../../PptFile/Records/Animations/TimeSetBehaviorContainer.h \
	../../../PptFile/Records/Animations/TimeStringListContainer.h \
	../../../PptFile/Records/Animations/TimeVariant.h \
	../../../PptFile/Records/Animations/TimeVariant4Behavior.h \
	../../../PptFile/Records/Animations/VisualPageAtom.h \
	../../../PptFile/Records/Animations/VisualShapeAtom.h \
	../../../PptFile/Records/Animations/VisualShapeChartElementAtom.h \
	../../../PptFile/Records/Animations/VisualShapeGeneralAtom.h \
	../../../PptFile/Records/Animations/VisualSoundAtom.h \
	../../../PptFile/Records/Animations/_includer.h \
	../../../PptFile/Records/BlipCollection9Container.h \
	../../../PptFile/Records/BlipEntityAtom.h \
	../../../PptFile/Records/BookmarkEntityAtom.h \
	../../../PptFile/Records/BookmarkSeedAtom.h \
	../../../PptFile/Records/BuildAtom.h \
	../../../PptFile/Records/DocInfoListContainer.h \
	../../../PptFile/Records/DocProgTagsContainer.h \
	../../../PptFile/Records/MouseInteractiveInfoContainer.h \
	../../../PptFile/Records/OfficeArtClientData.h \
	../../../PptFile/Records/OfficeArtClientTextbox.h \
	../../../PptFile/Records/PlaceHolderAtom.h \
	../../../PptFile/Records/RoundTrip.h \
	../../../PptFile/Records/ShapeProgBinaryTagSubContainerOrAtom.h \
	../../../PptFile/Records/CFMasks.h \
	../../../PptFile/Records/ColorSchemeAtom.h \
	../../../PptFile/Records/CString.h \
	../../../PptFile/Records/CurrentUserAtom.h \
	../../../PptFile/Records/DocRoutingSlipAtom.h \
	../../../PptFile/Records/DocumentAtom.h \
	../../../PptFile/Records/DocumentRecords.h \
	../../../PptFile/Records/DocumentTextInfo.h \
	../../../PptFile/Records/EndDocument.h \
	../../../PptFile/Records/ExCDAudioContainer.h \
	../../../PptFile/Records/ExControlAtom.h \
	../../../PptFile/Records/ExHyperlinkAtom.h \
	../../../PptFile/Records/ExMediaAtom.h \
	../../../PptFile/Records/ExMIDIAudioContainer.h \
	../../../PptFile/Records/ExObjListAtom.h \
	../../../PptFile/Records/ExObjListContainer.h \
	../../../PptFile/Records/ExObjRefAtom.h \
	../../../PptFile/Records/ExOleEmbedAtom.h \
	../../../PptFile/Records/ExOleLinkAtom.h \
	../../../PptFile/Records/ExOleObjAtom.h \
	../../../PptFile/Records/ExVideoContainer.h \
	../../../PptFile/Records/ExWAVAudioEmbeddedContainer.h \
	../../../PptFile/Records/ExWAVAudioLinkContainer.h \
	../../../PptFile/Records/FontEntityAtom.h \
	../../../PptFile/Records/GridSpacing10Atom.h \
	../../../PptFile/Records/GuideAtom.h \
	../../../PptFile/Records/HeadersFootersAtom.h \
	../../../PptFile/Records/InteractiveInfoAtom.h \
	../../../PptFile/Records/KinsokuAtom.h \
	../../../PptFile/Records/KinsokuContainer.h \
	../../../PptFile/Records/KinsokuFollowingAtom.h \
	../../../PptFile/Records/KinsokuLeadingAtom.h \
	../../../PptFile/Records/MasterPersistAtom.h \
	../../../PptFile/Records/MasterTextPropAtom.h \
	../../../PptFile/Records/MetafileBlob.h \
	../../../PptFile/Records/MouseTextInteractiveInfoAtom.h \
	../../../PptFile/Records/NamedShowSlidesAtom.h \
	../../../PptFile/Records/NormalViewSetInfoAtom.h \
	../../../PptFile/Records/NotesAtom.h \
	../../../PptFile/Records/NotesPersistAtom.h \
	../../../PptFile/Records/NoZoomViewInfoAtom.h \
	../../../PptFile/Records/OutlineTextRefAtom.h \
	../../../PptFile/Records/FontCollection10Container.h \
	../../../PptFile/Records/FontEmbedFlags10Atom.h \
	../../../PptFile/Records/OutlineTextProps9Container.h \
	../../../PptFile/Records/OutlineTextPropsHeaderExAtom.h \
	../../../PptFile/Records/PFMasks.h \
	../../../PptFile/Records/PersistDirectoryAtom.h \
	../../../PptFile/Records/PlaceHolderAtom.h \
	../../../PptFile/Records/ProgStringTagContainer.h \
	../../../PptFile/Records/RecordsIncluder.h \
	../../../PptFile/Records/ShapeFlags10Atom.h \
	../../../PptFile/Records/ShapeFlagsAtom.h \
	../../../PptFile/Records/Comment10Container.h \
	../../../PptFile/Records/LinkedShape10Atom.h \
	../../../PptFile/Records/LinkedSlide10Atom.h \
	../../../PptFile/Records/SlideFlags10Atom.h \
	../../../PptFile/Records/SlideProgTagsContainer.h \
	../../../PptFile/Records/SlideTime10Atom.h \
	../../../PptFile/Records/SlideAtom.h \
	../../../PptFile/Records/SlideColorSchemeAtom.h \
	../../../PptFile/Records/SlideContainer.h \
	../../../PptFile/Records/SlideListWithText.h \
	../../../PptFile/Records/SlidePersistAtom.h \
	../../../PptFile/Records/SlideProgTagsContainer.h \
	../../../PptFile/Records/SlideViewInfoAtom.h \
	../../../PptFile/Records/SoundCollAtom.h \
	../../../PptFile/Records/SoundCollectionContainer.h \
	../../../PptFile/Records/SoundContainer.h \
	../../../PptFile/Records/SoundDataBlob.h \
	../../../PptFile/Records/SSDocInfoAtom.h \
	../../../PptFile/Records/SSlideLayoutAtom.h \
	../../../PptFile/Records/SSSlideInfoAtom.h \
	../../../PptFile/Records/StyleTextPropAtom.h \
	../../../PptFile/Records/StyleTextProp10Atom.h \
	../../../PptFile/Records/StyleTextProp11Atom.h \
	../../../PptFile/Records/StyleTextProp9Atom.h \
	../../../PptFile/Records/TextCFException10.h \
	../../../PptFile/Records/TextCFException9.h \
	../../../PptFile/Records/TextDefaults9Atom.h \
	../../../PptFile/Records/TextMasterStyle9Atom.h \
	../../../PptFile/Records/TextMasterStyle9Level.h \
	../../../PptFile/Records/TextSIException.h \
	../../../PptFile/Records/TextBookmarkAtom.h \
	../../../PptFile/Records/TextBytesAtom.h \
	../../../PptFile/Records/TextCFExceptionAtom.h \
	../../../PptFile/Records/TextCharsAtom.h \
	../../../PptFile/Records/TextFullSettings.h \
	../../../PptFile/Records/TextHeaderAtom.h \
	../../../PptFile/Records/TextInteractiveInfoAtom.h \
	../../../PptFile/Records/TextMasterStyle9Atom.h \
	../../../PptFile/Records/TextMasterStyle9Level.h \
	../../../PptFile/Records/TextMasterStyleAtom.h \
	../../../PptFile/Records/TextPFException9.h \
	../../../PptFile/Records/TextPFExceptionAtom.h \
	../../../PptFile/Records/TextRulerAtom.h \
	../../../PptFile/Records/TextSIExceptionAtom.h \
	../../../PptFile/Records/TextSpecInfoAtom.h \
	../../../PptFile/Records/UserEditAtom.h \
	../../../PptFile/Records/VBAInfoAtom.h \
	../../../PptFile/Records/ViewInfoAtom.h \
	../../../PptFile/Records/ZoomViewInfoAtom.h \
	../../../PptFile/Records/Animations/AnimationTypes.h \
	../../../PptFile/Records/Drawing/BlipStoreContainer.h \
	../../../PptFile/Records/Drawing/BlipStoreEntry.h \
	../../../PptFile/Records/Drawing/ChildAnchor.h \
	../../../PptFile/Records/Drawing/ClientAnchor.h \
	../../../PptFile/Records/Drawing/DrawingContainer.h \
	../../../PptFile/Records/Drawing/DrawingGroup.h \
	../../../PptFile/Records/Drawing/DrawingRecord.h \
	../../../PptFile/Records/Drawing/GroupShape.h \
	../../../PptFile/Records/Drawing/GroupShapeContainer.h \
	../../../PptFile/Records/Drawing/RegGroupItems.h \
	../../../PptFile/Records/Drawing/Shape.h \
	../../../PptFile/Records/Drawing/ShapeContainer.h \
	../../../PptFile/Records/Drawing/ShapeProperties.h \
	../../../PptFile/Records/Drawing/TextBox.h \
	../../../PptFile/Records/Drawing/ArtBlip.h \
	../../../PptFile/PPTXWriter/Converter.h \
	../../../PptFile/PPTXWriter/ImageManager.h \
	../../../PptFile/PPTXWriter/ShapeWriter.h \
	../../../PptFile/PPTXWriter/StylesWriter.h \
	../../../PptFile/Drawing/Element.h \
	../../../PptFile/Drawing/Elements.h \
	../../../PptFile/Drawing/Layout.h \
	../../../PptFile/Drawing/Slide.h \
	../../../PptFile/Drawing/TextAttributesEx.h \
	../../../PptFile/Drawing/TextStructures.h \
	../../../PptFile/Drawing/Theme.h \
	../../../PptFile/Structures/ColorIndex.h \
	../../../PptFile/Structures/ColorIndexStruct.h \
	../../../PptFile/Structures/ColorStruct.h \
	../../../PptFile/Structures/DateTimeStruct.h \
	../../../PptFile/Structures/IStruct.h \
	../../../PptFile/Structures/PointStruct.h \
	../../../PptFile/Structures/RatioStruct.h \
	../../../PptFile/Structures/RectStruct.h \
	../../../PptFile/Structures/ScalingStruct.h \
	../../../PptFile/Structures/SmallRectStruct.h \
	../../../PptFile/Structures/TmsfTimeStruct.h \
	../../../PptFile/Structures/WideColorStruct.h \
	../../../PptFile/Structures/_includer.h

core_release {
SOURCES += \
	ppt_format_logic.cpp
}

core_debug {
SOURCES += \
	../../../PptFile/Enums/RecordType.cpp \
	../../../PptFile/Enums/RecordType.cpp \
	../../../PptFile/Reader/ReadStructures.cpp \
	../../../PptFile/Reader/RoundTripExtractor.cpp \
	../../../PptFile/Reader/PPTDocumentInfoOneUser.cpp \
	../../../PptFile/Reader/Records.cpp \
	../../../PptFile/Reader/PPTFileReader.cpp \
	../../../PptFile/Reader/SlidePersist.cpp \
	../../../PptFile/PPTXWriter/Converter.cpp \
	../../../PptFile/PPTXWriter/ShapeWriter.cpp \
	../../../PptFile/PPTXWriter/TableWriter.cpp \
	../../../PptFile/PPTXWriter/TxBodyConverter.cpp \
	../../../PptFile/Records/Drawing/ArtBlip.cpp \
	../../../PptFile/Records/Drawing/ShapeContainer.cpp \
	../../../PptFile/Records/Animations/TimeVariant.cpp \
	../../../PptFile/Records/BlipEntityAtom.cpp \
	../../../PptFile/Drawing/Elements.cpp \
	../../../PptFile/Drawing/TextAttributesEx.cpp \
  	../../../PptFile/Converter/Animation/AnimationParser.cpp \
	../../../PptFile/Converter/Animation/Animation_1995.cpp \
	../../../PptFile/Converter/Animation/TimingUtils.cpp \
	../../../PptFile/Converter/Animation/Timing_1995.cpp \
	../../../PptFile/Converter/Animation/Timing_2010.cpp \
	../../../PptFile/Converter/Animation/hashcode10.cpp \
	../../../PptFile/Converter/Animation/intermediate_anim.cpp \
	../../../PptFile/Converter/timing.cpp \
	../../../PptFile/Converter/transition.cpp \
	../../../PptFile/PPTXWriter/BulletsConverter.cpp
}

SOURCES += \
	../../../PptFile/Drawing/Element.cpp \
	../../../PptFile/Drawing/Slide.cpp \
	../../../PptFile/Drawing/TextStructures.cpp \
	../../../PptFile/Drawing/Theme.cpp \
	../../../PptFile/Main/PPTFormatLib.cpp \
	../../../../Common/3dParty/pole/pole.cpp \
	../../../PptFile/PPTXWriter/ImageManager.cpp \
	../../../PptFile/Reader/ClassesAtom.cpp \
	../../../PptFile/Reader/ExtXmlUtils.cpp \
	../../../PptFile/Reader/PPTDocumentInfo.cpp \
	../../../PptFile/Reader/SlideInfo.cpp \
	../../../PptFile/Records/TextFullSettings.cpp
