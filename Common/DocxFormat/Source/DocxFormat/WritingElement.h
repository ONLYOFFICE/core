#pragma once

//#include "NamespaceOwn.h"
#include "../XML/XmlUtils.h"
#include "atlstr.h"

namespace OOX
{
#define WritingElement_AdditionConstructors(Class) \
	Class(XmlUtils::CXmlNode& oNode)\
	{\
		fromXML( oNode );\
	}\
	Class(XmlUtils::CXmlLiteReader& oReader)\
	{\
		fromXML( oReader );\
	}\
	const Class& operator =(const XmlUtils::CXmlNode &oNode)\
	{\
		fromXML( (XmlUtils::CXmlNode &)oNode );\
		return *this;\
	}\
	const Class& operator =(const XmlUtils::CXmlLiteReader& oReader)\
	{\
		fromXML( (XmlUtils::CXmlLiteReader&)oReader );\
		return *this;\
	}

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

// Следующие 3 define используются для чтения аттрибутов через CXmlLiteReader
#define WritingElement_ReadAttributes_Start(Reader) \
	if ( Reader.GetAttributesCount() <= 0 )\
		return;\
	if ( !Reader.MoveToFirstAttribute() )\
		return;\
	CWCharWrapper wsName = Reader.GetName();\
	while( !wsName.IsNull() )\
	{

#define WritingElement_ReadAttributes_Start_No_NS(Reader) \
	if ( Reader.GetAttributesCount() <= 0 )\
		return;\
	if ( !Reader.MoveToFirstAttribute() )\
		return;\
	CString wsName = XmlUtils::GetNameNoNS(Reader.GetName());\
	while( !wsName.IsEmpty() )\
	{

#define WritingElement_ReadAttributes_Read_if(Reader, AttrName, Value) \
		if ( AttrName == wsName )\
			Value = Reader.##GetText();

#define WritingElement_ReadAttributes_Read_else_if(Reader, AttrName, Value) \
		else if ( AttrName == wsName )\
			Value = Reader.##GetText();

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
		
		et_a_p, // <a:p>
		et_a_pPr, // <a:pPr>
		et_a_r, // <a:p>
		et_a_rPr, // <a:pPr>
		et_a_t, // <w:t>

		et_ds_schemaRef, // <ds:shemeRef>
		et_ds_schemaRefs, // <ds:schemaRefs>		
		
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

		et_p_cNvPicPr, // <p:cNvPicPr>
		et_p_cNvPr, // <p:cNvPr>
		et_p_pic, // <p:pic>

		et_pic_blipFill, // <pic:blipFill>
		et_pic_cNvPicPr, // <pic:cNvPicPr>
		et_pic_cNvPr, // <pic:cNvPr>
		et_pic_nvPicPr, // <pic:nvPicPr>
		et_pic_spPr, // <pic:spPr>
		et_pic_pic, // <pic:pic>

		et_xdr_cNvPr, // <xdr:cNvPr>
		et_xdr_spPr, // <xdr:spPr>

		et_c_chart, // <c:chart>

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

		et_w_abstractNum, // <w:abstractNum>
		et_w_activeWritingStyle, // <w:activeWritingStyle>
		et_w_annotationRef, // <w:annotationRef>
		et_w_autoCaption, // <w:autoCaption>
		et_w_autoCaptions, // <w:autoCaptions>
		et_w_background, // <w:background>
		et_w_bdo, // <w:bdo>
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
		et_w_comment, // <w:comment>
		et_w_commentRangeEnd, // <w:commentRangeEnd>
		et_w_commentRangeStart, // <w:commentRangeStart>
		et_w_commentReference, // <w:commentReference>
		et_w_compat, // <w:compat>
		et_w_compatSetting, // <w:compatSetting>
		et_w_contentPart, // <w:contentPart>
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
		et_w_ins, // <w:ins>
		et_w_instrText, // <w:instrText>
		et_w_latentStyles, // <w:latentStyles>
		et_w_lastRenderedPageBreak, // <w:lastRenderedPageBreak>
		et_w_lvl, // <w:lvl>
		et_w_lvlOverride, // <w:lvlOverride>
		et_w_monthLong, // <w:monthLong>
		et_w_monthShort, // <w:monthShort>
		et_w_moveFromRangeEnd, // <w:moveFromRangeEnd>
		et_w_moveFromRangeStart, // <w:moveFromRangeStart>
		et_w_moveToRangeEnd, // <w:moveToRangeEnd>
		et_w_moveToRangeStart, // <w:moveToRangeStart>
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
		et_w_sectPr, // <w:sectPr>
		et_w_sectPrChange, // <w:sectPrChange>
		et_w_separator, // <w:separator>
		et_w_shapeDefaults, // <w:shapeDefaults>
		et_w_smartTag, // <w:smartTagType>
		et_w_smartTagType, // <w:smartTagType>
		et_w_softHyphen, // <w:softHyphen>
		et_w_style, // <w:style>
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
		et_wp_wrapNone, // <wp:wrapNone>
        et_wp_wrapPolygon, // <wp:wrapPolygon>
		et_wp_wrapSquare, // <wp:wrapSquare>
		et_wp_wrapThrough, // <wp:wrapThrough>
		et_wp_wrapTight, // <wp:wrapTight>
		et_wp_wrapTopAndBottom, // <wp:wrapTopAndBottom>

		et_w15_presenceInfo, // <w15:presenceInfo>
		et_w15_person, // <w15:person>
		et_w15_commentEx, // <w15:commentEx>
	};

	class WritingElement
	{
	public:
		WritingElement(){}
		virtual ~WritingElement() {}

		virtual void         fromXML(XmlUtils::CXmlNode& node)          = 0;
        virtual CString      toXML() const                              = 0;
		virtual EElementType getType() const
		{
			return OOX::et_Unknown;
		}
		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) {}
	};
	template<typename ElemType = WritingElement>
	class WritingElementWithChilds : public WritingElement
	{
	public:
		WritingElementWithChilds(){}
		virtual ~WritingElementWithChilds() {ClearItems();}
		virtual void ClearItems()
		{
			for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
					delete m_arrItems[nIndex];

				m_arrItems[nIndex] = NULL;
			}

			m_arrItems.RemoveAll();
		}
		CSimpleArray<ElemType *>         m_arrItems;
	};
}
