/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/BaseObject.h"
#include "../Base/SmartPtr.h"

namespace NSBinPptxRW
{
	class CBinaryFileWriter;
	class CBinaryFileReader;
	class CXmlWriter;
}
namespace OOX
{
#define WritingElement_AdditionConstructors(Class) \
	explicit Class(XmlUtils::CXmlNode& oNode)\
	{\
	m_pMainDocument = NULL;\
	fromXML( oNode );\
}\
	explicit Class(const XmlUtils::CXmlNode& node)\
	{\
	m_pMainDocument = NULL;\
	fromXML(const_cast<XmlUtils::CXmlNode&> (node));\
}\
	Class(XmlUtils::CXmlLiteReader& oReader)\
	{\
	m_pMainDocument = NULL;\
	fromXML( oReader );\
}\
	const Class& operator =(const XmlUtils::CXmlNode &oNode)\
	{\
	m_pMainDocument = NULL;\
	fromXML( (XmlUtils::CXmlNode &)oNode );\
	return *this;\
}\
	const Class& operator =(const XmlUtils::CXmlLiteReader& oReader)\
	{\
	m_pMainDocument = NULL;\
	fromXML( (XmlUtils::CXmlLiteReader&)oReader );\
	return *this;\
}\
	const Class& operator =(XmlUtils::CXmlNode& node)				\
	{																\
	m_pMainDocument = NULL;\
	fromXML(node);												\
	return *this;												\
}																\

#define WritingElement_XlsbConstructors(Class) \
	explicit Class(XLS::BaseObjectPtr& obj)\
	{\
	m_pMainDocument = NULL;\
	fromBin(obj);\
}\
	const Class& operator =(XLS::BaseObjectPtr& obj)\
	{\
	m_pMainDocument = NULL;\
	fromBin(obj);\
	return *this;\
}\

#define WritingElement_XlsbVectorConstructors(Class) \
	explicit Class(std::vector<XLS::BaseObjectPtr>& obj)\
	{\
	m_pMainDocument = NULL;\
	fromBin(obj);\
}\
	const Class& operator =(std::vector<XLS::BaseObjectPtr>& obj)\
	{\
	m_pMainDocument = NULL;\
	fromBin(obj);\
	return *this;\
}\

#define WritingElement_ReadNode( oRootNode, oChildNode, sNodeName, oValue ) \
	if ( oRootNode.GetNode( sNodeName, oChildNode ) )\
	oValue = oChildNode;

#define WritingElement_WriteNode_1( sStartNodeString, oValue ) \
	if ( oValue.IsInit() )\
	{\
	sResult += sStartNodeString;\
	sResult += oValue->ToString();\
	sResult += _T("/>");\
}

#define WritingElement_WriteNode_2( oValue ) \
	if ( oValue.IsInit() )\
	sResult += oValue->toXML();
	//-----------------------------------------------------------------------------------------------
#define WritingElement_ReadAttributes_ReadSingle2(Reader, AttrName, Value) \
	if ( Reader.GetAttributesCount() > 0 ){\
	if ( Reader.MoveToFirstAttribute() ){\
	std::wstring wsName = Reader.GetName();\
	while( !wsName.empty() )\
	{\
	if ( AttrName == wsName )\
	{\
	Value = Reader.GetText();\
	break;\
}\
	if ( !Reader.MoveToNextAttribute() ) \
	break;\
	wsName = Reader.GetName();\
}\
	Reader.MoveToElement();}}
	// Следующие 3 define используются для чтения аттрибутов через CXmlLiteReader
#define WritingElement_ReadAttributes_Start(Reader) \
	if ( Reader.GetAttributesCount() <= 0 )\
	return;\
	if ( !Reader.MoveToFirstAttribute() )\
	return;\
	std::wstring wsName = Reader.GetName();\
	while( !wsName.empty() )\
	{

#define WritingElement_ReadAttributes_StartChar(Reader) \
	if ( Reader.GetAttributesCount() <= 0 )\
	return;\
	if ( !Reader.MoveToFirstAttribute() )\
	return;\
	const char* wsName = Reader.GetNameChar();\
	while( strlen(wsName) != 0 )\
	{

#define WritingElement_ReadAttributes_StartChar_No_NS(Reader) \
	if ( Reader.GetAttributesCount() <= 0 )\
	return;\
	if ( !Reader.MoveToFirstAttribute() )\
	return;\
	const char* wsName = XmlUtils::GetNameNoNS(Reader.GetNameChar());\
	while( strlen(wsName) != 0 )\
	{

#define WritingElement_ReadAttributes_Start_No_NS(Reader) \
	if ( Reader.GetAttributesCount() <= 0 )\
	return;\
	if ( !Reader.MoveToFirstAttribute() )\
	return;\
	std::wstring wsName = XmlUtils::GetNameNoNS(Reader.GetName());\
	while( !wsName.empty() )\
	{

#define WritingElement_ReadAttributes_Read_if(Reader, AttrName, Value) \
	if ( AttrName == wsName )\
	{\
	Value = Reader.GetText();\
}

#define WritingElement_ReadAttributes_Read_ifChar(Reader, AttrName, Value) \
	if ( strcmp(AttrName, wsName) == 0 )\
	{\
	Value = Reader.GetText();\
}

#define WritingElement_ReadAttributes_Read_else_if(Reader, AttrName, Value) \
	else if ( AttrName == wsName )\
	Value = Reader.GetText();

#define WritingElement_ReadAttributes_Read_else_ifChar(Reader, AttrName, Value) \
	else if ( strcmp(AttrName, wsName) == 0 )\
	Value = Reader.GetText();

#define WritingElement_ReadAttributes_ReadSingle(Reader, AttrName, Value) \
	if ( AttrName == wsName )\
	{\
	Value = Reader.GetText();\
	break;\
}

#define WritingElement_ReadAttributes_End(Reader) \
	if ( !Reader.MoveToNextAttribute() ) \
	break;\
	wsName = Reader.GetName();\
}\
	Reader.MoveToElement();

#define WritingElement_ReadAttributes_EndChar(Reader) \
	if ( !Reader.MoveToNextAttribute() ) \
	break;\
	wsName = Reader.GetNameChar();\
}\
	Reader.MoveToElement();

#define WritingElement_ReadAttributes_EndChar_No_NS(Reader) \
	if ( !Reader.MoveToNextAttribute() ) \
	break;\
	wsName = XmlUtils::GetNameNoNS(Reader.GetNameChar());\
}\
	Reader.MoveToElement();

#define WritingElement_ReadAttributes_End_No_NS(Reader) \
	if ( !Reader.MoveToNextAttribute() ) \
	break;\
	wsName = XmlUtils::GetNameNoNS(Reader.GetName());\
}\
	Reader.MoveToElement();

	enum EElementType
	{
		et_Unknown,

		et_Default, // <Default>
		et_Override, // <Override>
		et_Relationship, // <Relationship>

