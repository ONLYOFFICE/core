#-------------------------------------------------
#
# Project created by QtCreator 2014-10-03T18:30:31
#
#-------------------------------------------------

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

# before include(base.pri) !!!
ios:CONFIG += build_x2t_as_library
android:CONFIG += build_x2t_as_library
build_x2t_as_library {
    TEMPLATE = lib
    CONFIG -= console

    DEFINES += BUILD_X2T_AS_LIBRARY_DYLIB

    CONFIG += shared
    CONFIG += plugin

    CONFIG += core_static_link_libstd
}

include(X2tConverter.pri)

build_x2t_as_library {
!core_debug:shared:QMAKE_LFLAGS += -exported_symbols_list $$PWD/../../src/dylib/export
}


!build_x2t_as_library {
    SOURCES += ../../src/main.cpp

    DESTDIR = $$CORE_BUILDS_BINARY_PATH
} else {
    HEADERS += ../../src/dylib/x2t.h
    SOURCES += ../../src/dylib/x2t.cpp
}

SOURCES += \
    ../../../MsBinaryFile/PptFile/Records/Animations/AnimationInfoAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/AnimationInfoContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/BuildAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/BuildListSubContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/ChartBuildAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/ChartBuildContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/ClientVisualElementContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/DiagramBuildAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/DiagramBuildContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/ExtTimeNodeContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/HashCode10Atom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/LevelInfoAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/ParaBuildAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/ParaBuildContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/ParaBuildLevel.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/SubEffectContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeAnimateBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeAnimateBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeAnimateColor.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeAnimateColorBy.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeAnimationValueAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeAnimationValueListContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeAnimationValueListEntry.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeColorBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeColorBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeCommandBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeCommandBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeConditionAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeConditionContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeEffectBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeEffectBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeIterateDataAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeModifierAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeMotionBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeMotionBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeNodeAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimePropertyList4TimeBehavior.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimePropertyList4TimeNodeContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeRotationBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeRotationBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeScaleBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeScaleBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeSequenceDataAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeSetBehaviorAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeSetBehaviorContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeStringListContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/TimeVariant4Behavior.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/VisualPageAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/VisualShapeAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/VisualShapeChartElementAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/VisualShapeGeneralAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Animations/VisualSoundAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/BlipCollection9Container.cpp \
    ../../../MsBinaryFile/PptFile/Records/BookmarkEntityAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/BookmarkSeedAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/BuildAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/CFMasks.cpp \
    ../../../MsBinaryFile/PptFile/Records/CString.cpp \
    ../../../MsBinaryFile/PptFile/Records/ColorSchemeAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/Comment10Container.cpp \
    ../../../MsBinaryFile/PptFile/Records/CurrentUserAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/DocInfoListContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/DocProgTagsContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/DocRoutingSlipAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/DocumentAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/DocumentRecords.cpp \
    ../../../MsBinaryFile/PptFile/Records/DocumentTextInfo.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/BlipStoreContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/BlipStoreEntry.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/ChildAnchor.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/ClientAnchor.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/DrawingContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/DrawingGroup.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/DrawingRecord.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/GroupShape.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/Shape.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/ShapeProperties.cpp \
    ../../../MsBinaryFile/PptFile/Records/Drawing/TextBox.cpp \
    ../../../MsBinaryFile/PptFile/Records/EndDocument.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExCDAudioContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExControlAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExHyperlinkAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExMIDIAudioContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExMediaAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExObjListAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExObjListContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExObjRefAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExOleEmbedAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExOleLinkAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExOleObjAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExVideoContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExWAVAudioEmbeddedContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/ExWAVAudioLinkContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/FontCollection10Container.cpp \
    ../../../MsBinaryFile/PptFile/Records/FontEmbedFlags10Atom.cpp \
    ../../../MsBinaryFile/PptFile/Records/FontEntityAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/GridSpacing10Atom.cpp \
    ../../../MsBinaryFile/PptFile/Records/GuideAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/HeadersFootersAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/InteractiveInfoAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/KinsokuAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/KinsokuContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/KinsokuFollowingAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/KinsokuLeadingAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/LinkedShape10Atom.cpp \
    ../../../MsBinaryFile/PptFile/Records/LinkedSlide10Atom.cpp \
    ../../../MsBinaryFile/PptFile/Records/MasterPersistAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/MasterTextPropAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/MetafileBlob.cpp \
    ../../../MsBinaryFile/PptFile/Records/MouseInteractiveInfoContainer.cpp \
    ../../../MsBinaryFile/PptFile/Records/MouseTextInteractiveInfoAtom.cpp \
    ../../../MsBinaryFile/PptFile/Records/NamedShowSlidesAtom.cpp
