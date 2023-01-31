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

namespace BinDocxRW
{
const double eps     = 0.001;

const int g_tabtype_left = 0;
const int g_tabtype_right = 1;
const int g_tabtype_center = 2;
const int g_tabtype_clear = 3;

const int border_None   = 0x0000;
const int border_Single = 0x0001;

const int heightrule_AtLeast = 0x00;
const int heightrule_Auto    = 0x01;
const int heightrule_Exact   = 0x02;

const int align_Right   = 0;
const int align_Left    = 1;
const int align_Center  = 2;
const int align_Justify = 3;

const int vertalignjc_Top    = 0x00;
const int vertalignjc_Center = 0x01;
const int vertalignjc_Bottom = 0x02;

const int vmerge_Restart  = 0x01;
const int vmerge_Continue = 0x02;

const int linerule_AtLeast = 0;
const int linerule_Auto    = 1;
const int linerule_Exact   = 2;

const int orientation_Portrait  = 0x00;
const int orientation_Landscape = 0x01;

const int numbering_numfmt_None        = 0x0000;
const int numbering_numfmt_Bullet      = 0x1001;
const int numbering_numfmt_Decimal     = 0x2002;
const int numbering_numfmt_LowerRoman  = 0x2003;
const int numbering_numfmt_UpperRoman  = 0x2004;
const int numbering_numfmt_LowerLetter = 0x2005;
const int numbering_numfmt_UpperLetter = 0x2006;
const int numbering_numfmt_DecimalZero = 0x2007;

const int numbering_suff_Tab     = 1;
const int numbering_suff_Space   = 2;
const int numbering_suff_Nothing = 3;

const int tblwidth_Auto = 0x00;
const int tblwidth_Mm   = 0x01;
const int tblwidth_Nil  = 0x02;
const int tblwidth_Pct  = 0x03;

const int fontstyle_mask_regular = 1;
const int fontstyle_mask_italic = 2;
const int fontstyle_mask_bold = 4;
const int fontstyle_mask_bolditalic = 8;

const int styletype_Character = 0x01;
const int styletype_Numbering = 0x02;
const int styletype_Paragraph = 0x03;
const int styletype_Table = 0x04;

const int fieldstruct_none = 0;
const int fieldstruct_toc = 1;
const int fieldstruct_hyperlink = 2;

const double g_dKoef_mm_to_pt = 72 / (2.54 * 10);
const double g_dKoef_mm_to_twips = 20 * g_dKoef_mm_to_pt;
const double g_dKoef_mm_to_emu = 36000;
const double g_dKoef_mm_to_eightpoint = 8 * g_dKoef_mm_to_pt;
const double g_dKoef_mm_to_hps = 2 * g_dKoef_mm_to_pt;

const static wchar_t* g_sFormatSignature = L"DOCY";
const int g_nFormatVersion = 5;
const int g_nFormatVersionNoBase64 = 10; 
extern int g_nCurFormatVersion;
	namespace c_oAscWrapStyle{enum c_oSerFormat
	{
		Inline = 0,
		Flow = 1
	};}
	namespace c_oSerConstants{enum c_oSerConstants
	{
		ErrorFormat =  -2,
		ErrorUnknown =  -1,
		ReadOk = 0,
		ReadUnknown = 1,
		ErrorStream = 0x55
	};}
	namespace c_oSerTableTypes{enum c_oSerTableTypes
	{
		Signature	= 0,
		Info		= 1,
		Media		= 2,
		Numbering	= 3,
		HdrFtr		= 4,
		Style		= 5,
		Document	= 6,
		Other		= 7,
		Comments	= 8,
		Settings	= 9,
		Footnotes	= 10,
		Endnotes	= 11,
		Background	= 12,
		VbaProject	= 13,
		App			= 15,
		Core		= 16,
		DocumentComments = 17,
		CustomProperties = 18,
		Glossary	= 19,
		Customs		= 20,
		OForm		= 21
	};}
	namespace c_oSerSigTypes{enum c_oSerSigTypes
	{
		Version = 0
	};}
	namespace c_oSerHdrFtrTypes{enum c_oSerHdrFtrTypes
	{
		Header = 0,
		Footer = 1,
		HdrFtr_First = 2,
		HdrFtr_Even = 3,
		HdrFtr_Odd = 4,
		HdrFtr_Content = 5,
		HdrFtr_Y2 = 6,//устарел
		HdrFtr_Y = 7//устарел
	};}
	namespace c_oSerNumTypes{enum c_oSerNumTypes
	{
		AbstractNums = 0,
		AbstractNum = 1,
		AbstractNum_Id = 2,
		AbstractNum_Type = 3,
		AbstractNum_Lvls = 4,
		Lvl = 5,
		lvl_Format = 6,//deprecated (instead use lvl_NumFmt)
		lvl_Jc_deprecated = 7,//deprecated
		lvl_LvlText = 8,
		lvl_LvlTextItem = 9,
		lvl_LvlTextItemText = 10,
		lvl_LvlTextItemNum = 11,
		lvl_Restart = 12,
		lvl_Start = 13,
		lvl_Suff = 14,
		lvl_ParaPr = 15,
		lvl_TextPr = 16,
		Nums = 17,
		Num = 18,
		Num_ANumId = 19,
		Num_NumId = 20,
		lvl_PStyle = 21,
		NumStyleLink = 22,
		StyleLink = 23,
		lvl_NumFmt = 24,
		NumFmtVal = 25,
		NumFmtFormat = 26,
		Num_LvlOverride = 27,
		StartOverride = 28,
		ILvl = 29,
		Tentative = 30,
		Tplc = 31,
		IsLgl = 32,
		LvlLegacy = 33,
		Legacy = 34,
		LegacyIndent = 35,
		LegacySpace = 36,
		lvl_Jc = 37
	};}
	namespace c_oSerOtherTableTypes{enum c_oSerOtherTableTypes
	{
		ImageMap		= 0, //not using now
		ImageMap_Src	= 1, //not using now
		EmbeddedFonts	= 2,
		DocxTheme		= 3
	};}
	namespace c_oSerFontsTypes{enum c_oSerFontsTypes
	{
		Name = 0
	};}
	namespace c_oSerImageMapTypes{enum c_oSerImageMapTypes
	{
		Src = 0
	};}
	namespace c_oSerStyleTypes{enum c_oSerStyleTypes
	{
		Name = 0,
		BasedOn = 1,
		Next = 2
	};}
	namespace c_oSerPropLenType{enum c_oSerPropLenType
	{
		Null		= 0,
		Byte		= 1,
		Short		= 2,
		Three		= 3,
		Long		= 4,
		Double		= 5,
		Variable	= 6,
		Double64	= 7,
		Long64		= 8
	};}
	namespace c_oSer_st{enum c_oSer_st
	{
		DefpPr = 0,
		DefrPr = 1,
		Styles = 2
	};}
	namespace c_oSer_sts{enum c_oSer_sts
	{
		Style = 0,
		Style_Id = 1,
		Style_Name = 2,
		Style_BasedOn = 3,
		Style_Next = 4,
		Style_TextPr = 5,
		Style_ParaPr = 6,
		Style_TablePr = 7,
		Style_Default = 8,
		Style_Type = 9,
		Style_qFormat = 10,
		Style_uiPriority = 11,
		Style_hidden = 12,
		Style_semiHidden = 13,
		Style_unhideWhenUsed = 14,
		Style_RowPr = 15,
		Style_CellPr = 16,
		Style_TblStylePr = 17,
		Style_Link = 18,
		Style_CustomStyle = 19,
		Style_Aliases = 20,
		Style_AutoRedefine = 21,
		Style_Locked = 22,
		Style_Personal = 23,
		Style_PersonalCompose = 24,
		Style_PersonalReply = 25
	};}
	namespace c_oSerProp_tblStylePrType{enum c_oSerProp_tblStylePrType
	{
		TblStylePr = 0,
		Type = 1,
		RunPr = 2,
		ParPr = 3,
		TblPr = 4,
		TrPr = 5,
		TcPr = 6
	};}
	namespace c_oSerProp_tblPrType{enum c_oSerProp_tblPrType
	{
		Rows = 0,
		Cols = 1,
		Jc = 2,
		TableInd = 3,
		TableW = 4,
		TableCellMar = 5,
		TableBorders = 6,
		Shd = 7,
		tblpPr = 8,
		Look = 9,
		Style = 10,
		tblpPr2 = 11,
		Layout = 12,
		tblPrChange = 13,
		TableCellSpacing = 14,
		RowBandSize = 15,
		ColBandSize = 16,
		tblCaption = 17,
		tblDescription = 18,
		TableIndTwips = 19,
		TableCellSpacingTwips = 20,
		tblOverlap = 21
	};}
	namespace c_oSer_tblpPrType{enum c_oSer_tblpPrType
	{
		Page = 0,
		X = 1,
		Y = 2,
		Paddings = 3
	};}
	namespace c_oSer_tblpPrType2{enum c_oSer_tblpPrType2
	{
		HorzAnchor = 0,
		TblpX = 1,
		TblpXSpec = 2,
		VertAnchor = 3,
		TblpY = 4,
		TblpYSpec = 5,
		Paddings = 6,
		TblpXTwips = 7,
		TblpYTwips = 8
	};}
	namespace c_oSerProp_pPrType{enum c_oSerProp_pPrType
	{
		ContextualSpacing = 0,
		Ind = 1,
		Ind_Left = 2,
		Ind_Right = 3,
		Ind_FirstLine = 4,
		Jc = 5,
		KeepLines = 6,
		KeepNext = 7,
		PageBreakBefore = 8,
		Spacing = 9,
		Spacing_Line = 10,
		Spacing_LineRule = 11,
		Spacing_Before = 12,
		Spacing_After = 13,
		Shd = 14,
		Tab = 17,
		Tab_Item = 18,
		Tab_Item_Pos = 19,
		Tab_Item_Val_deprecated = 20,
		ParaStyle = 21,
		numPr =  22,
		numPr_lvl =  23,
		numPr_id =  24,
		WidowControl = 25,
		pPr_rPr = 26,
		pBdr = 27,
		Spacing_BeforeAuto = 28,
		Spacing_AfterAuto = 29,
		FramePr = 30,
		SectPr = 31,
		numPr_Ins =  32,
		pPrChange = 33,
		outlineLvl = 34,
		Tab_Item_Leader = 35,
		Ind_LeftTwips = 36,
		Ind_RightTwips = 37,
		Ind_FirstLineTwips = 38,
		Spacing_LineTwips = 39,
		Spacing_BeforeTwips = 40,
		Spacing_AfterTwips = 41,
		Tab_Item_PosTwips = 42,
		Tab_Item_Val = 43,
		SuppressLineNumbers = 44
	};}
	namespace c_oSerProp_rPrType{enum c_oSerProp_rPrType
	{
		Bold = 0,
		Italic = 1,
		Underline = 2,
		Strikeout = 3,
		FontAscii = 4,
		FontHAnsi = 5,
		FontAE = 6,
		FontCS = 7,
		FontSize = 8,
		Color = 9,
		VertAlign = 10,
		HighLight = 11,
		HighLightTyped = 12,
		RStyle = 13,
		Spacing = 14,
		DStrikeout = 15,
		Caps = 16,
		SmallCaps = 17,
		Position = 18,
		FontHint = 19,
		BoldCs = 20,
		ItalicCs = 21,
		FontSizeCs = 22,
		Cs = 23,
		Rtl = 24,
		Lang = 25,
		LangBidi = 26,
		LangEA = 27,
		ColorTheme = 28,
		Shd = 29,
		Vanish = 30,
		TextOutline = 31,
		TextFill = 32,
		Del = 33,
		Ins = 34,
		rPrChange = 35,
		MoveFrom = 36,
		MoveTo = 37,
		SpacingTwips = 38,
		PositionHps = 39,
		FontAsciiTheme = 40,
		FontHAnsiTheme = 41,
		FontAETheme = 42,
		FontCSTheme = 43,
		CompressText = 44,
		SnapToGrid = 45,
		Ligatures = 46,
		NumSpacing = 47,
		NumForm = 48,
		StylisticSets = 49,
		CntxtAlts = 50,
		ShadowExt = 51,
		Reflection = 52,
		Glow = 53,
		Props3d = 54,
		Scene3d = 55
	};}
	namespace c_oSerProp_rowPrType{enum c_oSerProp_rowPrType
	{
		CantSplit = 0,
		GridAfter = 1,
		GridBefore = 2,
		Jc = 3,
		TableCellSpacing = 4,
		Height = 5,
		Height_Rule = 6,
		Height_Value = 7,
		WAfter = 8,
		WBefore = 9,
		WAfterBefore_W = 10,
		WAfterBefore_Type = 11,
		After = 12,
		Before = 13,
		TableHeader = 14,
		Del = 15,
		Ins = 16,
		trPrChange = 17,
		TableCellSpacingTwips = 18,
		Height_ValueTwips = 19
	};}
	namespace c_oSerProp_cellPrType{enum c_oSerProp_cellPrType
	{
		GridSpan = 0,
		Shd = 1,
		TableCellBorders = 2,
		TableCellW = 3,
		VAlign = 4,
		VMerge = 5,
		CellMar = 6,
		CellDel = 7,
		CellIns = 8,
		CellMerge = 9,
		tcPrChange = 10,
		textDirection = 11,
		hideMark = 12,
		noWrap = 13,
		tcFitText = 14,
		HMerge = 15
	};}
	namespace c_oSerProp_secPrType{enum c_oSerProp_secPrType
	{
		pgSz = 0,
		pgMar = 1,
		setting = 2,
		headers = 3,
		footers = 4,
		hdrftrelem = 5,
		pageNumType = 6,
		sectPrChange = 7,
		cols = 8,
		pgBorders = 9,
		footnotePr = 10,
		endnotePr = 11,
		rtlGutter = 12,
		lnNumType = 13
	};}
	namespace c_oSerProp_secPrSettingsType{enum c_oSerProp_secPrSettingsType
	{
		titlePg = 0,
		EvenAndOddHeaders = 1,
		SectionType = 2
	};}
	namespace c_oSerProp_secPrPageNumType{enum c_oSerProp_secPrPageNumType
	{
		start = 0
	};}
	namespace c_oSerProp_secPrLineNumType{enum c_oSerProp_secPrLineNumType
	{
		CountBy = 0,
		Distance = 1,
		Restart = 2,
		Start = 3
	};}
	namespace c_oSerParType{enum c_oSerParType
	{
		Par				= 0,
		pPr				= 1,
		Content			= 2,
		Table			= 3,
		sectPr			=  4,
		Run				= 5,
		CommentStart	= 6,
		CommentEnd		= 7,
		OMathPara		= 8,
		OMath			= 9,
		Hyperlink		= 10,
		FldSimple		= 11,
		Del				= 12,
		Ins				= 13,
		Background		= 14,
		Sdt				= 15,
		MoveFrom		= 16,
		MoveTo			= 17,
		MoveFromRangeStart = 18,
		MoveFromRangeEnd = 19,
		MoveToRangeStart = 20,
		MoveToRangeEnd = 21,
		JsaProject = 22,
		BookmarkStart = 23,
		BookmarkEnd = 24,
		MRun = 25,
		AltChunk = 26,
		DocParts = 27,
		PermStart = 28,
		PermEnd = 29
	};}
	namespace c_oSerGlossary {
		enum c_oSerGlossary
		{
			DocPart = 0,
			DocPartPr = 1,
			DocPartBody = 2,
			Name = 3,
			Style = 4,
			Guid = 5,
			Description = 6,
			CategoryName = 7,
			CategoryGallery = 8,
			Types = 9,
			Type = 10,
			Behaviors = 11,
			Behavior = 12
	};}
	namespace c_oSerDocTableType{enum c_oSerDocTableType
	{
		tblPr = 0,
		tblGrid = 1,
		tblGrid_Item = 2,
		Content = 3,
		Row =  4,
		Row_Pr =  4,
		Row_Content =  5,
		Cell =  6,
		Cell_Pr =  7,
		Cell_Content =  8,
		tblGridChange = 9,
		Sdt = 10,
		BookmarkStart = 11,
		BookmarkEnd = 12,
		tblGrid_ItemTwips = 13,
		MoveFromRangeStart = 14,
		MoveFromRangeEnd = 15,
		MoveToRangeStart = 16,
		MoveToRangeEnd = 17
	};}
	namespace c_oSerRunType{enum c_oSerRunType
	{
		run = 0,
		rPr = 1,
		tab = 2,
		pagenum = 3,
		pagebreak = 4,
		linebreak = 5,
		image = 6,
		table = 7,
		Content = 8,
		fldstart_deprecated = 9,
		fldend_deprecated = 10,
		CommentReference = 11,
		pptxDrawing = 12,
        object = 14,
		delText = 15,
		del = 16,
		ins = 17,
		columnbreak = 18,
		cr = 19,
		noBreakHyphen = 20,
		softHyphen = 21,
		separator = 22,
		continuationSeparator = 23,
		footnoteRef = 24,
		endnoteRef = 25,
		footnoteReference = 26,
		endnoteReference = 27,
		arPr = 28,
		fldChar = 29,
		instrText = 30,
		delInstrText = 31,
		linebreakClearAll = 32,
		linebreakClearLeft = 33,
		linebreakClearRight = 34
	};}
	namespace c_oSerVbaProjectTypes{enum c_oSerVbaProjectType
	{
		Name = 0
	};}
    namespace c_oSerBackgroundType{enum c_oSerBackgroundType
	{
		Color		= 0,
		ColorTheme	= 1,
		pptxDrawing = 2
	};}
	namespace c_oSerImageType{enum c_oSerImageType
	{
		MediaId = 0,
		Type = 1,
		Width = 2,
		Height = 3,
		X = 4,
		Y = 5,
		Page = 6,
		Padding = 7
	};}
	namespace c_oSerImageType2{enum c_oSerImageType2
	{
		Type = 0,
		PptxData = 1,
		AllowOverlap = 2,
		BehindDoc = 3,
		DistB = 4,
		DistL = 5,
		DistR = 6,
		DistT = 7,
		Hidden = 8,
		LayoutInCell = 9,
		Locked = 10,
		RelativeHeight = 11,
		BSimplePos = 12,
		EffectExtent = 13,
		Extent = 14,
		PositionH = 15,
		PositionV = 16,
		SimplePos = 17,
		WrapNone = 18,
		WrapSquare = 19,
		WrapThrough = 20,
		WrapTight = 21,
		WrapTopAndBottom = 22,
		Chart = 25,
		CachedImage = 26,
		SizeRelH = 27,
		SizeRelV = 28,
		Embedded = 29,
		GraphicFramePr = 30,
		DocPr = 31,
		DistBEmu = 32,
		DistLEmu = 33,
		DistREmu = 34,
		DistTEmu = 35,
		ChartEx = 36
	};}
	namespace c_oSerEffectExtent{enum c_oSerEffectExtent
	{
		Left = 0,
		Top = 1,
		Right = 2,
		Bottom = 3,
		LeftEmu = 4,
		TopEmu = 5,
		RightEmu = 6,
		BottomEmu = 7
	};}
	namespace c_oSerExtent{enum c_oSerExtent
	{
		Cx = 0,
		Cy = 1,
		CxEmu = 2,
		CyEmu = 3
	};}
	namespace c_oSerPosHV{enum c_oSerPosHV
	{
		RelativeFrom	= 0,
		Align			= 1,
		PosOffset		= 2,
		PctOffset		= 3,
		PosOffsetEmu	= 4
	};}
	namespace c_oSerSizeRelHV{enum c_oSerSizeRelHV
	{
		RelativeFrom	= 0,
		Pct				= 1
	};}
	namespace c_oSerSimplePos{enum c_oSerSimplePos
	{
		X = 0,
		Y = 1,
		XEmu = 2,
		YEmu = 3
	};}
	namespace c_oSerWrapSquare{enum c_oSerWrapSquare
	{
		DistL = 0,
		DistT = 1,
		DistR = 2,
		DistB = 3,
		WrapText = 4,
		EffectExtent = 5,
		DistLEmu = 6,
		DistTEmu = 7,
		DistREmu = 8,
		DistBEmu = 9
	};}
	namespace c_oSerWrapThroughTight{enum c_oSerWrapThroughTight
	{
		DistL = 0,
		DistR = 1,
		WrapText = 2,
		WrapPolygon = 3,
		DistLEmu = 4,
		DistREmu = 5
	};}
	namespace c_oSerWrapTopBottom{enum c_oSerWrapTopBottom
	{
		DistT = 0,
		DistB = 1,
		EffectExtent = 2,
		DistTEmu = 3,
		DistBEmu = 4
	};}
	namespace c_oSerWrapPolygon{enum c_oSerWrapPolygon
	{
		Edited = 0,
		Start = 1,
		ALineTo = 2,
		LineTo = 3
	};}
	namespace c_oSerPoint2D{enum c_oSerPoint2D
	{
		X = 0,
		Y = 1,
		XEmu = 2,
		YEmu = 3
	};}
	namespace c_oSerBorderType{enum c_oSerBorderType
	{
		Color = 0,
		Space = 1,
		Size = 2,
		Value = 3,
		ColorTheme = 4,
		SpacePoint = 5,
		Size8Point = 6
	};}
	namespace c_oSerShdType{enum c_oSerShdType
	{
		Value = 0,
		Color = 1,
		ColorTheme = 2,
		Fill = 3,
		FillTheme = 4
	};}
	namespace c_oSerPaddingType{enum c_oSerPaddingType
	{
		left = 0,
		top = 1,
		right = 2,
		bottom = 3,
		leftTwips = 4,
		topTwips = 5,
		rightTwips = 6,
		bottomTwips = 7
	};}
	namespace c_oSerMarginsType{enum c_oSerMarginsType
	{
		left = 0,
		top = 1,
		right = 2,
		bottom = 3
	};}
	namespace c_oSerBordersType{enum c_oSerBordersType
	{
		left = 0,
		top = 1,
		right = 2,
		bottom = 3,
		insideV = 4,
		insideH = 5,
		start = 6,
		end = 7,
		tl2br = 8,
		tr2bl = 9,
		bar = 10,
		between = 11
	};}
	namespace c_oSerWidthType{enum c_oSerWidthType
	{
		Type = 0,
		W = 1,
		WDocx = 2
	};}
	namespace c_oSer_pgSzType{enum c_oSer_pgSzType
	{
		W = 0,
		H = 1,
		Orientation = 2,
		WTwips = 3,
		HTwips = 4
	};}
	namespace c_oSer_pgMarType{enum c_oSer_pgMarType
	{
		Left = 0,
		Top = 1,
		Right = 2,
		Bottom = 3,
		Header = 4,
		Footer = 5,
		LeftTwips = 6,
		TopTwips = 7,
		RightTwips = 8,
		BottomTwips = 9,
		HeaderTwips = 10,
		FooterTwips = 11,
		GutterTwips = 12
	};}
	namespace c_oSer_ColorType{enum c_oSer_ColorType
	{
		None = 0,
		Auto = 1
	};}
	namespace c_oSer_CommentsType{enum c_oSer_CommentsType
	{
		Comment = 0,
		Id = 1,
		Initials = 2,
		UserName = 3,
		UserId = 4,
		Date = 5,
		Text = 6,
		QuoteText = 7,
		Solved = 8,
		Replies = 9,
		OOData = 10,
		DurableId = 11,
		ProviderId = 12,
		CommentContent = 13,
		DateUtc = 14,
		UserData = 15
	};}
	namespace c_oSer_SettingsType{enum c_oSer_SettingsType
	{
		ClrSchemeMapping = 0,
		DefaultTabStop = 1,
		MathPr = 2,
		TrackRevisions = 3,
		FootnotePr = 4,
		EndnotePr = 5,
		SdtGlobalColor = 6,
		SdtGlobalShowHighlight = 7,
		Compat = 8,
		DefaultTabStopTwips = 9,
		DecimalSymbol = 10,
		ListSeparator = 11,
		GutterAtTop = 12,
		MirrorMargins = 13,
		PrintTwoOnOne = 14,
		BookFoldPrinting = 15,
		BookFoldPrintingSheets = 16,
		BookFoldRevPrinting = 17,
		SpecialFormsHighlight = 18,
		DocumentProtection = 19,
		WriteProtection = 20

	};}
	namespace c_oSer_MathPrType{enum c_oSer_SettingsType
	{
		BrkBin = 0,
		BrkBinSub = 1,
		DefJc = 2,
		DispDef = 3,
		InterSp = 4,
		IntLim = 5,
		IntraSp = 6,
		LMargin = 7,
		MathFont = 8,
		NaryLim = 9,
		PostSp = 10,
		PreSp = 11,
		RMargin = 12,
		SmallFrac = 13,
		WrapIndent = 14,
		WrapRight = 15
	};}
	namespace c_oSer_ClrSchemeMappingType{enum c_oSer_ClrSchemeMappingType
	{
		Accent1 = 0,
		Accent2 = 1,
		Accent3 = 2,
		Accent4 = 3,
		Accent5 = 4,
		Accent6 = 5,
		Bg1 = 6,
		Bg2 = 7,
		FollowedHyperlink = 8,
		Hyperlink = 9,
		T1 = 10,
		T2 = 11
	};}
	namespace c_oSer_OMathBottomNodesType{enum c_oSer_OMathBottomNodesType
	{		
		Aln = 0,
		AlnScr = 1,
		ArgSz = 2,
		BaseJc = 3,
		BegChr = 4,
		Brk = 5,
		CGp = 6,
		CGpRule = 7,
		Chr = 8,
		Count = 9,
		CSp = 10,
		CtrlPr = 11,
		DegHide = 12,
		Diff = 13,
		EndChr = 14,
		Grow = 15,
		HideBot = 16,
		HideLeft = 17,
		HideRight = 18,
		HideTop = 19,
		MJc = 20,
		LimLoc = 21,
		Lit = 22,
		MaxDist = 23,
		McJc = 24,
		Mcs = 25,
		NoBreak = 26,
		Nor = 27,
		ObjDist = 28,
		OpEmu = 29,
		PlcHide = 30,
		Pos = 31,
		RSp = 32,
		RSpRule = 33,
		Scr = 34,
		SepChr = 35,
		Show = 36,
		Shp = 37,
		StrikeBLTR = 38,
		StrikeH = 39,
		StrikeTLBR = 40,
		StrikeV = 41,
		Sty = 42,
		SubHide = 43,
		SupHide = 44,
		Transp = 45,
		Type = 46,
		VertJc = 47,
		ZeroAsc = 48,
		ZeroDesc = 49,
		ZeroWid = 50,	
		Column = 51,
		Row = 52
	};}
	namespace c_oSer_OMathBottomNodesValType{enum c_oSer_OMathBottomNodesValType
	{		
		Val = 0,
		AlnAt = 1,
		ValTwips = 2
	};}
	namespace c_oSer_OMathContentType{enum c_oSer_OMathContentType
	{		
		Acc = 0,
		AccPr = 1,
		ArgPr = 2,
		Bar = 3,
		BarPr = 4,
		BorderBox = 5,
		BorderBoxPr = 6,
		Box = 7,
		BoxPr = 8,
		Deg = 9,
		Den = 10,
		Delimiter = 11,
		DelimiterPr = 12,
		Element = 13,
		EqArr = 14,
		EqArrPr = 15,
		FName = 16,
		Fraction = 17,
		FPr = 18,
		Func = 19,
		FuncPr = 20,
		GroupChr = 21,
		GroupChrPr = 22,
		Lim = 23,
		LimLow = 24,
		LimLowPr = 25,
		LimUpp = 26,
		LimUppPr = 27,
		Matrix = 28,
		MathPr = 29,
		Mc = 30,
		McPr = 31,		
		MPr = 32,
		Mr = 33,
		Nary = 34,
		NaryPr = 35,
		Num = 36,
		OMath = 37,
		OMathPara = 38,
		OMathParaPr = 39,
		Phant = 40,
		PhantPr = 41,
		MRun = 42,
		Rad = 43,
		RadPr = 44,
		RPr = 45,
		MRPr = 46,
		SPre = 47,
		SPrePr = 48,
		SSub = 49,
		SSubPr = 50,
		SSubSup = 51,
		SSubSupPr = 52,
		SSup = 53,
		SSupPr = 54,
		Sub = 55,
		Sup = 56,
		MText = 57,
		CtrlPr = 58,
		pagebreak = 59,
		linebreak = 60,
		Run = 61,
		Ins = 62,
		Del = 63,
		columnbreak = 64,
		ARPr = 65,
		BookmarkStart = 66,
		BookmarkEnd = 67,
		MoveFromRangeStart = 68,
		MoveFromRangeEnd = 69,
		MoveToRangeStart = 70,
		MoveToRangeEnd = 71
	};}
	namespace c_oSer_FramePrType{ enum c_oSer_FramePrType
	{		
		DropCap = 0,
		H = 1,
		HAnchor = 2,
		HRule = 3,
		HSpace = 4,
		Lines = 5,
		VAnchor = 6,
		VSpace = 7,
		W = 8,
		Wrap = 9,
		X = 10,
		XAlign = 11,
		Y = 12,
		YAlign = 13
	};}
	namespace c_oSer_HyperlinkType{ enum c_oSer_HyperlinkType
	{
		Content = 0,
		Link = 1,
		Anchor = 2,
		Tooltip = 3,
		History = 4,
		DocLocation = 5,
		TgtFrame = 6
	};}
	namespace c_oSer_FldSimpleType{ enum c_oSer_FldSimpleType
	{
		Content = 0,
		Instr = 1,
		FFData = 2,
		CharType = 3
	};}
	namespace c_oSer_ColorThemeType{ enum c_oSer_ColorThemeType
	{
		Auto = 0,
		Color = 1,
		Tint = 2,
		Shade = 3
	};}
	namespace c_oSerProp_RevisionType{enum c_oSerProp_RevisionType
	{
		Author = 0,
		Date = 1,
		Id = 2,
        UserId = 3,
        Content = 4,
        VMerge = 5,
        VMergeOrigin = 6,
        pPrChange = 7,
        rPrChange = 8,
        sectPrChange = 9,
        tblGridChange = 10,
        tblPrChange = 11,
        tcPrChange = 12,
		trPrChange = 13,
		ContentRun = 14
	};}
	namespace c_oSerProp_Columns{enum c_oSerProp_Columns
	{
		EqualWidth = 0,
		Num = 1,
		Sep = 2,
		Space = 3,
		Column = 4,
		ColumnSpace = 5,
		ColumnW = 6
	};}
	namespace c_oSerPageBorders{enum c_oSerPageBorders
	{
		Display = 0,
		OffsetFrom = 1,
		ZOrder = 2,
		Bottom = 3,
		Left = 4,
		Right = 5,
		Top = 6,
		Color = 7,
		Frame = 8,
		Id = 9,
		Shadow = 10,
		Space = 11,
		Sz = 12,
		ColorTheme = 13,
		Val = 16
	};}
	namespace c_oSerGraphicFramePr{enum c_oSerGraphicFramePr
	{
		NoChangeAspect = 0,
		NoDrilldown = 1,
		NoGrp = 2,
		NoMove	= 3,
		NoResize = 4,
		NoSelect = 5
	};}
	namespace c_oSerNotes{enum c_oSerNotes
	{
		Note = 0,
		NoteType = 1,
		NoteId = 2,
		NoteContent = 3,
		RefCustomMarkFollows = 4,
		RefId = 5,
		PrFmt = 6,
		PrRestart = 7,
		PrStart = 8,
		PrFntPos = 9,
		PrEndPos = 10,
		PrRef = 11
	};}
	namespace c_oSerCustoms {enum c_oSerCustoms
	{
		Custom = 0,
		ItemId = 1,
		Uri = 2,
		Content = 3,
		ContentA = 4
	};}
	namespace c_oSerApp{enum c_oSerApp
	{
		Application = 0,
		AppVersion = 1
	};}
	namespace c_oSerDocPr{enum c_oSerDocPr
	{
		Id = 0,
		Name = 1,
		Hidden = 2,
		Title	= 3,
		Descr = 4,
		Form = 5
	};}
	namespace c_oSerEmbedded{enum c_oSerEmbedded
	{
		Type = 0,
		Data = 1,
		Program = 2
	};}
	namespace c_oSerSdt{enum c_oSerSdt
	{
		Pr = 0,
		EndPr = 1,
		Content = 2,
		Type = 3,
		Alias = 4,
		ComboBox = 5,
		LastValue = 6,
		SdtListItem = 7,
		DisplayText = 8,
		Value = 9,
		DataBinding = 10,
		PrefixMappings = 11,
		StoreItemID = 12,
		XPath = 13,
		PrDate = 14,
		FullDate = 15,
		Calendar = 16,
		DateFormat = 17,
		Lid = 18,
		StoreMappedDataAs = 19,
		DocPartList = 20,
		DocPartObj = 21,
		DocPartCategory = 22,
		DocPartGallery = 23,
		DocPartUnique = 24,
		DropDownList = 25,
		Id = 26,
		Label = 27,
		Lock = 28,
		PlaceHolder = 29,
		RPr = 30,
		ShowingPlcHdr = 31,
		TabIndex = 32,
		Tag = 33,
		Temporary = 34,
		MultiLine = 35,
		Appearance = 36,
		Color = 37,
		Checkbox = 38,
		CheckboxChecked = 39,
		CheckboxCheckedFont = 40,
		CheckboxCheckedVal = 41,
		CheckboxUncheckedFont = 42,
		CheckboxUncheckedVal = 43,
		FormPr = 44,
		FormPrKey = 45,
		FormPrLabel = 46,
		FormPrHelpText = 47,
		FormPrRequired = 48,
		CheckboxGroupKey = 59,
		TextFormPr = 50,
		TextFormPrComb = 51,
		TextFormPrCombWidth = 52,
		TextFormPrCombSym = 53,
		TextFormPrCombFont = 54,
		TextFormPrMaxCharacters = 55,
		TextFormPrCombBorder = 56,
		TextFormPrAutoFit = 57,
		TextFormPrMultiLine = 58,
		TextFormPrFormat = 59,
		PictureFormPr = 60,
		PictureFormPrScaleFlag = 61,
		PictureFormPrLockProportions = 62,
		PictureFormPrRespectBorders = 63,
		PictureFormPrShiftX = 64,
		PictureFormPrShiftY = 65,
		FormPrBorder = 70,
		FormPrShd = 71,
		TextFormPrCombWRule = 72,
		TextFormPrFormatType    = 80,
		TextFormPrFormatVal     = 81,
		TextFormPrFormatSymbols = 82,	
		ComplexFormPr     = 90,
		ComplexFormPrType = 91,
		OformMaster = 92
	};}
	namespace c_oSerFFData{enum c_oSerFFData
	{
		CalcOnExit = 0,
		CheckBox = 1,
		DDList = 2,
		Enabled = 3,
		EntryMacro = 4,
		ExitMacro = 5,
		HelpText = 6,
		Label = 7,
		Name = 8,
		StatusText = 9,
		TabIndex = 10,
		TextInput = 11,
		CBChecked = 12,
		CBDefault = 13,
		CBSize = 14,
		CBSizeAuto = 15,
		DLDefault = 16,
		DLResult = 17,
		DLListEntry = 18,
		HTType = 19,
		HTVal = 20,
		TIDefault = 21,
		TIFormat = 22,
		TIMaxLength = 23,
		TIType = 24,
	};}
	namespace c_oSerMoveRange{enum c_oSerMoveRange
	{
		Author = 0,
		ColFirst = 1,
		ColLast = 2,
		Date = 3,
		DisplacedByCustomXml = 4,
		Id = 5,
		Name = 6,
		UserId = 7
	};}
	namespace c_oSerBookmark{enum c_oSerBookmark
	{
		Id = 0,
		Name = 1,
		DisplacedByCustomXml = 2,
		ColFirst = 3,
		ColLast = 4
	};}
	namespace c_oSerPermission {enum c_oSerPermission
	{
		Id = 0,
		DisplacedByCustomXml = 1,
		ColFirst = 2,
		ColLast = 3,
		Ed = 4,
		EdGroup = 5
	};}
	namespace c_oSerCompat{enum c_oSerCompat
	{
		CompatSetting = 0,
		CompatName = 1,
		CompatUri = 2,
		CompatValue = 3,
		Flags1 = 4,
		Flags2 = 5,
		Flags3 = 6
	};}
	namespace c_oDocProtect {
		enum c_oDocProtect
		{
			AlgorithmName = 0,
			Edit = 1,
			Enforcment = 2,
			Formatting = 3,
			HashValue = 4,
			SaltValue = 5,
			SpinCount = 6,
			
			AlgIdExt = 7,
			AlgIdExtSource = 8,
			CryptAlgorithmClass = 9,
			CryptAlgorithmSid = 10,
			CryptAlgorithmType = 11,
			CryptProvider = 12,
			CryptProviderType = 13,
			CryptProviderTypeExt = 14,
			CryptProviderTypeExtSource = 15
		};
	}
	namespace c_oWriteProtect {
		enum c_oWriteProtect
		{
			AlgorithmName = 0,
			Recommended = 1,
			HashValue = 2,
			SaltValue = 3,
			SpinCount = 4,
			
			AlgIdExt = 7,
			AlgIdExtSource = 8,
			CryptAlgorithmClass = 9,
			CryptAlgorithmSid = 10,
			CryptAlgorithmType = 11,
			CryptProvider = 12,
			CryptProviderType = 13,
			CryptProviderTypeExt = 14,
			CryptProviderTypeExtSource = 15
		};
	}
}