		et_a_accent1, // <a:accent1>
		et_a_accent2, // <a:accent2>
		et_a_accent3, // <a:accent3>
		et_a_accent4, // <a:accent4>
		et_a_accent5, // <a:accent5>
		et_a_accent6, // <a:accent6>
		et_a_ahLst, // <a:ahLst>
		et_a_ahPolar, // <a:ahPolar>
		et_a_ahXY, // <a:ahXY>
		et_a_alpha, // <a:alpha>
		et_a_alphaBiLevel, // <a:alphaBiLevel>
		et_a_alphaCeiling, // <a:alphaCeiling>
		et_a_alphaFloor, // <a:alphaFloor>
		et_a_alphaInv, // <a:alphaInv>
		et_a_alphaMod, // <a:alphaMod>
		et_a_alphaModFix, // <a:alphaModFix>
		et_a_alphaOff, // <a:alphaOff>
		et_a_alphaOutset, // <a:alphaOutset>
		et_a_alphaRepl, // <a:alphaRepl>
		et_a_anchor, // <a:anchor>
		et_a_arcTo, // <a:arcTo>
		et_a_avLst, // <a:avLst>
		et_a_backdrop, // <a:backdrop>
		et_a_bevel, // <a:bevel>
		et_a_bevelB, // <a:bevelB>
		et_a_bevelT, // <a:bevelT>
		et_a_bgClr, // <a:bgClr>
		et_a_bgFillStyleLst, // <a:bgFillStyleLst>
		et_a_biLevel, // <a:biLevel>
		et_a_blend, // <a:blend>
		et_a_blip, // <a:blip>
		et_a_blipFill, // <a:blipFill>
		et_a_blue, // <a:blue>
		et_a_blueMod, // <a:blueMod>
		et_a_blueOff, // <a:blueOff>
		et_a_blur, // <a:blur>
		et_a_bodyPr, // <a:bodyPr>
		et_a_camera, // <a:camera>
		et_a_close, // <a:close>
		et_a_clrChange, // <a:clrChange>
		et_a_clrFrom, // <a:clrFrom>
		et_a_clrMap, // <a:clrMap>
		et_a_clrMapOvr, // <a:clrMapOvr>
		et_a_clrRepl, // <a:clrRepl>
		et_a_clrScheme, // <a:clrScheme>
		et_a_clrTo, // <a:clrTo>
		et_a_cNvPicPr, // <a:cNvPicPr>
		et_a_comp, // <a:comp>
		et_a_cont, // <a:cont>
		et_a_contourClr, // <a:contourClr>
		et_a_cs, // <a:cs>
		et_a_cubicBezTo, // <a:cubicBezTo>
		et_a_custDash, // <a:custDash>
		et_a_custClr, // <a:custClr>
		et_a_custClrLst, // <a:custClrLst>
		et_a_custGeom, // <a:custGeom>
		et_a_cxn, // <a:cxn>
		et_a_cxnLst, // <a:cxnLst>
		et_a_dk1, // <a:dk1>
		et_a_dk2, // <a:dk2>
		et_a_ds, // <a:ds>
		et_a_duotone, // <a:duotone>
		et_a_ea, // <a:ea>
		et_a_effect, // <a:effect>
		et_a_effectDag, // <a:effectDag>
		et_a_effectLst, // <a:effectLst>
		et_a_effectRef, // <a:effectRef>
		et_a_effectStyle, // <a:effectStyle>
		et_a_effectStyleLst, // <a:effectStyleLst>
		et_a_compatExt,
		et_a_ext, // <a:ext>
		et_a_extLst, // <a:extLst>
		et_a_extraClrScheme, // <a:extraClrScheme>
		et_a_extraClrSchemeLst, // <a:extraClrSchemeLst>
		et_a_extrusionClr, // <a:extrusionClr>
		et_a_fgClr, // <a:fgClr>
		et_a_fill, // <a:fill>
		et_a_fillOverlay, // <a:fillOverlay>
		et_a_fillRect, // <a:fillRect>
		et_a_fillRef, // <a:fillRef>
		et_a_fillStyleLst, // <a:fillStyleLst>
		et_a_fillToRect, // <a:fillToRect>
		et_a_flatTx, // <a:flatTx>
		et_a_fmtScheme, // <a:fmtScheme>
		et_a_folHlink, // <a:folHlink>
		et_a_font, // <a:font>
		et_a_fontRef, // <a:fontRef>
		et_a_fontScheme, // <a:fontScheme>
		et_a_gamma, // <a:gamma>
		et_a_gd, // <a:gd>
		et_a_gdLst, // <a:gdLst>
		et_a_glow, // <a:glow>
		et_a_gradFill, // <a:gradFill>
		et_a_graphic, // <a:graphic>
		et_a_graphicFrame,
		et_a_graphicData,
		et_a_graphicFrameLocks, // <a:graphicFrameLocks>
		et_a_gray, // <a:gray>
		et_a_grayscl, // <a:grayscl>
		et_a_green, // <a:green>
		et_a_greenMod, // <a:greenMod>
		et_a_greenOff, // <a:greenOff>
		et_a_grpFill, // <a:grpFill>
		et_a_gs, // <a:gs>
		et_a_gsLst, // <a:gsLst>
		et_a_headEnd, // <a:headEnd>
		et_a_hlink, // <a:hlink>
		et_a_hlinkClick, // <a:hlinkClick>
		et_a_hlinkHover, // <a:hlinkHover>
		et_a_hsl, // <a:hsl>
		et_a_hslClr, // <a:hslClr>
		et_a_hue, // <a:hue>
		et_a_hueMod, // <a:hueMod>
		et_a_hueOff, // <a:hueOff>
		et_a_innerShdw, // <a:innerShdw>
		et_a_inv, // <a:inv>
		et_a_invGamma, // <a:invGamma>
		et_a_latin, // <a:latin>
		et_a_lightRig, // <a:lightRig>
		et_a_lin, // <a:lin>
		et_a_lineTo, // <a:lineTo>
		et_a_ln, // <a:ln>
		et_a_lnDef, // <a:lnDef>
		et_a_lnRef, // <a:lnRef>
		et_a_lnStyleLst, // <a:lnStyleLst>
		et_a_lnTo, // <a:lnTo>
		et_a_lt1, // <a:lt1>
		et_a_lt2, // <a:lt2>
		et_a_lum, // <a:lum>
		et_a_lumMod, // <a:lumMod>
		et_a_lumOff, // <a:lumOff>
		et_a_majorFont, // <a:majorFont>
		et_a_minorFont, // <a:minorFont>
		et_a_masterClrMapping, // <a:masterClrMapping>
		et_a_miter, // <a:miter>
		et_a_moveTo, // <a:moveTo>
		et_a_noAutofit, // <a:noAutofit>
		et_a_noFill, // <a:noFill>
		et_a_norm, // <a:norm>
		et_a_normAutofit, // <a:normAutofit>
		et_a_objectDefaults, // <a:objectDefaults>
		et_a_outerShdw, // <a:outerShdw>
		et_a_overrideClrMapping, // <a:overrideClrMapping>
		et_a_path, // <a:path>
		et_a_pathLst, // <a:pathLst>
		et_a_pattFill, // <a:pattFill>
		et_a_picLocks, // <a:picLocks>
		et_a_pos, // <a:pos>
		et_a_prstClr, // <a:prstClr>
		et_a_prstDash, // <a:prstDash>
		et_a_prstGeom, // <a:prstGeom>
		et_a_prstShdw, // <a:prstShdw>
		et_a_prstTxWarp, // <a:prstTxWarp>
		et_a_pt, // <a:pt>
		et_a_quadBezTo, // <a:quadBezTo>
		et_a_rect, // <a:rect>
		et_a_red, // <a:red>
		et_a_redMod, // <a:redMod>
		et_a_redOff, // <a:redOff>
		et_a_reflection, // <a:reflection>
		et_a_relOff, // <a:relOff>
		et_a_rot, // <a:rot>
		et_a_round, // <a:round>
		et_a_sat, // <a:sat>
		et_a_satMod, // <a:satMod>
		et_a_satOff, // <a:satOff>
		et_a_scene3d, // <a:scene3d>
		et_a_schemeClr, // <a:schemeClr>
		et_a_scrgbClr, // <a:scrgbClr>
		et_a_styleClr,
		et_a_shade, // <a:shade>
		et_a_snd, // <a:snd>
		et_a_softEdge, // <a:softEdge>
		et_a_solidFill, // <a:solidFill>
		et_a_sp3d, // <a:sp3d>
		et_a_spAutoFit, // <a:spAutoFit>
		et_a_spDef, // <a:spDef>
		et_a_spPr, // <a:spPr>
		et_a_srcRect, // <a:srcRect>
		et_a_srgbClr, // <a:srgbClr>
		et_a_stretch, // <a:stretch>
		et_a_style, // <a:style>
		et_a_sym, // <a:sym>
		et_a_sysClr, // <a:sysClr>
		et_a_tailEnd, // <a:tailEnd>
		et_a_themeElements, // <a:themeElements>
		et_a_tile, // <a:tile>
		et_a_tileRect, // <a:tileRect>
		et_a_tint, // <a:tint>
		et_a_txDef, // <a:txDef>
		et_a_up, // <a:up>
		et_a_xfrm, // <a:xfrm>
		et_a_uFillTx,
		et_a_highlight,

