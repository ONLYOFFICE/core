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
#include "SimpleTypes_Base.h"

namespace SimpleTypes
{
	namespace Spreadsheet
	{
		enum ETableType
		{
			typeQueryTable	=  0,
			typeWorksheet	=  1,
			typeXml			=  2,
		};

		DEFINE_SIMPLE_TYPE(CTableType, ETableType, typeWorksheet)

		enum EDatabaseSourceType
		{
			typeODBCBasedSource = 1,
			typeDAOBasedSource = 2,
			typeFileBasedDatabaseSource = 3,
			typeWebQuery = 4,
			typeOLEDBBasedSource = 5,
			typeTextBasedSource = 6,
			typeADORecordSet = 7,
			typeDSP
		};

		enum EExternalConnectionType
		{
			extConnTypeGeneral	= 0,
			extConnTypeText		= 1,
			extConnTypeMDY		= 2,
			extConnTypeDMY		= 3,
			extConnTypeYMD		= 4,
			extConnTypeMYD		= 5,
			extConnTypeDYM		= 6,
			extConnTypeYDM		= 7,
			extConnTypeSkip		= 8,
			extConnTypeEMD		= 9
		};

		DEFINE_SIMPLE_TYPE(CExternalConnectionType, EExternalConnectionType, extConnTypeGeneral)

		enum ECredMethod
		{
			integrated_method	= 0,
			none_method			= 1,
			prompt_method		= 2,
			stored_method		= 3
		};

		DEFINE_SIMPLE_TYPE(CCredMethod, ECredMethod, integrated_method)

		enum EVisibleType
		{
			visibleHidden		=  0,
			visibleVeryHidden	=  1,
			visibleVisible		=  2,
		};

		DEFINE_SIMPLE_TYPE(CVisibleType, EVisibleType, visibleVisible)

		enum EHtmlFormat
		{
			htmlNone =  0,
			htmlRtf	=  1,
			htmlAll	=  2
		};

		DEFINE_SIMPLE_TYPE(CHtmlFormat, EHtmlFormat, htmlNone)

		enum EParameterType
		{
			parameterPrompt =  0,
			parameterValue	=  1,
			parameterCell	=  2
		};

		DEFINE_SIMPLE_TYPE(CParameterType, EParameterType, parameterPrompt)

		enum EQualifier
		{
			doubleQuote =  0,
			singleQuote	=  1,
			noneQuote	=  2
		};

		DEFINE_SIMPLE_TYPE(CQualifier, EQualifier, doubleQuote)

		enum EFileType
		{
			fileTypeMac		=  0,
			fileTypeWin		=  1,
			fileTypeDos		=  2,
			fileTypeLin		=  3,
			fileTypeOther	=  4
		};

		DEFINE_SIMPLE_TYPE(CFileType, EFileType, fileTypeWin)

		enum EPhoneticAlignmentType
		{
			phoneticalignmentCenter			=  0,
			phoneticalignmentDistributed	=  1,
			phoneticalignmentLeft			=  2,
			phoneticalignmentNoControl 		=  3,
		};

		DEFINE_SIMPLE_TYPE(CPhoneticAlignment, EPhoneticAlignmentType, phoneticalignmentNoControl)

		enum EPhoneticTypeType
		{
			phonetictypeFullwidthKatakana			=  0,
			phonetictypeHalfwidthKatakana			=  1,
			phonetictypeHiragana					=  2,
			phonetictypeNoConversion 				=  3,
		};

		DEFINE_SIMPLE_TYPE(CPhoneticType, EPhoneticTypeType, phonetictypeNoConversion)

		enum EFontCharset
		{
			fontcharsetANSI        =   0, // 00
			fontcharsetDefault     =   1, // 01
			fontcharsetSymbol      =   2, // 02
			fontcharsetMacintosh   =  77, // 4D
			fontcharsetShitJIS     = 128, // 80
			fontcharsetHangeul     = 129, // 81
			fontcharsetJohab       = 130, // 82
			fontcharsetGB2313      = 134, // 86
			fontcharsetChineseBig5 = 136, // 88
			fontcharsetGreek       = 161, // A1
			fontcharsetTurkish     = 162, // A2
			fontcharsetVietnamese  = 163, // A3
			fontcharsetHebrew      = 177, // B1
			fontcharsetArabic      = 178, // B2
			fontcharsetBaltic      = 186, // BA
			fontcharsetRussian     = 204, // CC
			fontcharsetThai        = 222, // DE
			fontcharsetEastEurope  = 238, // EE
			fontcharsetOEM         = 255, // FF
		};

		DEFINE_SIMPLE_TYPE_START(CFontCharset, EFontCharset, fontcharsetANSI)
		public:
			std::wstring       ToHexString  () const;
		};

		enum EThemeColor
		{
			themecolorLight1            =  0,
			themecolorDark1				=  1,
			themecolorLight2            =  2,
			themecolorDark2				=  3,
			themecolorAccent1           =  4,
			themecolorAccent2           =  5,
			themecolorAccent3           =  6,
			themecolorAccent4           =  7,
			themecolorAccent5           =  8,
			themecolorAccent6           =  9,
			themecolorHyperlink         = 10,
			themecolorFollowedHyperlink = 11
		};

		DEFINE_SIMPLE_TYPE(CThemeColor, EThemeColor, themecolorDark1)

		class CHexColor
		{
		public:
			CHexColor();
			CHexColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
			CHexColor(std::wstring & cwsValue);

			virtual void FromString(const std::wstring &sValue);

