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
#    ../../../ASCOfficePPTXFile/Editor/Drawing/Animations.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/Attributes.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/AudioOverlay.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/Document.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/Effects.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/Interactive.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/Metric.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/SlideShow.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/Structures.h \
#    ../../../ASCOfficePPTXFile/Editor/Drawing/XmlStringWriter.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/AlbumLayout.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/AnimationDgmBuild.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/BWMode.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/BaseLimit.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/BevelType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/BlendMode.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/BlipCompression.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/CameraType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/ChartBuild.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/ChartBuildType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/ColorSchemeIndex.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/CompoundLine.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/Conformance.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/ContentStatus.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/CornerDirectionVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/DgmBuild.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/DgmBuildType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/EffectContainerType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/EightDirectionVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/FillPath.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/Flip.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/FontAlign.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/FontStyleIndex.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/FrameShape.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/HorzOverflow.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/InOutDirectionVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/IterateType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/LastView.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/LightRigType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/LineCap.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/LineEndSize.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/LineEndType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/Material.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/OnOff.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/Orient.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/ParaBuildType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PathFillMode.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PattFillVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PenAlign.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PlaceholderSize.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PlaceholderType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PresetShadowVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PrstClrVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/PrstDashVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/RectAlign.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/SchemeClrVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/ShapeType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/SideDirectionVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/SlideLayoutType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/SlideSize.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/SplitterBarState.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/SysClrVal.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLAccumulate.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLAdditive.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLCalcMode.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLChartSubElement.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLColorDirection.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLColorSpace.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLCommandType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLMasterRelation.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLNextAc.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLNodeFillType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLNodeType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLOrigin.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLOverride.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLPathEditMode.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLPresetClass.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLPrevAc.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLRestart.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLRuntimeTrigger.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLSyncBehavior.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLTransform.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLTriggerEvent.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLValueType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextAlign.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextAnchor.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextAutonumberScheme.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextCaps.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextShapeType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextStrike.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextTabAlignType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextUnderline.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextVerticalType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TextWrap.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/TransitionSpeed.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/VariantType.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Limit/VertOverflow.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Ah.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/AhBase.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/AhPolar.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/AhXY.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Backdrop.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bevel.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bg.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/BgPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/BodyPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuAutoNum.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuBlip.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuChar.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuClr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuClrTx.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuFontTx.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuNone.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuSzPct.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuSzPts.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BuSzTx.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/Bullet.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BulletColor.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BulletSize.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bullets/BulletTypeface.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CNvCxnSpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CNvGraphicFramePr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CNvGrpSpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CNvPicPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CNvPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CNvSpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CSld.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Camera.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Cell3D.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/ClrMap.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/ClrMapOvr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/ColorBase.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/ColorModifier.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/PrstClr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SrgbClr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SysClr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/ContentPart.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Controls.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CustGeom.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Cxn.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CxnSp.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/DefaultShapeDefinition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectDag.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectProperties.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaBiLevel.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaCeiling.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaFloor.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaInv.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaMod.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaModFix.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaOutset.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaRepl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/BiLevel.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Blend.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Blur.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/ClrChange.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/ClrRepl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Duotone.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/EffectElement.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/FillEffect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/FillOverlay.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Glow.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Grayscl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/HslEffect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/InnerShdw.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/LumEffect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/OuterShdw.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/PrstShdw.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Reflection.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/RelOff.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/SoftEdge.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/TintEffect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/XfrmEffect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/ExtP.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/FillStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/Blip.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/BlipFill.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/GradFill.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/NoFill.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/PattFill.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/SolidFill.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/Stretch.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/Tile.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/FontCollection.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/FontRef.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Gd.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Geometry.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/GraphicFrame.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/GrpSpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Gs.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/HeadingVariant.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Hf.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Hyperlink.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/LightRig.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Lin.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/LineEnd.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/LineJoin.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/LineStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Ln.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Media/AudioCD.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Media/MediaFile.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Media/WavAudioFile.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvCxnSpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvGraphicFramePr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvGrpSpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvPicPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvSpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Paragraph.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/PartTitle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D/ArcTo.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D/Close.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D/CubicBezTo.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D/LineTo.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D/MoveTo.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D/PathBase.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Path2D/QuadBezTo.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Ph.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Pic.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/PrstDash.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/PrstGeom.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/PrstTxWarp.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Rect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Rot.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/RunElem.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/RunProperties.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Runs/Br.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Runs/Fld.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Runs/MathParaWrapper.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Runs/Run.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Runs/RunBase.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Scene3d.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/ShapeStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SmartArt.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Sp3d.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTree.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTreeElem.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/StyleRef.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SupplementalFont.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Tab.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/Table.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/TableCell.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/TableCellProperties.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/TableCol.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/TableProperties.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/TableRow.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TableBgStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TablePartStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TableStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TcBdr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TcStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TcTxStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TextFit.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TextFont.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TextListStyle.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TextParagraphPr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TextSpacing.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Anim.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimClr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimEffect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimMotion.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimRot.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimScale.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimVariant.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AttrName.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AttrNameLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Audio.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldDgm.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldGraphic.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldOleChart.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldSub.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BuildNodeBase.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CBhvr.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CMediaNode.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CTn.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/ChildTnLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Cmd.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Cond.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CondLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Excl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/GraphicEl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Iterate.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Set.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/SpTgt.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Tav.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TavLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TgtEl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TimeNodeBase.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Tmpl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TmplLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TnLst.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TxEl.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Video.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/CornerDirectionTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/EightDirectionTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/EmptyTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/OptionalBlackTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/OrientationTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/SideDirectionTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/SndAc.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/SplitTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/StSnd.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/Transition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/TransitionBase.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/WheelTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/ZoomTransition.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TxBody.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TxStyles.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniColor.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniEffect.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniFill.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniMedia.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniPath2D.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Xfrm.h \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/XmlId.h \
    ../Enums/RecordType.h \
    ../Enums/_includer.h \
    ../Enums/enums.h \
    ../PPTFormatLib.h \
    ../PPTXWriter/Animation.h \
    ../PPTXWriter/ridmanager.h \
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
    ../Records/BlipEntityAtom.h \
    ../Records/BookmarkEntityAtom.h \
    ../Records/BookmarkSeedAtom.h \
    ../Records/BuildAtom.h \
    ../Records/DocInfoListContainer.h \
    ../Records/DocProgTagsContainer.h \
    ../Records/OfficeArtClientData.h \
    ../Records/PlaceHolderAtom.h \
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
    ../Records/RoundTripColorMappingAtom.h \
    ../Records/RoundTripCompositeMainMasterId12Atom.h \
    ../Records/RoundTripNewPlaceholderId12Atom.h \
    ../Records/RoundTripOriginalMainMasterId12Atom.h \
    ../Records/RoundTripThemeAtom.h \
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
    ../Reader/ReadStructures.cpp \
    ../Reader/PPTDocumentInfoOneUser.cpp \
    ../Reader/Records.cpp \
    ../Reader/PPTFileReader.cpp \
    ../Reader/SlidePersist.cpp \
    ../Records/Drawing/ArtBlip.cpp \
    ../Records/Drawing/ShapeContainer.cpp \
    ../PPTXWriter/Converter.cpp \
    ../PPTXWriter/ShapeWriter.cpp
}

SOURCES += \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Bg.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CNvGrpSpPr.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/ContentPart.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Controls.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/CxnSp.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectDag.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectLst.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectProperties.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/EffectStyle.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/Blip.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Fills/BlipFill.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Geometry.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/GraphicFrame.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/GrpSpPr.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Hyperlink.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Media/WavAudioFile.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvGraphicFramePr.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/NvGrpSpPr.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Pic.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Runs/MathParaWrapper.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SmartArt.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpPr.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTree.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTreeElem.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/TableCell.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BuildNodeBase.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TimeNodeBase.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/Transitions/TransitionBase.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/TxBody.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniColor.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniEffect.cpp \
#    ../../../ASCOfficePPTXFile/PPTXFormat/Logic/UniFill.cpp \
    ../Enums/RecordType.cpp \
    ../PPTFormatLib.cpp \
    ../../../ASCOfficePPTXFile/Editor/Drawing/Elements.cpp \
    ../../../ASCOfficePPTXFile/Editor/Drawing/TextAttributesEx.cpp \
    ../../../Common/3dParty/pole/pole.cpp \
    ../PPTXWriter/Animation.cpp \
    ../PPTXWriter/ridmanager.cpp \
    ../Records/Animations/TimeVariant.cpp