		et_a_groupSpPr,			// <a:groupSpPr>
		et_a_Shape,				// <a:sp>
		et_a_GroupShape,		// <a:srpSp>
		et_a_TextShape,			// <a:txSp>
		et_a_TextShapeBody,		// <a:txBody>
		et_a_LockedCanvas,		// <a:LockedCanvas>
		et_a_GroupShapeNonVisual,
		et_a_ConnectionNonVisualShapeProps,
		et_a_ShapeNonVisual,
		et_a_Slicer,
		
		et_dgm_DiagrammParts,	// <dgm:relIds>
		et_dgm_ptLst,			// <dgm:ptLst>
		et_dgm_pt,				// <dgm:pt>
		et_dgm_prSet,			// <dgm:prSet>
		et_dgm_spPr,			// <dgm:spPr>
		et_dgm_t,				// <dgm:t>
		et_dgm_cxnLst,
		et_dgm_cxn,
		et_dgm_VariableList,
		et_dgm_Shape,
		et_dgm_AdjLst,
		et_dgm_ShapeAdjust,
		et_dgm_PresOf,
		et_dgm_Choose,
		et_dgm_Else,
		et_dgm_If,
		et_dgm_Alg,
		et_dgm_Param,
		et_dgm_ConstrLst,
		et_dgm_Constraint,
		et_dgm_RuleLst,
		et_dgm_Rule,
		et_dgm_ForEach,
		et_dgm_LayoutNode,
		et_dgm_DiferentData,
		et_dgm_styleLbl,
		et_dgm_ClrLst,
		et_dgm_Whole,
		et_dgm_Bg,
		et_dgm_DataModel,
		et_dgm_Cat,
		et_dgm_CatLst,
		et_dgm_ResizeHandles,
		et_dgm_OrgChart,
		et_dgm_HierBranch,
		et_dgm_Direction,
		et_dgm_chPref,
		et_dgm_chMax,
		et_dgm_BulletEnabled,
		et_dgm_AnimOne,
		et_dgm_AnimLvl,
		et_dgm_ComplexType,
		et_dgm_text,
		et_dgm_ColorStyleLbl,

		et_dsp_Shape,			// <dsp:sp>
		et_dsp_ShapeTree,		// <dsp:spTree>
		et_dsp_spPr,			// <dsp:spPr>
		et_dsp_style,			// <dsp:style>
		et_dsp_groupSpPr,		// <dsp:grpSpPr>
		et_dsp_cNvPr,
		et_dsp_txXfrm,


		et_lc_LockedCanvas,	// <lc:lockedCanvas>

		et_graphicFrame,	// <...:graphicFrame>
		et_pic,				// <...:pic>
		et_cxnSp,			// <...:cxnSp>
		
		et_p_cNvPicPr,        // <p:cNvPicPr>
		et_p_cNvPr,            // <p:cNvPr>
		et_p_pic,            // <p:pic>
		et_p_Shape,            // <p:sp>
		et_p_ShapeTree,        // <p:spTree>
		et_p_spPr,            // <p:spPr>
		et_p_style,            // <p:style>
		et_p_groupSpPr,        // <p:grpSpPr>
		et_p_NvGrpSpPr,
		et_p_xfrm,
		et_p_r,
		et_p_fld,
		et_p_br,
		et_p_MathPara,
		
		et_p_EmptyTransition,
		et_p_OrientationTransition,
		et_p_EightDirectionTransition,
		et_p_OptionalBlackTransition,
		et_p_CornerDirectionTransition,
		et_p_SideDirectionTransition,
		et_p_WheelTransition,
		et_p_SplitTransition,
		et_p_ZoomTransition,

		et_p_par,
		et_p_seq,
		et_p_audio,
		et_p_video,
		et_p_excl,
		et_p_anim,
		et_p_animClr,
		et_p_animEffect,
		et_p_animMotion,
		et_p_animRot,
		et_p_animScale,
		et_p_cmd,
		et_p_set,

		et_p_bldP,
		et_p_bldDgm,
		et_p_bldGraphic,
		et_p_bldOleChart,

		et_a_textFit,
		et_a_hyperlink,
		et_a_fld,
		et_a_p,            // <a:p>
		et_a_pPr,        // <a:pPr>
		et_a_r,            // <a:p>
		et_a_rPr,        // <a:pPr>
		et_a_t,            // <a:t>
		et_a_br,        // <a:br>
		et_a_spcPts,    // <a:spcPts>
		et_a_spcPct,    // <a:spcPct>
		et_a_spcAft,    // <a:spcAft>
		et_a_spcBef,    // <a:spcBef>
		et_a_lnSpc,        // <a:lnSpc>
		et_a_tab,
		et_a_rtl,

		et_a_buNone,
		et_a_buChar,
		et_a_buAutoNum,
		et_a_buClr,
		et_a_buClrTx,
		et_a_buFontTx,
		et_a_buBlip,
		et_a_buSzPct,
		et_a_buSzPts,
		et_a_buSzTx,

		et_ds_customXmlProps,
		et_ds_schemaRefs, // <ds:shemeRefs>
		et_ds_schemaRef, // <ds:shemeRef>
		