			CHexColor(const std::wstring& wsStr);
			const CHexColor &operator =(const std::wstring& wsStr);
			virtual std::wstring   ToString  () const;
			void Set_R(unsigned char R);
			void Set_G(unsigned char G);
			void Set_B(unsigned char B);
			void Set_A(unsigned char A);
			unsigned char Get_R() const;
			unsigned char Get_G() const;
			unsigned char Get_B() const;
			unsigned char Get_A() const;
			int ToInt() const;
			void FromInt(int nColor);

		private:
			void Parse(const std::wstring sValue);
			int	HexToInt(int nHex);

		private:
			unsigned char m_unA;
			unsigned char m_unR;
			unsigned char m_unG;
			unsigned char m_unB;
		};

		enum EFontFamily
		{
			fontfamilyNotApplicable		=  0,
			fontfamilyRoman				=  1,
			fontfamilySwiss				=  2,
			fontfamilyModern			=  3,
			fontfamilyScript			=  4,
			fontfamilyDecorative		=  5,
		};

		DEFINE_SIMPLE_TYPE_START(CFontFamily, EFontFamily, fontfamilyNotApplicable)
		public:
			std::wstring ToStringWord() const;
		};

		enum EFontScheme
		{
			fontschemeMajor		=  0,
			fontschemeMinor		=  1,
			fontschemeNone		=  2
		};

		DEFINE_SIMPLE_TYPE(CFontScheme, EFontScheme, fontschemeNone)

		enum EUnderline
		{
			underlineDouble				=  0,
			underlineDoubleAccounting	=  1,
			underlineNone				=  2,
			underlineSingle				=  3,
			underlineSingleAccounting	=  4
		};

		DEFINE_SIMPLE_TYPE(CUnderline, EUnderline, underlineSingle)

		enum EBorderStyle
		{
			borderstyleDashDot			=  0,
			borderstyleDashDotDot		=  1,
			borderstyleDashed			=  2,
			borderstyleDotted			=  3,
			borderstyleDouble			=  4,
			borderstyleHair				=  5,
			borderstyleMedium			=  6,
			borderstyleMediumDashDot	=  7,
			borderstyleMediumDashDotDot	=  8,
			borderstyleMediumDashed		=  9,
			borderstyleNone				= 10,
			borderstyleSlantDashDot		= 11,
			borderstyleThick			= 12,
			borderstyleThin				= 13
		};

		DEFINE_SIMPLE_TYPE(CBorderStyle, EBorderStyle, borderstyleNone)

		enum EHorizontalAlignment
		{
			horizontalalignmentCenter			=  0,
			horizontalalignmentContinuous		=  1,
			horizontalalignmentDistributed		=  2,
			horizontalalignmentFill				=  3,
			horizontalalignmentGeneral			=  4,
			horizontalalignmentJustify			=  5,
			horizontalalignmentLeft				=  6,
			horizontalalignmentRight			=  7,
			horizontalalignmentCenterContinuous	=  8
		};

		DEFINE_SIMPLE_TYPE_START(CHorizontalAlignment, EHorizontalAlignment, horizontalalignmentGeneral)
		public:
			std::wstring ToVmlString() const;
		};

		enum EVerticalAlignment
		{
			verticalalignmentBottom				=  0,
			verticalalignmentCenter				=  1,
			verticalalignmentDistributed		=  2,
			verticalalignmentJustify			=  3,
			verticalalignmentTop				=  4
		};

		DEFINE_SIMPLE_TYPE_START(CVerticalAlignment, EVerticalAlignment, verticalalignmentBottom)
		public:
			std::wstring ToVmlString() const;
		};

		enum EGradientType
		{
			gradienttypeLine				=  0,
			gradienttypePath				=  1
		};

		DEFINE_SIMPLE_TYPE(CGradientType, EGradientType, gradienttypeLine)

		enum EPatternType
		{
			patterntypeDarkDown				=  0,
			patterntypeDarkGray				=  1,
			patterntypeDarkGrid				=  2,
			patterntypeDarkHorizontal		=  3,
			patterntypeDarkTrellis			=  4,
			patterntypeDarkUp				=  5,
			patterntypeDarkVertical			=  6,
			patterntypeGray0625				=  7,
			patterntypeGray125				=  8,
			patterntypeLightDown			=  9,
			patterntypeLightGray			= 10,
			patterntypeLightGrid			= 11,
			patterntypeLightHorizontal		= 12,
			patterntypeLightTrellis			= 13,
			patterntypeLightUp			= 14,
			patterntypeLightVertical	= 15,
			patterntypeMediumGray		= 16,
			patterntypeNone					= 17,
			patterntypeSolid				= 18
		};

		DEFINE_SIMPLE_TYPE(CPatternType, EPatternType, patterntypeNone)

		enum ETableStyleType
		{
			tablestyletypeBlankRow			=  0,
			tablestyletypeFirstColumn			=  1,
			tablestyletypeFirstColumnStripe			=  2,
			tablestyletypeFirstColumnSubheading			=  3,
			tablestyletypeFirstHeaderCell			=  4,
			tablestyletypeFirstRowStripe			=  5,
			tablestyletypeFirstRowSubheading			=  6,
			tablestyletypeFirstSubtotalColumn			=  7,
			tablestyletypeFirstSubtotalRow			=  8,
			tablestyletypeFirstTotalCell			=  9,
			tablestyletypeHeaderRow			=  10,
			tablestyletypeLastColumn			=  11,
			tablestyletypeLastHeaderCell			=  12,
			tablestyletypeLastTotalCell			=  13,
			tablestyletypePageFieldLabels			=  14,
			tablestyletypePageFieldValues			=  15,
			tablestyletypeSecondColumnStripe			=  16,
			tablestyletypeSecondColumnSubheading			=  17,
			tablestyletypeSecondRowStripe			=  18,
			tablestyletypeSecondRowSubheading			=  19,
			tablestyletypeSecondSubtotalColumn			=  20,
			tablestyletypeSecondSubtotalRow			=  21,
			tablestyletypeThirdColumnSubheading			=  22,
			tablestyletypeThirdRowSubheading			=  23,
			tablestyletypeThirdSubtotalColumn			=  24,
			tablestyletypeThirdSubtotalRow			=  25,
			tablestyletypeTotalRow			=  26,
			tablestyletypeWholeTable			=  27
		};

		DEFINE_SIMPLE_TYPE(CTableStyleType, ETableStyleType, tablestyletypeBlankRow)

		enum ECellFormulaType
		{
			cellformulatypeArray			=  0,
			cellformulatypeDataTable		=  1,
			cellformulatypeNormal			=  2,
			cellformulatypeShared			=  3
		};

		DEFINE_SIMPLE_TYPE(CCellFormulaType, ECellFormulaType, cellformulatypeNormal)

		enum EUpdateLinksType
		{
			updatelinksAlways			=  0,
			updatelinksNever			=  1,
			updatelinksUserSet			=  2
		};

		DEFINE_SIMPLE_TYPE(CUpdateLinksType, EUpdateLinksType, updatelinksAlways)

		enum ECellTypeType
		{
			celltypeBool			=  0,
			celltypeDate			=  1,
			celltypeError			=  2,
			celltypeInlineStr		=  3,
			celltypeNumber			=  4,
			celltypeSharedString	=  5,
			celltypeStr				=  6,
			//--------------- for our using
			celltypePercentage		= 7,
			celltypeScientific		= 8,
			celltypeFraction		= 9,
			celltypeTime			= 10,
			celltypeCurrency		= 11,
			celltypeDateTime		= 12
		};

		DEFINE_SIMPLE_TYPE(CCellTypeType, ECellTypeType, celltypeNumber)

		enum ECellAnchorType
		{
			cellanchorAbsolute			=  0,
			cellanchorOneCell			=  1,
			cellanchorTwoCell			=  2
		};

		DEFINE_SIMPLE_TYPE(CCellAnchorType, ECellAnchorType, cellanchorTwoCell)

		enum ESheetViewType
		{
			sheetviewNormal				=  0,
			sheetviewPageBreakPreview	=  1,
			sheetviewPageLayout			=  2
		};

		DEFINE_SIMPLE_TYPE(CSheetViewType, ESheetViewType, sheetviewNormal)

		enum EChartLegendPos
		{
			chartlegendposLeft		=  0,
			chartlegendposTop		=  1,
			chartlegendposRight		=  2,
			chartlegendposBottom	=  3,
			chartlegendposRightTop	=  4
		};

		DEFINE_SIMPLE_TYPE(CChartLegendPos, EChartLegendPos, chartlegendposTop)

		enum EChartHMode
		{
			charthmodeFactor	=  0,
			charthmodeEdge		=  1
		};

		DEFINE_SIMPLE_TYPE(CChartHMode, EChartHMode, charthmodeFactor)

		enum EChartLayoutTarget
		{
			chartlayouttargetInner	=  0,
			chartlayouttargetOuter	=  1
		};

		DEFINE_SIMPLE_TYPE(CChartLayoutTarget, EChartLayoutTarget, chartlayouttargetInner)

		enum EChartAxPos
		{
			chartaxposLeft	=  0,
			chartaxposTop	=  1,
			chartaxposRight	=  2,
			chartaxposBottom=  3
		};

		DEFINE_SIMPLE_TYPE(CChartAxPos, EChartAxPos, chartaxposLeft)

		enum EChartBarGrouping
		{
			chartbargroupingClustered	=  0,
			chartbargroupingPercentStacked	=  1,
			chartbargroupingStacked	=  2,
			chartbargroupingStandard=  3
		};

		DEFINE_SIMPLE_TYPE(CChartBarGrouping, EChartBarGrouping, chartbargroupingClustered)

		enum EChartBarDirection
		{
			chartbardirectionBar	=  0,
			chartbardirectionCol	=  1
		};

		DEFINE_SIMPLE_TYPE(CChartBarDirection, EChartBarDirection, chartbardirectionBar)

		enum EChartSymbol
		{
			chartsymbolCircle	=  0,
			chartsymbolDash	=  1,
			chartsymbolDiamond	=  2,
			chartsymbolDot	=  3,
			chartsymbolNone	=  4,
			chartsymbolPicture =  5,
			chartsymbolPlus	=  6,
			chartsymbolSquare	=  7,
			chartsymbolStare	=  8,
			chartsymbolStar	=  9,
			chartsymbolTriangle	=  10,
			chartsymbolX	=  11
		};

		DEFINE_SIMPLE_TYPE(CChartSymbol, EChartSymbol, chartsymbolNone)

		enum EPageSize
		{
			pagesizeLetterPaper	=  1,
			pagesizeLetterSmall	=  2,
			pagesizeTabloidPaper =  3,
			pagesizeLedgerPaper =  4,
			pagesizeLegalPaper =  5,
			pagesizeStatementPaper =  6,
			pagesizeExecutivePaper =  7,
			pagesizeA3Paper =  8,
			pagesizeA4Paper =  9,
			pagesizeA4SmallPaper =  10,
			pagesizeA5Paper =  11,
			pagesizeB4Paper =  12,
			pagesizeB5Paper =  13,
			pagesizeFolioPaper =  14,
			pagesizeQuartoPaper =  15,
			pagesizeStandardPaper1 =  16,
			pagesizeStandardPaper2 =  17,
			pagesizeNotePaper =  18,
			pagesize9Envelope =  19,
			pagesize10Envelope =  20,
			pagesize11Envelope =  21,
			pagesize12Envelope =  22,
			pagesize14Envelope =  23,
			pagesizeCPaper =  24,
			pagesizeDPaper =  25,
			pagesizeEPaper =  26,
			pagesizeDLEnvelope =  27,
			pagesizeC5Envelope =  28,
			pagesizeC3Envelope =  29,
			pagesizeC4Envelope =  30,
			pagesizeC6Envelope =  31,
			pagesizeC65Envelope =  32,
			pagesizeB4Envelope =  33,
			pagesizeB5Envelope =  34,
			pagesizeB6Envelope =  35,
			pagesizeItalyEnvelope =  36,
			pagesizeMonarchEnvelope =  37,
			pagesize6_3_4Envelope =  38,
			pagesizeUSStandardFanfold =  39,
			pagesizeGermanStandardFanfold =  40,
			pagesizeGermanLegalFanfold =  41,
			pagesizeISOB4 =  42,
			pagesizeJapaneseDoublePostcard =  43,
			pagesizeStandardPaper3 =  44,
			pagesizeStandardPaper4 =  45,
			pagesizeStandardPaper5 =  46,
			pagesizeInviteEnvelope =  47,
			pagesizeLetterExtraPaper =  50,
			pagesizeLegalExtraPaper =  51,
			pagesizeTabloidExtraPaper =  52,
			pagesizeA4ExtraPaper =  53,
			pagesizeLetterTransversePaper =  54,
			pagesizeA4TransversePaper =  55,
			pagesizeLetterExtraTransversePaper =  56,
			pagesizeSuperA_SuperA_A4Paper =  57,
			pagesizeSuperB_SuperB_A3Paper =  58,
			pagesizeLetterPlusPaper =  59,
			pagesizeA4PlusPaper =  60,
			pagesizeA5TransversePaper =  61,
			pagesizeJISB5TransversePaper =  62,
			pagesizeA3ExtraPaper =  63,
			pagesizeA5ExtraPaper =  64,
			pagesizeISOB5ExtraPaper =  65,
			pagesizeA2Paper =  66,
			pagesizeA3TransversePaper =  67,
			pagesizeA3ExtraTransversePaper =  68
		};

		DEFINE_SIMPLE_TYPE(CPageSize, EPageSize, pagesizeA4Paper)
		
		enum EPageUnits
		{
			mm		= 0,
			cm		= 1,
			inch	= 2,
			pt		= 3,
			px		= 4,
			emu		= 5

		};

		DEFINE_SIMPLE_TYPE(CPageUnits, EPageUnits, mm)

		enum ECellComments
		{
			cellcommentsNone = 0,
			cellcommentsAsDisplayed = 1,
			cellcommentsAtEnd = 2
		};


		DEFINE_SIMPLE_TYPE(CCellComments, ECellComments, cellcommentsNone)

		enum EPrintError
		{
			printerrorDisplayed  = 0,
			printerrorBlank = 1,
			printerrorDash  = 2,
			printerrorNA  = 3
		};

		DEFINE_SIMPLE_TYPE(CPrintError, EPrintError, printerrorDisplayed)

		enum EPageOrder
		{
			pageorderDownThenOver = 0,
			pageorderOverThenDown  = 1
		};

		DEFINE_SIMPLE_TYPE(CPageOrder, EPageOrder, pageorderDownThenOver)

		enum ETotalsRowFunction
		{
			totalrowfunctionAverage		=  1,
			totalrowfunctionCount		=  2,
			totalrowfunctionCountNums	=  3,
			totalrowfunctionCustom		=  4,
			totalrowfunctionMax			=  5,
			totalrowfunctionMin			=  6,
			totalrowfunctionNone		=  7,
			totalrowfunctionStdDev		=  8,
			totalrowfunctionSum			=  9,
			totalrowfunctionVar			=  10
		};

		DEFINE_SIMPLE_TYPE(CTotalsRowFunction, ETotalsRowFunction, totalrowfunctionNone)

		enum ESortBy
		{
			sortbyCellColor	=  1,
			sortbyFontColor =  2,
			sortbyIcon =  3,
			sortbyValue =  4
		};

		DEFINE_SIMPLE_TYPE(CSortBy, ESortBy, sortbyValue)

		enum ECustomFilter
		{
			customfilterEqual	=  1,
			customfilterGreaterThan =  2,
			customfilterGreaterThanOrEqual =  3,
			customfilterLessThan =  4,
			customfilterLessThanOrEqual =  5,
			customfilterNotEqual =  6
		};

		DEFINE_SIMPLE_TYPE(CCustomFilter, ECustomFilter, customfilterEqual)

		enum EDateTimeGroup
		{
			datetimegroupDay	=  1,
			datetimegroupHour =  2,
			datetimegroupMinute =  3,
			datetimegroupMonth =  4,
			datetimegroupSecond =  5,
			datetimegroupYear =  6
		};

		DEFINE_SIMPLE_TYPE(CDateTimeGroup, EDateTimeGroup, datetimegroupDay)

		enum ESortMethod
		{
			sortmethodNone = 1,
			sortmethodPinYin = 2,
			sortmethodStroke = 3,
		};

		DEFINE_SIMPLE_TYPE(CSortMethod, ESortMethod, sortmethodNone)

		enum EDynamicFilterType
		{
			dynamicfiltertypeAboveAverage = 1,
			dynamicfiltertypeBelowAverage = 2,
			dynamicfiltertypeLastMonth = 3,
			dynamicfiltertypeLastQuarter = 4,
			dynamicfiltertypeLastWeek = 5,
			dynamicfiltertypeLastYear = 6,
			dynamicfiltertypeM1 = 7,
			dynamicfiltertypeM10 = 8,
			dynamicfiltertypeM11 = 9,
			dynamicfiltertypeM12 = 10,
			dynamicfiltertypeM2 = 11,
			dynamicfiltertypeM3 = 12,
			dynamicfiltertypeM4 = 13,
			dynamicfiltertypeM5 = 14,
			dynamicfiltertypeM6 = 15,
			dynamicfiltertypeM7 = 16,
			dynamicfiltertypeM8 = 17,
			dynamicfiltertypeM9 = 18,
			dynamicfiltertypeNextMonth = 19,
			dynamicfiltertypeNextQuarter = 20,
			dynamicfiltertypeNextWeek = 21,
			dynamicfiltertypeNextYear = 22,
			dynamicfiltertypeNull = 23,
			dynamicfiltertypeQ1 = 24,
			dynamicfiltertypeQ2 = 25,
			dynamicfiltertypeQ3 = 26,
			dynamicfiltertypeQ4 = 27,
			dynamicfiltertypeThisMonth = 28,
			dynamicfiltertypeThisQuarter = 29,
			dynamicfiltertypeThisWeek = 30,
			dynamicfiltertypeThisYear = 31,
			dynamicfiltertypeToday = 32,
			dynamicfiltertypeTomorrow = 33,
			dynamicfiltertypeYearToDate = 34,
			dynamicfiltertypeYesterday = 35
		};

		DEFINE_SIMPLE_TYPE(CDynamicFilterType, EDynamicFilterType, dynamicfiltertypeNull)

		//--------------------------------------------------------
		//	ST_IconSetType (18.18.42)
		//--------------------------------------------------------
		enum EIconSetType
		{
			Arrows3				= 0,
			Arrows3Gray			= 1,
			Flags3				= 2,
			Signs3				= 3,
			Symbols3			= 4,
			Symbols3_2			= 5,
			Traffic3Lights1		= 6,
			Traffic3Lights2		= 7,
			Arrows4				= 8,
			Arrows4Gray			= 9,
			Rating4				= 10,
			RedToBlack4			= 11,
			Traffic4Lights		= 12,
			Arrows5				= 13,
			Arrows5Gray			= 14,
			Quarters5			= 15,
			Rating5				= 16,
			Triangles3			= 17,
			Stars3				= 18,
			Boxes5				= 19,
			NoIcons				= 20


		};

		DEFINE_SIMPLE_TYPE(ST_IconSetType, EIconSetType, Arrows3)
		
		//--------------------------------------------------------
		//	ST_DataBarAxisPosition
		//--------------------------------------------------------
		enum EDataBarAxisPosition
		{
			automatic_pos	= 0,
			middle_pos		= 1,
			none_pos		= 2
		};

		DEFINE_SIMPLE_TYPE(ST_DataBarAxisPosition, EDataBarAxisPosition, automatic_pos)

		//--------------------------------------------------------
		//	ST_DataBarDirection
		//--------------------------------------------------------
		enum EDataBarDirection
		{
			context_direction	= 0,
			leftToRight	= 1,
			rightToLeft	= 2
		};

		DEFINE_SIMPLE_TYPE(ST_DataBarDirection, EDataBarDirection, context_direction)

		//----------------------------------------------------
		//	18.18.15 ST_CfOperator (Conditional Format Operators)
		//----------------------------------------------------
		enum ECfOperator
		{
			Operator_beginsWith			= 0,
			Operator_between			= 1,
			Operator_containsText		= 2,
			Operator_endsWith			= 3,
			Operator_equal				= 4,
			Operator_greaterThan		= 5,
			Operator_greaterThanOrEqual	= 6,
			Operator_lessThan			= 7,
			Operator_lessThanOrEqual	= 8,
			Operator_notBetween			= 9,
			Operator_notContains		= 10,
			Operator_notEqual			= 11
		};

		DEFINE_SIMPLE_TYPE(ST_CfOperator, ECfOperator, Operator_equal)

		//----------------------------------------------------
		//	18.18.82 ST_TimePeriod (Conditional Format Value Object Type)
		//----------------------------------------------------
		enum ETimePeriod
		{
			last7Days					= 0,
			lastMonth					= 1,
			lastWeek					= 2,
			nextMonth					= 3,
			nextWeek					= 4,
			thisMonth       				= 5,
			thisWeek					= 6,
			today                                           = 7,
			tomorrow                                        = 8,
			yesterday                                       = 9
		};

		DEFINE_SIMPLE_TYPE(ST_TimePeriod, ETimePeriod, last7Days)

		//----------------------------------------------------
		//	18.18.12 ST_CfType (Conditional Format Type)
		//----------------------------------------------------
		enum ECfType
		{
			aboveAverage			= 0,
			beginsWith				= 1,
			cellIs					= 2,
			colorScale				= 3,
			containsBlanks			= 4,
			containsErrors			= 5,
			containsText			= 6,
			dataBar					= 7,
			duplicateValues			= 8,
			expression				= 9,
			iconSet					= 10,
			notContainsBlanks		= 11,
			notContainsErrors		= 12,
			notContainsText			= 13,
			timePeriod				= 14,
			top10					= 15,
			uniqueValues			= 16,
			endsWith				= 17
		};

		DEFINE_SIMPLE_TYPE(ST_CfType, ECfType, dataBar)

		//----------------------------------------------------
		//	18.18.13 ST_CfvoType (Conditional Format Value Object Type)
		//----------------------------------------------------
		enum ECfvoType
		{
			Formula					= 0,
			Maximum					= 1,
			Minimum					= 2,
			Number					= 3,
			Percent					= 4,
			Percentile				= 5,
			autoMin					= 6,
			autoMax					= 7
		};

		DEFINE_SIMPLE_TYPE(ST_CfvoType, ECfvoType, Number)

		enum ESparklineType
		{
			Line					= 0,
			Column					= 1,
			Stacked					= 2
		};

		DEFINE_SIMPLE_TYPE(ST_SparklineType, ESparklineType, Line)

		enum ESparklineAxisMinMax
		{
			Individual				= 0,
			Group					= 1,
			Custom					= 2
		};

		DEFINE_SIMPLE_TYPE(ST_SparklineAxisMinMax, ESparklineAxisMinMax, Individual)

		enum EDvAspect
		{
			Content					= 0,
			Icon					= 1
		};

		DEFINE_SIMPLE_TYPE(ST_DvAspect, EDvAspect, Icon)

		enum EOleUpdate
		{
			Always = 0,
			OnCall = 1
		};

		DEFINE_SIMPLE_TYPE(ST_OleUpdate, EOleUpdate, Always)

		enum EActivePane
		{
			activepaneBottomLeft = 0,
			activepaneBottomRight  = 1,
			activepaneTopLeft = 2,
			activepaneTopRight = 3
		};

		DEFINE_SIMPLE_TYPE(CActivePane, EActivePane, activepaneTopLeft)

		enum EPaneState
		{
			panestateFrozen = 0,
			panestateFrozenSplit  = 1,
			panestateSplit = 2
		};

		DEFINE_SIMPLE_TYPE(CPaneState, EPaneState, panestateFrozen)

		enum EDdeValueType
		{
			ddevaluetypeNil = 0,
			ddevaluetypeB = 1,
			ddevaluetypeN = 2,
			ddevaluetypeE = 3,
			ddevaluetypeStr = 4
		};

		DEFINE_SIMPLE_TYPE(CDdeValueType, EDdeValueType, ddevaluetypeNil)

		enum ECalcMode
		{
			calcmodeAuto			=  0,
			calcmodeAutoNoTable	=  1,
			calcmodeManual		=  2,
		};

		DEFINE_SIMPLE_TYPE(CCalcMode, ECalcMode, calcmodeAuto)

		enum ERefMode
		{
			refmodeA1	=  0,
			refmodeR1C1	=  1
		};

		DEFINE_SIMPLE_TYPE(CRefMode, ERefMode, refmodeA1)

		enum EDoubleOrAutomatic
		{
			typeAuto		=  0,
			typeDouble		=  1
		};

		template<EDoubleOrAutomatic eDefValue = typeAuto>
		class CDoubleOrAutomatic : public CSimpleType<EDoubleOrAutomatic, eDefValue>
		{
		public:
			double m_dValue;
			CDoubleOrAutomatic() : m_dValue(0){}
			
			CDoubleOrAutomatic(const double & val) { m_dValue = val; this->m_eValue = typeDouble; }
			CDoubleOrAutomatic(const EDoubleOrAutomatic & val) { this->m_eValue = val; m_dValue = 0; }

			virtual EDoubleOrAutomatic FromString(const std::wstring &sValue);
			double GetDoubleValue() const;
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDoubleOrAutomatic)
		};
		
		enum ESeriesLayout
		{
			typeSeriesLayoutBoxWhisker		=  0,
			typeSeriesLayoutClusteredColumn	=  1,
			typeSeriesLayoutFunnel			=  2,
			typeSeriesLayoutParetoLine		=  3,
			typeSeriesLayoutRegionMap		=  4,
			typeSeriesLayoutSunburst		=  5,
			typeSeriesLayoutTreemap			=  6,
			typeSeriesLayoutWaterfall		=  7,
		};


		DEFINE_SIMPLE_TYPE(CSeriesLayout, ESeriesLayout, typeSeriesLayoutBoxWhisker)
		
		enum EAxisUnit
		{
			unitHundreds			=  0,
			unitThousands			=  1,
			unitTenThousands		=  2,
			unitHundredThousands	=  3,
			unitMillions			=  4,
			unitTenMillions			=  5,
			unitHundredMillions		=  6,
			unitBillions			=  7,
			unitTrillions			=  8,
			unitPercentage			=  9

		};

		DEFINE_SIMPLE_TYPE(CAxisUnit, EAxisUnit, unitPercentage)

		enum EDimensionType
		{
			typeDimensionVal		=  0,
			typeDimensionX			=  1,
			typeDimensionY			=  2,
			typeDimensionSize		=  3,
			typeDimensionColorVal	=  4,
			typeDimensionCat		=  10,
			typeDimensionColorStr	=  11
		};

		DEFINE_SIMPLE_TYPE(CDimensionType, EDimensionType, typeDimensionVal)

		enum EFormulaDirection
		{
			directionCol		=  0,
			directionRow		=  1
		};

		DEFINE_SIMPLE_TYPE(CFormulaDirection, EFormulaDirection, directionCol)

		enum ESidePos
		{
			sideLeft		=  0,
			sideRight		=  1,
			sideTop			=  2,
			sideBottom		=  3
		};

		DEFINE_SIMPLE_TYPE(CSidePos, ESidePos, sideTop)

		enum EPosAlign
		{
			posAlignMin		=  0,
			posAlignCtr		=  1,
			posAlignMax		=  2
		};

		DEFINE_SIMPLE_TYPE(CPosAlign, EPosAlign, posAlignCtr)

		enum ERegionLabelLayout
		{
			layoutNone			=  0,
			layoutBestFitOnly	=  1,
			layoutShowAll		=  2
		};

		DEFINE_SIMPLE_TYPE(CRegionLabelLayout, ERegionLabelLayout, layoutNone)

		enum EParentLabelLayout
		{
			layoutNone_			=  0,
			layoutBanner		=  1,
			layoutOverlapping	=  2
		};

		DEFINE_SIMPLE_TYPE(CParentLabelLayout, EParentLabelLayout, layoutNone_)
		
		enum EQuartileMethod
		{
			methodInclusive		=  0,
			methodExclusive		=  1,
		};

		DEFINE_SIMPLE_TYPE(CQuartileMethod, EQuartileMethod, methodInclusive)

		enum EDataLabelPos
		{
			posBestFit	=  0,
			posBottom	=  1,
			posCtr		=  2,
			posInBase	=  3,
			posInEnd	=  4,
			posLeft		=  5,
			posOutEnd	=  6,
			posRight	=  7,
			posTop		=  8

		};

		DEFINE_SIMPLE_TYPE(CDataLabelPos, EDataLabelPos, posBestFit)

		DEFINE_SIMPLE_TYPE(CIntervalClosedSide, ESidePos, sideLeft)
		
		enum ETickMarksType
		{
			marksTypeIn		=  0,
			marksTypeOut	=  1,
			marksTypeCross	=  2,
			marksTypeNone	=  3

		};

		DEFINE_SIMPLE_TYPE(CTickMarksType, ETickMarksType, marksTypeNone)

		enum ESourceCacheType
		{
			typeSourceConsolidation	=  0,
			typeSourceExternal		=  1,
			typeSourceScenario		=  2,
			typeSourceWorksheet		=  3,
		};

		DEFINE_SIMPLE_TYPE(CSourceCacheType, ESourceCacheType, typeSourceWorksheet)

		enum EValuesGroupBy
		{
			groupByDays				=  0,
			groupByHours			=  1,
			groupByMinutes			=  2,
			groupByMonths			=  3,
			groupByQuarters			=  4,
			groupByNumericRanges	=  5,
			groupBySeconds			=  6,
			groupByYears			=  7
		};

		DEFINE_SIMPLE_TYPE(CValuesGroupBy, EValuesGroupBy, groupByMonths)

		enum EPivotItemType
		{
			typeAverage	=  0,
			typeBlank	=  1,
			typeCount	=  2,
			typeCountA	=  3,
			typeData	=  4,
			typeDefault	=  5,
			typeGrandTotalt	=  6,
			typeMin		=  7,
			typeMax		=  8,
			typeProduct	=  9,
			typeStdDev	=  10,
			typeStdDevP	=  11,
			typeSum		=  12,
			typeVar		=  13,
			typeVarP	=  14
		};

		DEFINE_SIMPLE_TYPE(CPivotItemType, EPivotItemType, typeSum)

		enum EFieldSortType
		{
			sortManual		=  0,
			sortAscending	=  1,
			sortDescending	=  2
		};

		DEFINE_SIMPLE_TYPE(CFieldSortType, EFieldSortType, sortManual)
		
		enum EPivotAreaType
		{
			areaAll			=  0,
			areaFieldButton =  1,
			areaData		=  2,
			areaNone		=  3,
			areaNormal		=  4,
			areaOrigin		=  5,
			areaTopEnd		=  6
		};

		DEFINE_SIMPLE_TYPE(CPivotAreaType, EPivotAreaType, areaNone)

		enum EPivotAxisType
		{
			axisCol		=  0,
			axisPage	=  1,
			axisRow		=  2,
			axisValues	=  3
		};

		DEFINE_SIMPLE_TYPE(CPivotAxisType, EPivotAxisType, axisCol)

		enum EFormatAction
		{
			actionBlank		=  0,
			actionDrill		=  1,
			actionFormatting=  2,
			actionFormula	=  3
		};

		DEFINE_SIMPLE_TYPE(CFormatAction, EFormatAction, actionBlank)

		enum EShowDataAs
		{
			dataAsDifference	=  0,
			dataAsIndex			=  1,
			dataAsNormal		=  2,
			dataAsPercentOff	=  3,
			dataAsPercentDiff	=  4,
			dataAsPercentOfCol	=  5,
			dataAsPercentOfRow	=  6,
			dataAsPercentOfTotal =  7,
			dataAsRunTotal		=  8
		};

		DEFINE_SIMPLE_TYPE(CShowDataAs, EShowDataAs, dataAsRunTotal)

		enum EDataConsolidateFunction
		{
			functionAverage		=  0,
			functionCount		=  1,
			functionCountNums	=  2,
			functionMaximum		=  3,
			functionMinimum		=  4,
			functionProduct		=  5,
			functionStdDev		=  6,
			functionStdDevP		=  7,
			functionSum		=  8,
			functionVariance	=  9,
			functionVarP		=  10
		};

		DEFINE_SIMPLE_TYPE(CDataConsolidateFunction, EDataConsolidateFunction, functionSum)

		enum EDataValidationType
		{
			validationTypeNone		=  0,
			validationTypeCustom	=  1,
			validationTypeDate		=  2,
			validationTypeDecimal	=  3,
			validationTypeList		=  4,
			validationTypeTextLength=  5,
			validationTypeTime		=  6,
			validationTypeWhole		=  7
		};

		DEFINE_SIMPLE_TYPE(CDataValidationType, EDataValidationType, validationTypeNone)

		enum EDataValidationErrorStyle
		{
			errorStyleStop			=  0,
			errorStyleWarning		=  1,
			errorStyleInformation	=  2
		};

		DEFINE_SIMPLE_TYPE(CDataValidationErrorStyle, EDataValidationErrorStyle, errorStyleStop)

		enum EDataValidationOperator
		{
			operatorBetween				=  0,
			operatorNotBetween			=  1,
			operatorEqual				=  2,
			operatorNotEqual			=  3,
			operatorLessThan			=  4,
			operatorLessThanOrEqual		=  5,
			operatorGreaterThan			=  6,
			operatorGreaterThanOrEqual	=  7
		};

		DEFINE_SIMPLE_TYPE(CDataValidationOperator, EDataValidationOperator, operatorBetween)

		enum EDataValidationImeMode
		{
			imeModeNoControl		=  0,
			imeModeOff				=  1,
			imeModeOn				=  2,
			imeModeDisabled		=  3,
			imeModeHiragana		=  4,
			imeModeFullKatakana	=  5,
			imeModeHalfKatakana	=  6,
			imeModeFullAlpha		=  7,
			imeModeHalfAlpha		=  8,
			imeModeFullHangul		=  9,
			imeModeHalfHangul		=  10
		};

		DEFINE_SIMPLE_TYPE(CDataValidationImeMode, EDataValidationImeMode, imeModeNoControl)

		enum EObjectType
		{
			objectButton	= 0,
			objectCheckBox	= 1,
			objectDrop		= 2,
			objectGBox		= 3,
			objectLabel		= 4,
			objectList		= 5,
			objectRadio		= 6,
			objectScroll	= 7,
			objectSpin		= 8,
			objectEditBox	= 9,
			objectDialog	= 10,
			objectToggleButton = 11,
			objectTabStrip = 12,
			objectImage = 13
		};

		DEFINE_SIMPLE_TYPE(CObjectType, EObjectType, objectButton)

		enum EChecked
		{
			valUnchecked	=  0,
			valChecked		=  1,
			valMixed		=  2
		};

		DEFINE_SIMPLE_TYPE(CChecked, EChecked, valUnchecked)

		enum EDropStyle
		{
			valCombo		=  0,
			valComboedit	=  1,
			valSimple		=  2
		};

		DEFINE_SIMPLE_TYPE_START(CDropStyle, EDropStyle, valSimple)
		public:
			std::wstring ToVmlString() const;
		};

		enum ESelType
		{
			valSingle	=  0,
			valMulti	=  1,
			valExtended	=  2
		};

		DEFINE_SIMPLE_TYPE_START(CSelType, ESelType, valSingle)
		public:
			std::wstring ToVmlString() const;
		};

		enum EEditValidation
		{
			editText		=  0,
			editInteger		=  1,
			editNumber		=  2,
			editReference	=  3,
			editFormula		=  4
		};

		DEFINE_SIMPLE_TYPE(CEditValidation, EEditValidation, editText)

		enum ETabularSlicerCacheSortOrder
		{
			ctabularslicercachesortorderAscending = 0,
			ctabularslicercachesortorderDescending = 1
		};

		DEFINE_SIMPLE_TYPE(CTabularSlicerCacheSortOrder, ETabularSlicerCacheSortOrder, ctabularslicercachesortorderAscending)

		enum ESlicerCacheCrossFilter
		{
			cslicercachecrossfilterNone = 0,
			cslicercachecrossfilterShowItemsWithDataAtTop = 1,
			cslicercachecrossfilterShowItemsWithNoData = 2
		};		

		DEFINE_SIMPLE_TYPE(CSlicerCacheCrossFilter, ESlicerCacheCrossFilter, cslicercachecrossfilterNone)

		enum EOlapSlicerCacheSortOrder
		{
			colapslicercachesortorderNatural = 0,
			colapslicercachesortorderAscending = 1,
			colapslicercachesortorderDescending = 2
		};

		DEFINE_SIMPLE_TYPE(COlapSlicerCacheSortOrder, EOlapSlicerCacheSortOrder, colapslicercachesortorderNatural)

		enum ESlicerStyleType
		{
			cslicerstyletypeUnselectedItemWithData = 0,
			cslicerstyletypeSelectedItemWithData = 1,
			cslicerstyletypeUnselectedItemWithNoData = 2,
			cslicerstyletypeSelectedItemWithNoData = 3,
			cslicerstyletypeHoveredUnselectedItemWithData = 4,
			cslicerstyletypeHoveredSelectedItemWithData = 5,
			cslicerstyletypeHoveredUnselectedItemWithNoData = 6,
			cslicerstyletypeHoveredSelectedItemWithNoData = 7
		};

		DEFINE_SIMPLE_TYPE(CSlicerStyleType, ESlicerStyleType, cslicerstyletypeUnselectedItemWithData)

	}// Spreadsheet
} // SimpleTypes