		et_m_acc, //m:acc
		et_m_accPr, //m:accPr
		et_m_aln, //m:aln
		et_m_alnScr, //m:alnScr
		et_m_argPr, //m:argPr
		et_m_argSz, //m:argSz
		et_m_bar, //m:bar
		et_m_barPr, //m:barPr
		et_m_baseJc, //m:baseJc
		et_m_begChr, //m:begChr
		et_m_borderBox, //m:borderBox
		et_m_borderBoxPr, //m:borderBoxPr
		et_m_box, //m:box
		et_m_boxPr, //m:boxPr
		et_m_brk, //m:brk
		et_m_brkBin, //m:brkBin
		et_m_brkBinSub, //m:brkBinSub
		et_m_cGp, //m:cGp
		et_m_cGpRule, //m:cGpRule
		et_m_chr, //m:chr
		et_m_count, //m:count
		et_m_cSp, //m:cSp
		et_m_ctrlPr, //m:ctrlPr
		et_m_d, //m:d
		et_m_defJc, //m:defJc
		et_m_deg, //m:deg
		et_m_degHide, //m:degHide
		et_m_den, //m:den
		et_m_diff, //m:diff
		et_m_dispDef, //m:dispDef
		et_m_dPr, //m:dPr
		et_m_e, //m:e
		et_m_endChr, //m:endChr
		et_m_eqArr, //m:eqArr
		et_m_eqArrPr, //m:eqArrPr
		et_m_f, //m:f
		et_m_fName, //m:fName
		et_m_fPr, //m:fPr
		et_m_func, //m:func
		et_m_funcPr, //m:funcPr
		et_m_groupChr, //m:groupChr
		et_m_groupChrPr, //m:groupChrPr
		et_m_grow, //m:grow
		et_m_hideBot, //m:hideBot
		et_m_hideLeft, //m:hideLeft
		et_m_hideRight, //m:hideRight
		et_m_hideTop, //m:hideTop
		et_m_interSp, //m:interSp
		et_m_intLim, //m:intLim
		et_m_intraSp, //m:intraSp
		et_m_jc, //m:jc
		et_m_lim, //m:lim
		et_m_limLoc, //m:limLoc
		et_m_limLow, //m:limLow
		et_m_limLowPr, //m:limLowPr
		et_m_limUpp, //m:limUpp
		et_m_limUppPr, //m:limUppPr
		et_m_lit, //m:lit
		et_m_lMargin, //m:lMargin
		et_m_m, //m:m
		et_m_mathFont, //m:mathFont
		et_m_mathPr, //m:mathPr
		et_m_maxDist, //m:maxDist
		et_m_mc, //m:mc
		et_m_mcJc, //m:mcJc
		et_m_mcPr, //m:mPr
		et_m_mcs, //m:mcs
		et_m_mPr, //m:mPr
		et_m_mr, //m:mr
		et_m_nary, //m:nary
		et_m_naryLim, //m:naryLim
		et_m_naryPr, //m:naryPr
		et_m_noBreak, //m:noBreak
		et_m_nor, //m:nor
		et_m_num, //m:num
		et_m_objDist, //m:objDist
		et_m_oMath, //m:oMath
		et_m_oMathPara, // m:oMathPara
		et_m_oMathParaPr, //m:oMathParaPr
		et_m_opEmu, //m:opEmu
		et_m_phant, //m:phant
		et_m_phantPr, //m:phantPr
		et_m_plcHide, //m:plcHide
		et_m_pos, //m:pos
		et_m_postSp, //m:postSp
		et_m_preSp, //m:preSp
		et_m_r, //m:r
		et_m_rad, //m:rad
		et_m_radPr, //m:radPr
		et_m_rMargin, //m:rMargin
		et_m_rPr, //m:rPr
		et_m_rSp, //m:rSp
		et_m_rSpRule, //m:rSpRule
		et_m_scr, //m:scr
		et_m_sepChr, //m:sepChr
		et_m_show, //m:show
		et_m_shp, //m:shp
		et_m_smallFrac, //m:smallFrac
		et_m_sPre, //m:sPre
		et_m_sPrePr, //m:sPrePr
		et_m_sSub, //m:sSub
		et_m_sSubPr, //m:sSubPr
		et_m_sSubSup, //m:sSubSup
		et_m_sSubSupPr, //m:sSubSupPr
		et_m_sSup, //m:sSup
		et_m_sSupPr, //m:sSupPr
		et_m_strikeBLTR, //m:strikeBLTR
		et_m_strikeH, //m:strikeH
		et_m_strikeTLBR, //m:strikeTLBR
		et_m_strikeV, //m:strikeV
		et_m_sty, //m:sty
		et_m_sub, //m:sub
		et_m_subHide, //m:subHide
		et_m_sup, //m:sup
		et_m_supHide, //m:supHide
		et_m_t, //m:t
		et_m_transp, //m:transp
		et_m_type, //m:type
		et_m_vertJc, //m:vertJc
		et_m_wrapIndent, //m:wrapIndent
		et_m_wrapRight, //m:wrapRight
		et_m_zeroAsc, //m:zeroAsc
		et_m_zeroDesc, //m:zeroDesc
		et_m_zeroWid, //m:zeroWid

		et_mc_alternateContent, // <mc:alternateContent>

		et_o_bottom, // <o:bottom>
		et_o_callout, // <o:callout>
		et_o_clippath, // <o:clippath>
		et_o_colormenu, // <o:colormenu>
		et_o_colormru, // <o:colormru>
		et_o_column, // <o:column>
		et_o_complex, // <o:complex>
		et_o_diagram, // <o:diagram>
		et_o_entry, // <o:entry>
		et_o_equationXml, // <o:equationXml>
		et_o_extrusion, // <o:extrusion>
		et_o_FieldCodes, // <o:FieldCodes>
		et_o_fill, // <o:fill>
		et_o_idmap, // <o:idmap>
		et_o_ink, // <o:ink>
		et_o_left, // <o:left>
		et_o_LinkType, // <o:LinkType>
		et_o_lock, // <o:lock>
		et_o_LockedField, // <o:LockedField>
		et_o_OLEObject, // <o:OLEObject>
		et_o_proxy, // <o:proxy>
		et_o_r, // <o:r>
		et_o_regrouptable, // <o:regrouptable>
		et_o_rel, // <o:rel>
		et_o_relationtable, // <o:relationtable>
		et_o_right, // <o:right>
		et_o_rules, // <o:rules>
		et_o_shapedefaults, // <o:shapedefaults>
		et_o_shapelayout, // <o:shapelayout>
		et_o_signatureline, // <o:signatureline>
		et_o_skew, // <o:skew>
		et_o_top, // <o:top>

		et_pic_blipFill, // <pic:blipFill>
		et_pic_cNvPicPr, // <pic:cNvPicPr>
		et_pic_cNvPr, // <pic:cNvPr>
		et_pic_nvPicPr, // <pic:nvPicPr>
		et_pic_spPr, // <pic:spPr>
		et_pic_pic, // <pic:pic>

		et_x_cNvPr, // <xdr:cNvPr>
		et_x_spPr, // <xdr:spPr>
		et_x_groupSpPr, // <xdr:groupSpPr>

		et_c_chart, // <c:chart>
		et_c_spPr,

		et_sl_schema, // <sl:shema>
		et_sl_schemaLibrary, // <sl:schemaLibrary>

		et_v_arc, // <v:arc>
		et_v_background, // <v:background>
		et_v_curve, // <v:curve>
		et_v_f, // <v:f>
		et_v_fill, // <v:fill>
		et_v_formulas, // <v:formulas>
		et_v_group, // <v:group>
		et_v_h, // <v:h>
		et_v_handles, // <v:handles>
		et_v_image, // <v:image>
		et_v_imagedata, // <v:imagedata>
		et_v_line, // <v:line>
		et_v_oval, // <v:oval>
		et_v_path, // <v:path>
		et_v_polyline, // <v:polyline>
		et_v_rect, // <v:rect>
		et_v_roundrect, // <v:roundrect>
		et_v_shadow, // <v:shadow>
		et_v_shape, // <v:shape>
		et_v_shapetype, // <v:shapetype>
		et_v_stroke, // <v:stroke>
		et_v_textbox, // <v:textbox>
		et_v_textpath, // <v:textpath>
		et_v_ClientData, // <x:textpath>
		et_v_shape_elements,
		et_v_shape_attrs,

		et_w_abstractNum, // <w:abstractNum>
		et_w_activeWritingStyle, // <w:activeWritingStyle>
		et_w_annotationRef, // <w:annotationRef>
		et_w_autoCaption, // <w:autoCaption>
		et_w_autoCaptions, // <w:autoCaptions>
		et_w_background, // <w:background>
		et_w_bdo, // <w:bdo>
		et_w_binData, // <w:binData>
		et_w_bgPict,  // <w:bgPict>
		et_w_bookmarkEnd, // <w:bookmarkEnd>
		et_w_bookmarkStart, // <w:bookmarkStart>
		et_w_br, // <w:br>
		et_w_caption, // <w:caption>
		et_w_captions, // <w:captions>
		et_w_characterSpacingControl, // <w:characterSpacingControl>
		et_w_checkBox, // <w:checkBox>
		et_w_clrSchemeMapping, // <w:clrSchemeMapping>
		et_w_cols, // <w:cols>
		et_w_comboBox, // <w:comboBox>
		et_w_textFormPr, // <w:textFormPr> custom!
		et_w_formPr,// <w:formPr> custom!
		et_w_complexFormPr, // <w:complexFormPr> custom!
		et_w_comment, // <w:comment>
		et_w_commentRangeEnd, // <w:commentRangeEnd>
		et_w_commentRangeStart, // <w:commentRangeStart>
		et_w_commentReference, // <w:commentReference>
		et_w_compat, // <w:compat>
		et_w_compatSetting, // <w:compatSetting>
		et_w_contentPart, // <w:contentPart> <w14:contentPart>
		et_w_nvContentPartPr, // <w14:nvContentPartPr>
		et_w_continuationSeparator, // <w:continuationSeparator>
		et_w_control, // <w:control>
		et_w_customXmlDelRangeEnd, // <w:customXmlDelRangeEnd>
		et_w_customXmlDelRangeStart, // <w:customXmlDelRangeStart>
		et_w_customXmlInsRangeEnd, // <w:customXmlInsRangeEnd>
		et_w_customXmlInsRangeStart, // <w:customXmlInsRangeStart>
		et_w_customXmlMoveFromRangeEnd, // <w:customXmlMoveFromRangeEnd>
		et_w_customXmlMoveFromRangeStart, // <w:customXmlMoveFromRangeStart>
		et_w_customXmlMoveToRangeEnd, // <w:customXmlMoveToRangeEnd>
		et_w_customXmlMoveToRangeStart, // <w:customXmlMoveToRangeStart>
		et_w_cr, // <w:cr>
		et_w_date, // <w:date>
		et_w_dayLong, // <w:dayLong>
		et_w_dayShort, // <w:dayShort>
		et_w_ddList, // <w:ddList>
		et_w_del, // <w:del>
		et_w_delInstrText, // <w:delInstrText>
		et_w_delText, // <w:delText>
		et_w_drawing, // <w:drawing>
		et_w_docDefaults, // <w:docDefaults>
		et_w_docPartList, // <w:docPartList>
		et_w_documentProtection, // <w:documentProtection>
		et_w_documentType, // <w:documentType>
		et_w_docVar, // <w:docVar>
		et_w_docVars, // <w:docVars>
		et_w_dropDownList, // <w:dropDownList>
		et_w_endnote, // <w:endnote>
		et_w_endnotePr, // <w:endnotePr>
		et_w_endnoteRef, // <w:endnoteRef>
		et_w_endnoteReference, // <w:endnoteReference>
		et_w_ffData, // <w:ffData>
		et_w_fldChar, // <w:fldChar>
		et_w_fldSimple, // <w:fldSimple>
		et_w_fonts, // <w:fonts>
		et_w_font, // <w:font>
		et_w_footnote, // <w:footnote>
		et_w_footnotePr, // <w:footnotePr>
		et_w_footnoteRef, // <w:footnoteRef>
		et_w_footnoteReference, // <w:footnoteReference>
		et_w_ftr, // <w:ftr>
		et_w_hdr, // <w:hdr>
		et_w_hdrShapeDefaults, // <w:hdrShapeDefaults>
		et_w_headers, // <w:headers>
		et_w_hyperlink, // <w:hyperlink>
		et_w_altChunk, // <w:altChunk>
		et_w_altChunkPr, // <w:altChunkPr>
		et_w_ins, // <w:ins>
		et_w_instrText, // <w:instrText>
		et_w_latentStyles, // <w:latentStyles>
		et_w_lastRenderedPageBreak, // <w:lastRenderedPageBreak>
		et_w_lvl, // <w:lvl>
		et_w_lvlOverride, // <w:lvlOverride>
		et_w_monthLong, // <w:monthLong>
		et_w_monthShort, // <w:monthShort>
		et_w_moveFrom, // <w:moveFrom>
		et_w_moveTo, // <w:moveTo>
		et_w_moveFromRangeEnd, // <w:moveFromRangeEnd>
		et_w_moveFromRangeStart, // <w:moveFromRangeStart>
		et_w_moveToRangeEnd, // <w:moveToRangeEnd>
		et_w_moveToRangeStart, // <w:moveToRangeStart>
		et_w_numbering, // <w:numbering>
		et_w_num, // <w:num>
		et_w_numPicBullet, // <w:numPicBullet>
		et_w_numPr, // <w:numPr>
		et_w_noLineBreaksAfter, // <w:noLineBreaksAfter>
		et_w_noLineBreaksBefore, // <w:noLineBreaksBefore>
		et_w_nonBreakHyphen, // <w:nonBreakHyphen>
		et_w_object, // <w:object>
		et_w_p, // <w:p>
		et_w_pBdr, // <w:pBdr>
		et_w_permEnd, // <w:permEnd>
		et_w_permStart, // <w:permStart>
		et_w_pgBorders, // <w:pgBorders>
		et_w_pgNum, // <w:pgNum>
		et_w_pict, // <w:pict>
		et_w_placeholder, // <w:placeholder>
		et_w_pPr, // <w:pPr>
		et_w_pPrChange, // <w:pPrChange>
		et_w_proofErr, // <w:proofErr>
		et_w_proofState, // <w:proofState>
		et_w_ptab, // <w:ptab>
		et_w_r, // <w:r>
		et_w_readModeInkLockDown, // <w:readModeInkLockDown>
		et_w_revisionView, // <w:revisionView>
		et_w_rsids, // <w:rsids>
		et_w_rPr, // <w:rPr>
		et_w_rPrChange, // <w:rPrChange>
		et_w_ruby, // <w:ruby>
		et_w_saveThroughXslt, // <w:saveThroughXslt>
		et_w_sdt, // <w:sdt>
		et_w_sdtContent, // <w:sdtContent>
		et_w_dir, // <w:dir>
		et_w_sdtEndPr, // <w:sdtEndPr>
		et_w_sdtPr, // <w:sdtPr>
		et_w_sdtCheckbox, // <w:checkbox>
		et_w_sdtPicture, // <w:picture>
		et_w_sdtCheckboxSymbol, // <w:checkedState>
		et_w_sectPr, // <w:sectPr>
		et_w_sectPrChange, // <w:sectPrChange>
		et_w_separator, // <w:separator>
		et_w_shapeDefaults, // <w:shapeDefaults>
		et_w_smartTag, // <w:smartTagType>
		et_w_smartTagType, // <w:smartTagType>
		et_w_softHyphen, // <w:softHyphen>
		et_w_wordDocument, // <w:wordDocument>
		et_w_document, // <w:document>
		et_w_settings, // <w:settings>
		et_w_style, // <w:style>
		et_w_styles, // <w:styles>
		et_w_stylePaneFormatFilter, // <w:stylePaneFormatFilter>
		et_w_stylePaneSortMethod, // <w:stylePaneSortMethod>
		et_w_sym, // <w:sym>
		et_w_t, // <w:t>
		et_w_tab, // <w:tab>
		et_w_tabs, // <w:tabs>
		et_w_tbl, // <w:tbl>
		et_w_tblBorders, // <w:tblBorders>
		et_w_tblCellMar, // <w:tblCellMar>
		et_w_tblGrid, // <w:tblGrid>
		et_w_tblGridChange, // <w:tblGridChange>
		et_w_tblPr, // <w:tblPr>
		et_w_tblPrChange, // <w:tblPrChange>
		et_w_tblPrEx, // <w:tblPrEx>
		et_w_tblPrExChange, // <w:tblPrExChange>
		et_w_tblStylePr, // <w:tblStylePr>
		et_w_tc, // <w:tc>
		et_w_tcBorders, // <w:tcBorders>
		et_w_tcMar, // <w:tcMar>
		et_w_tcPr, // <w:tcPr>
		et_w_tcPrChange, // <w:tcPrChange>
		et_w_textInput, // <w:textInput>
		et_w_tr, // <w:tr>
		et_w_trPr, // <w:trPr>
		et_w_trPrChange, // <w:trPrChange>
		et_w_txbxContent, // <w:txbxContent>
		et_w_writeProtection, // <w:writeProtection>
		et_w_yearLong, // <w:yearLong>
		et_w_yearShort, // <w:yearShort>
		et_w_zoom, // <w:zoom>
		et_w_docParts,
		et_w_docPart,
		et_w_docPartBody,
		et_w_docPartPr,
		et_w_docPartCategory,
		et_w_docPartTypes,
		et_w_docPartBehaviors,
		et_w_stylisticSets,

		et_wd_anchorLock, // <wd:anchorLock>
		et_wd_borderbottom, // <wd:borderbottom>
		et_wd_borderleft, // <wd:borderleft>
		et_wd_borderright, // <wd:borderright>
		et_wd_bordertop, // <wd:bordertop>
		et_wd_wrap, // <wd:wrap>

		et_wp_anchor, // <wp:anchor>
		et_wp_cNvGraphicFramePr, // <wp:cNvGraphicFramePr>
		et_wp_docPr, // <wp:docPr>
		et_wp_effectExtent, // <wp:effectExtent>
		et_wp_extent, // <wp:extent>
		et_wp_inline, // <wp:inline>
		et_wp_positionH, // <wp:positionH>
		et_wp_positionV, // <wp:positionV>
		et_wp_sizeRelH, // <wp14:sizeRelH>
		et_wp_sizeRelV, // <wp14:sizeRelV>
		et_wp_wrapNone, // <wp:wrapNone>
		et_wp_wrapPolygon, // <wp:wrapPolygon>
		et_wp_wrapSquare, // <wp:wrapSquare>
		et_wp_wrapThrough, // <wp:wrapThrough>
		et_wp_wrapTight, // <wp:wrapTight>
		et_wp_wrapTopAndBottom, // <wp:wrapTopAndBottom>

		et_w15_presenceInfo, // <w15:presenceInfo>
		et_w15_person, // <w15:person>
		et_w15_commentEx, // <w15:commentEx>

		et_w16_commentId, // <w16cid:commentId>
		et_w16_commentExtensible, // <w16cex:commentExtensible>

		et_w_ShapeTextBody,		//<wps:txbx>
		et_w_Shape,				//<wps:wsp>
		et_w_GroupShape,		//<wpg:wgp>
		et_w_LockedCanvas,		//<wpc:wgc>
		et_w_cNvPr,				//<wps:cNvPr>
		et_w_spPr,				//<wps:spPr>
		et_w_groupSpPr,			//<wpg:grpSpPr>
		et_w_ConnectionNonVisualShapeProps,//<wps:cNvSpPr>
		et_w_NonVisualGroupShapeDrawingProps,//<wpg:cNvGrpSpPr>
		et_w_ConnectionNonVisualConnShapeProps,
		et_w_ShapeNonVisual,		//<wps:spPr>

		et_ct_extlst,
		et_ct_ChartSpace,
		et_ct_boolean,
		et_ct_relid,
		et_ct_pagesetup,
		et_ct_pagemargins,
		et_ct_headerfooter,
		et_ct_printsettings,
		et_ct_externaldata,
		et_ct_dispblanksas,
		et_ct_unsignedint,
		et_ct_extension,
		et_ct_layout,
		et_ct_manuallayout,
		et_ct_layouttarget,
		et_ct_layoutmode,
		et_ct_double,
		et_ct_dtable,
		et_ct_serax,
		et_ct_scaling,
		et_ct_logbase,
		et_ct_orientation,
		et_ct_axpos,
		et_ct_chartlines,
		et_ct_title,
		et_ct_tx,
		et_ct_strref,
		et_ct_strdata,
		et_ct_strval,
		et_ct_numfmt,
		et_ct_tickmark,
		et_ct_ticklblpos,
		et_ct_crosses,
		et_ct_skip,
		et_ct_timeunit,
		et_ct_dateax,
		et_ct_lbloffset,
		et_ct_axisunit,
		et_ct_lblalgn,
		et_ct_catax,
		et_ct_dispunitslbl,
		et_ct_builtinunit,
		et_ct_dispunits,
		et_ct_crossbetween,
		et_ct_valax,
		et_ct_sizerepresents,
		et_ct_bubblescale,
		et_ct_bubbleser,
		et_ct_sertx,
		et_ct_dpt,
		et_ct_marker,
		et_ct_markerstyle,
		et_ct_markersize,
		et_ct_pictureoptions,
		et_ct_pictureformat,
		et_ct_picturestackunit,
		et_ct_dlbls,
		et_ct_dlbl,
		et_ct_dlblpos,
		et_ct_trendline,
		et_ct_trendlinetype,
		et_ct_order,
		et_ct_period,
		et_ct_trendlinelbl,
		et_ct_errbars,
		et_ct_errdir,
		et_ct_errbartype,
		et_ct_errvaltype,
		et_ct_numdatasource,
		et_ct_numdata,
		et_ct_numval,
		et_ct_numref,
		et_ct_axdatasource,
		et_ct_multilvlstrref,
		et_ct_lvl,
		et_ct_multilvlstrdata,
		et_ct_bubblechart,
		et_ct_bandfmts,
		et_ct_surface3dchart,
		et_ct_surfaceser,
		et_ct_bandfmt,
		et_ct_surfacechart,
		et_ct_secondpiesize,
		et_ct_splittype,
		et_ct_ofpietype,
		et_ct_custsplit,
		et_ct_ofpiechart,
		et_ct_pieser,
		et_ct_gapamount,
		et_ct_bar3dchart,
		et_ct_bardir,
		et_ct_bargrouping,
		et_ct_barser,
		et_ct_shape,
		et_ct_overlap,
		et_ct_barchart,
		et_ct_holesize,
		et_ct_doughnutchart,
		et_ct_firstsliceang,
		et_ct_pie3dchart,
		et_ct_piechart,
		et_ct_scatterser,
		et_ct_scatterstyle,
		et_ct_scatterchart,
		et_ct_radarser,
		et_ct_radarstyle,
		et_ct_radarchart,
		et_ct_stockchart,
		et_ct_lineser,
		et_ct_updownbars,
		et_ct_updownbar,
		et_ct_line3dchart,
		et_ct_grouping,
		et_ct_linechart,
		et_ct_area3dchart,
		et_ct_areaser,
		et_ct_areachart,
		et_ct_thickness,
		et_ct_surface,
		et_ct_perspective,
		et_ct_depthpercent,
		et_ct_roty,
		et_ct_hpercent,
		et_ct_rotx,
		et_ct_view3d,
		et_ct_pivotfmt,
		et_ct_pivotfmts,
		et_ct_protection,
		et_ct_pivotsource,
		et_ct_style1,
		et_ct_style,
		et_ct_textlanguageid,
		et_ct_Chart,
		et_ct_ChartStyle,
		et_ct_Title,
		et_ct_Tx,
		et_ct_Rich,
		et_ct_Legend,
		et_ct_Overlay,
		et_ct_LegendPos,
		et_ct_LegendEntry,
		et_ct_Layout,
		et_ct_ManualLayout,
		et_ct_PlotArea,
		et_ct_CatAx,
		et_ct_ValAx,
		et_ct_CatAy,
		et_ct_ValAy,
		et_ct_BasicChart,
		et_ct_Series,
		et_ct_NumPoint,
		et_ct_NumCache,
		et_ct_NumCacheRef,
		et_ct_NumCacheValues,
		et_ct_SeriesCat,
		et_ct_StrCacheRef,
		et_ct_SeriesTx,
		et_ct_SeriesMarker,
		et_ct_SeriesDataLabels,
		et_ct_SeriesShapeProperties,
		et_ct_SeriesShapeIndex,
		et_ct_SeriesShapeOrder,
		et_ct_SeriesShapeOutline,
		et_ct_alternatecontent,
		et_ct_alternatecontentchoice,
		et_ct_alternatecontentfallback,

		et_ct_TickMarks,
		et_ct_Gridlines,
		et_ct_DataPoint,
		et_ct_DataLabels,
		et_ct_DataLabel,
		et_ct_DataLabelHidden,
		et_ct_NumberFormat,
		et_ct_SeriesLayoutProperties,
		et_ct_Subtotals,
		et_ct_Statistics,
		et_ct_Binning,
		et_ct_DataLabelVisibilities,
		et_ct_SeriesElementVisibilities,
		et_ct_StringValue,
		et_ct_StringLevel,
		et_ct_NumericValue,
		et_ct_NumericLevel,
		et_ct_Text,
		et_ct_TextData,
		et_ct_Formula,
		et_ct_CatScaling,
		et_ct_ValScaling,
		et_ct_AxisUnitsLabel,
		et_ct_AxisUnit,
		et_ct_Axis,
		et_ct_PlotSurface,
		et_ct_PlotAreaRegion,
		et_ct_Data,
		et_ct_StrDimension,
		et_ct_NumDimension,
		et_ct_ChartData,
		et_ct_ExternalData,

		et_cs_ChartStyle,
		et_cs_StyleEntry,
		et_cs_MarkerLayout,
		et_cs_ColorStyle,
		et_cs_SchemeClr,
		et_cs_Variation,

		et_cdr_FromTo,
		et_cdr_Ext,
		et_cdr_RelSizeAnchor,
		et_cdr_AbsSizeAnchor,

		et_x_Unknown,
		et_x_SpreadsheetFlat,
		et_x_BookViews, // <bookViews>
		et_x_Workbook,
		et_x_WorkbookPr,
		et_x_WorkbookProtection,
		et_x_WorkbookView, // <workbookView>
		et_x_DefinedNames, // <definedNames>
		et_x_DefinedName, // <definedName>
		et_x_Sheets, // <sheets>
		et_x_Sheet, // <sheet>
		et_x_ExternalReference,
		et_x_ExternalReferences,
		et_x_Si, // <si>
		et_x_PhoneticPr, // <PhoneticPr>
		et_x_r, // <r>
		et_x_rPr, // <rPr>
		et_x_rPh, // <rPh>
		et_x_t, // <t>
		et_x_Borders, // <borders>
		et_x_Border,
		et_x_BorderProp,
		et_x_CalcPr, // <calcPr>
		et_x_CellStyles,
		et_x_CellStyle,
		et_x_CellStyleXfs,
		et_x_CellXfs,
		et_x_Xfs,
		et_x_Aligment,
		et_x_Protection,
		et_x_Colors,
		et_x_Color,
		et_x_RgbColor,
		et_x_IndexedColors,
		et_x_MruColors,
		et_x_Dxfs,
		et_x_Dxf,
		et_x_Fills,
		et_x_Fill,
		et_x_GradientFill,
		et_x_GradientStop,
		et_x_PatternFill,
		et_x_BgColor,
		et_x_FgColor,
		et_x_Fonts,
		et_x_Font,
		et_x_NumFmts,
		et_x_NumFmt,
		et_x_TableStyles,
		et_x_TableStyle,
		et_x_TableStyleElement,
		et_x_SheetData,
		et_x_Row,
		et_x_Cell,
		et_x_Formula,
		et_x_Data,
		et_x_Cols,
		et_x_Col,
		et_x_Hyperlinks,
		et_x_Hyperlink,
		et_x_PageMargins,
		et_x_PageSetup,
		et_x_PrintOptions,
		et_x_MergeCells,
		et_x_MergeCell,
		et_x_Dimension,
		et_x_SheetFormatPr,
		et_x_CellAnchor,
		et_x_Pic,
		et_x_BlipFill,
		et_x_Blip,
		et_x_FromTo,
		et_x_Pos,
		et_x_Ext,
		et_x_ClientData,
		et_x_CalcCell,
		et_x_SheetViews,
		et_x_SheetView,
		et_x_ProtectedRanges,
		et_x_ProtectedRange,
		et_x_CellWatches,
		et_x_CellWatch,
		et_x_GraphicFrame,
		et_x_Graphic,
		et_x_GraphicData,
		et_x_OleObjects,
		et_x_OleObject,
		et_x_OleObjectPr,
		et_x_ExtAnchor,
		et_x_Controls,
		et_x_Control,
		et_x_ControlPr,
		et_x_OcxPr,
		et_x_QueryTable,
		et_x_QueryTableField,
		et_x_QueryTableFields,
		et_x_QueryTableRefresh,
		et_x_QueryTableDeletedFields,
		et_x_QueryTableDeletedField,
		et_x_Connections,
		et_x_Connection,
		et_x_rangePr,
		et_x_dbPr,
		et_x_textPr,
		et_x_olapPr,
		et_x_webPr,
		et_x_parameter,
		et_x_parameters,
		et_x_textFields,
		et_x_textField,
		et_x_TableParts,
		et_x_TablePart,
		et_x_Table,
		et_x_TableColumns,
		et_x_TableColumn,
		et_x_TableStyleInfo,
		et_x_AltTextTable,
		et_x_SortState,
		et_x_SortCondition,
		et_x_Autofilter,
		et_x_FilterColumn,
		et_x_ColorFilter,
		et_x_DynamicFilter,
		et_x_CustomFilters,
		et_x_Filters,
		et_x_Filter,
		et_x_DateGroupItem,
		et_x_Authors,
		et_x_CommentList,
		et_x_Comment,
		et_x_ThreadedComments,
		et_x_ThreadedComment,
		et_x_ThreadedCommentMentions,
		et_x_ThreadedCommentMention,
		et_x_PersonList,
		et_x_Person,
		et_x_ConditionalFormatting,
		et_x_ConditionalFormattingRule,
		et_x_ColorScale,
		et_x_DataBar,
		et_x_FormulaCF,
		et_x_IconSet,
		et_x_ConditionalFormatValueObject,
		et_x_ConditionalFormatIconSet,
		et_x_PageSetUpPr,
		et_x_OutlinePr,
		et_x_SheetPr,
		et_x_Pane,
		et_x_ExternalBook,
		et_x_ExternalSheetNames,
		et_x_ExternalDefinedNames,
		et_x_ExternalDefinedName,
		et_x_ExternalSheetDataSet,
		et_x_ExternalSheetData,
		et_x_ExternalRow,
		et_x_ExternalCell,
		et_x_OleLink,
		et_x_OleItems,
		et_x_OleItem,
		et_x_DdeLink,
		et_x_DdeItems,
		et_x_DdeItem,
		et_x_DdeValues,
		et_x_DdeValue,
		et_x_Selection,
		et_x_LegacyDrawingWorksheet,
		et_x_LegacyDrawingHFWorksheet,
		et_x_PictureWorksheet,
		et_x_HeaderFooterWorksheet,
		et_x_HeaderFooterElementWorksheet,
		et_x_Break,
		et_x_RowColBreaks,
		et_x_SheetProtection,
		et_x_DataValidations,
		et_x_DataValidation,
		et_x_DataValidationFormula,
		et_x_DataConsolidate,
		et_x_DataRefs,
		et_x_DataRef,
		et_x_FormControlPr,
		et_x_ListItems,
		et_x_ListItem,

		et_x_WorkbookPivotCache,
		et_x_WorkbookPivotCaches,

		et_x_PivotTableDefinition,
		et_x_PivotCacheDefinition,
		et_x_PivotCacheDefinitionExt,
		et_x_PivotCacheRecords,
		et_x_ColumnRowFields,
		et_x_ColumnRowItems,
		et_x_Field,
		et_x_ColumnRowItem,
		et_x_DataField,
		et_x_DataFields,
		et_x_PageField,
		et_x_PageFields,
		et_x_PivotField,
		et_x_PivotFields,
		et_x_FieldItem,
		et_x_FieldItems,
		et_x_AutoSortScope,
		et_x_PivotArea,
		et_x_References,
		et_x_Reference,
		et_x_PivotTableLocation,
		et_x_PivotTableFormats,
		et_x_PivotTableFormat,
		et_x_PivotTableStyleInfo,
		et_x_PivotTableFilters,
		et_x_PivotTableFilter,
		et_x_PivotCacheFields,
		et_x_PivotCacheField,
		et_x_SharedItems,
		et_x_FieldGroupProperties,
		et_x_DiscreteGroupingProperties,
		et_x_RangeGroupingProperties,
		et_x_OLAPGroupItems,
		et_x_PivotCacheSource,
		et_x_WorksheetSource,
		et_x_ConsolidationSource,
		et_x_PageItemValues,
		et_x_PageItems,
		et_x_PageItem,
		et_x_RangeSets,
		et_x_RangeSet,
		et_x_PivotCharacterValue,
		et_x_PivotBooleanValue,
		et_x_PivotErrorValue,
		et_x_PivotDateTimeValue,
		et_x_PivotNoValue,
		et_x_PivotNumericValue,
		et_x_PivotCacheRecord,
		et_x_SharedItemsIndex,
		et_x_MemberPropertyIndex,

		et_x_SlicerCacheDefinition,
		et_x_SlicerCachePivotTable,
		et_x_SlicerCacheData,
		et_x_OlapSlicerCache,
		et_x_TabularSlicerCache,
		et_x_OlapSlicerCacheLevelsData,
		et_x_OlapSlicerCacheSelections,
		et_x_TabularSlicerCacheItems,
		et_x_OlapSlicerCacheLevelData,
		et_x_OlapSlicerCacheSelection,
		et_x_TabularSlicerCacheItem,
		et_x_OlapSlicerCacheRange,
		et_x_OlapSlicerCacheItemParent,
		et_x_OlapSlicerCacheItem,
		et_x_SlicerCacheOlapLevelName,
		et_x_SlicerCacheHideNoData,
		et_x_TableSlicerCache,
		et_x_SlicerStyleElement,
		et_x_Slicer,
		et_x_SlicerCache,
		et_x_SlicerRef,
		et_x_SlicerStyle,
		et_x_Slicers,
		et_x_SlicerCaches,
		et_x_SlicerRefs,
		et_x_SlicerStyles,

		et_x_SortRule,
		et_x_SortRules,
		et_x_ColumnFilter,
		et_x_NsvFilter,
		et_x_NamedSheetView,
		et_x_NamedSheetViews,

		et_x_SparklineGroups,
		et_x_SparklineGroup,
		et_x_Sparklines,
		et_x_Sparkline,
		et_x_Style2003
	};

	class File;

	class Document
	{
	public:
		Document();
		virtual ~Document();

		std::wstring m_sDocumentPath;
		std::map<std::wstring, NSCommon::smart_ptr<OOX::File>> m_mapContent;
	};

	class WritingElement
	{
	public:
		WritingElement(OOX::Document *pMain = NULL);
		virtual ~WritingElement();

		virtual void			fromXML(XmlUtils::CXmlNode& node)	= 0;
		virtual std::wstring	toXML()     const					= 0;
		virtual EElementType    getType()   const { return OOX::et_Unknown; }
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) {}
		
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

	public:
		OOX::Document *m_pMainDocument;
	};
	
	template<typename ElemType = WritingElement>
	class WritingElementWithChilds : public WritingElement
	{
	public:
		std::vector<ElemType *>  m_arrItems;

		WritingElementWithChilds(OOX::Document *pMain = NULL) :  WritingElement(pMain){}
		
		virtual ~WritingElementWithChilds()
		{
			ClearItems();
		}

		virtual void ClearItems()
		{
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i]) delete m_arrItems[i];
			}
			m_arrItems.clear();
		}
	};
}
