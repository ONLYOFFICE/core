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
		template<ETableType eDefValue = typeWorksheet>
		class CTableType : public CSimpleType<ETableType, eDefValue>
		{
		public:
			CTableType();
			CTableType(const ETableType & val)	{ this->m_eValue = val; }

			virtual ETableType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CTableType)
		};
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
		template<EExternalConnectionType eDefValue = extConnTypeGeneral>
		class CExternalConnectionType : public CSimpleType<EExternalConnectionType, eDefValue>
		{
		public:
			CExternalConnectionType() {}
			CExternalConnectionType(const EExternalConnectionType & val)	{ this->m_eValue = val; }

			virtual EExternalConnectionType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CExternalConnectionType)
		};
		enum ECredMethod
		{
			integrated_method	= 0,
			none_method			= 1,
			prompt_method		= 2,
			stored_method		= 3
		};
		template<ECredMethod eDefValue = integrated_method>
		class CCredMethod : public CSimpleType<ECredMethod, eDefValue>
		{
		public:
			CCredMethod() {}
			CCredMethod(const ECredMethod & val)	{ this->m_eValue = val; }

			virtual ECredMethod FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CCredMethod)
		};

		enum EVisibleType
		{
			visibleHidden		=  0,
			visibleVeryHidden	=  1,
			visibleVisible		=  2,
		};
		template<EVisibleType eDefValue = visibleVisible>
		class CVisibleType : public CSimpleType<EVisibleType, eDefValue>
		{
		public:
			CVisibleType() {}
			CVisibleType(const EVisibleType & val)
			{
				this->m_eValue = val;
			}

			virtual EVisibleType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CVisibleType)
		};
		enum EHtmlFormat
		{
			htmlNone =  0,
			htmlRtf	=  1,
			htmlAll	=  2
		};
		template<EHtmlFormat eDefValue = htmlNone>
		class CHtmlFormat : public CSimpleType<EHtmlFormat, eDefValue>
		{
		public:
			CHtmlFormat() {}
			CHtmlFormat(const EHtmlFormat & val)	{ this->m_eValue = val; }

			virtual EHtmlFormat FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CHtmlFormat)
		};
		enum EParameterType
		{
			parameterPrompt =  0,
			parameterValue	=  1,
			parameterCell	=  2
		};
		template<EParameterType eDefValue = parameterPrompt>
		class CParameterType : public CSimpleType<EParameterType, eDefValue>
		{
		public:
			CParameterType() {}
			CParameterType(const EParameterType & val)	{ this->m_eValue = val; }

			virtual EParameterType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CParameterType)
		};
		enum EQualifier
		{
			doubleQuote =  0,
			singleQuote	=  1,
			noneQuote	=  2
		};
		template<EQualifier eDefValue = doubleQuote>
		class CQualifier : public CSimpleType<EQualifier, eDefValue>
		{
		public:
			CQualifier() {}
			CQualifier(const EQualifier & val)	{ this->m_eValue = val; }

			virtual EQualifier FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CQualifier)
		};
		enum EFileType
		{
			fileTypeMac		=  0,
			fileTypeWin		=  1,
			fileTypeDos		=  2,
			fileTypeLin		=  3,
			fileTypeOther	=  4
		};
		template<EFileType eDefValue = fileTypeWin>
		class CFileType : public CSimpleType<EFileType, eDefValue>
		{
		public:
			CFileType() {}
			CFileType(const EFileType & val)	{ this->m_eValue = val; }

			virtual EFileType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CFileType)
		};
		enum EPhoneticAlignmentType
		{
			phoneticalignmentCenter			=  0,
			phoneticalignmentDistributed	=  1,
			phoneticalignmentLeft			=  2,
			phoneticalignmentNoControl 		=  3,
		};

		template<EPhoneticAlignmentType eDefValue = phoneticalignmentNoControl>
		class CPhoneticAlignment : public CSimpleType<EPhoneticAlignmentType, eDefValue>
		{
		public:
			CPhoneticAlignment() {}
			CPhoneticAlignment(const EPhoneticAlignmentType & val)	{ this->m_eValue = val; }

			virtual EPhoneticAlignmentType FromString(const std::wstring &sValue);
			virtual std::wstring       ToString  () const;

			SimpleTypes_Default(CPhoneticAlignment)
		};

		enum EPhoneticTypeType
		{
			phonetictypeFullwidthKatakana			=  0,
			phonetictypeHalfwidthKatakana			=  1,
			phonetictypeHiragana					=  2,
			phonetictypeNoConversion 				=  3,
		};

		template<EPhoneticTypeType eDefValue = phonetictypeNoConversion>
		class CPhoneticType : public CSimpleType<EPhoneticTypeType, eDefValue>
		{
		public:
			CPhoneticType() {}
			CPhoneticType(const EPhoneticTypeType & val)	{ this->m_eValue = val; }

			virtual EPhoneticTypeType FromString(const std::wstring &sValue);
			virtual std::wstring       ToString  () const;

			SimpleTypes_Default(CPhoneticType)
		};

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

		template<EFontCharset eDefValue = fontcharsetANSI>
		class CFontCharset : public CSimpleType<EFontCharset, eDefValue>
		{
		public:
			CFontCharset() {}
			CFontCharset(const EFontCharset & val)	{ this->m_eValue = val; }

			virtual EFontCharset FromString(const std::wstring &sValue);

			virtual std::wstring       ToString  () const;
			virtual std::wstring       ToHexString  () const;

			SimpleTypes_Default(CFontCharset)
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

		template<EThemeColor eDefValue = themecolorDark1>
		class CThemeColor : public CSimpleType<EThemeColor, eDefValue>
		{
		public:
			CThemeColor() {}
			CThemeColor(const EThemeColor & val)	{ this->m_eValue = val; }

			virtual EThemeColor FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CThemeColor)
		};

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
		template<EFontFamily eDefValue = fontfamilyNotApplicable>
		class CFontFamily : public CSimpleType<EFontFamily, eDefValue>
		{
		public:
			CFontFamily() {}
			CFontFamily(const EFontFamily & val)	{ this->m_eValue = val; }

			virtual EFontFamily FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;
			std::wstring ToStringWord() const;

			SimpleTypes_Default(CFontFamily)
		};

		enum EFontScheme
		{
			fontschemeMajor		=  0,
			fontschemeMinor		=  1,
			fontschemeNone		=  2
		};
		template<EFontScheme eDefValue = fontschemeNone>
		class CFontScheme : public CSimpleType<EFontScheme, eDefValue>
		{
		public:
			CFontScheme() {}
			CFontScheme(const EFontScheme & val)	{ this->m_eValue = val; }

			virtual EFontScheme FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CFontScheme)
		};
		enum EUnderline
		{
			underlineDouble				=  0,
			underlineDoubleAccounting	=  1,
			underlineNone				=  2,
			underlineSingle				=  3,
			underlineSingleAccounting	=  4
		};
		template<EUnderline eDefValue = underlineSingle>
		class CUnderline : public CSimpleType<EUnderline, eDefValue>
		{
		public:
			CUnderline() {}
			CUnderline(const EUnderline & val)	{ this->m_eValue = val; }

			virtual EUnderline FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CUnderline)
		};

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
		template<EBorderStyle eDefValue = borderstyleNone>
		class CBorderStyle : public CSimpleType<EBorderStyle, eDefValue>
		{
		public:
			CBorderStyle() {}
			CBorderStyle(const EBorderStyle & val)	{ this->m_eValue = val; }

			virtual EBorderStyle FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CBorderStyle)
		};

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
		template<EHorizontalAlignment eDefValue = horizontalalignmentGeneral>
		class CHorizontalAlignment : public CSimpleType<EHorizontalAlignment, eDefValue>
		{
		public:
			CHorizontalAlignment() {}
			CHorizontalAlignment(const EHorizontalAlignment & val)	{ this->m_eValue = val; }

			virtual EHorizontalAlignment FromString(const std::wstring &sValue);
			virtual std::wstring ToVmlString() const;
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CHorizontalAlignment)
		};
		enum EVerticalAlignment
		{
			verticalalignmentBottom				=  0,
			verticalalignmentCenter				=  1,
			verticalalignmentDistributed		=  2,
			verticalalignmentJustify			=  3,
			verticalalignmentTop				=  4
		};
		template<EVerticalAlignment eDefValue = verticalalignmentBottom>
		class CVerticalAlignment : public CSimpleType<EVerticalAlignment, eDefValue>
		{
		public:
			CVerticalAlignment() {}
			CVerticalAlignment(const EVerticalAlignment & val)	{ this->m_eValue = val; }

			virtual EVerticalAlignment FromString(const std::wstring &sValue);
			virtual std::wstring ToVmlString() const;
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CVerticalAlignment)
		};
		enum EGradientType
		{
			gradienttypeLine				=  0,
			gradienttypePath				=  1
		};
		template<EGradientType eDefValue = gradienttypeLine>
		class CGradientType : public CSimpleType<EGradientType, eDefValue>
		{
		public:
			CGradientType() {}
			CGradientType(const EGradientType & val)	{ this->m_eValue = val; }

			virtual EGradientType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CGradientType)
		};
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
		template<EPatternType eDefValue = patterntypeNone>
		class CPatternType : public CSimpleType<EPatternType, eDefValue>
		{
		public:
			CPatternType() {}
			CPatternType(const EPatternType & val)	{ this->m_eValue = val; }

			virtual EPatternType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPatternType)
		};

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
		template<ETableStyleType eDefValue = tablestyletypeBlankRow>
		class CTableStyleType : public CSimpleType<ETableStyleType, eDefValue>
		{
		public:
			CTableStyleType() {}
			CTableStyleType(const ETableStyleType & val)	{ this->m_eValue = val; }

			virtual ETableStyleType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CTableStyleType)
		};
		enum ECellFormulaType
		{
			cellformulatypeArray			=  0,
			cellformulatypeDataTable		=  1,
			cellformulatypeNormal			=  2,
			cellformulatypeShared			=  3
		};
		template<ECellFormulaType eDefValue = cellformulatypeNormal>
		class CCellFormulaType : public CSimpleType<ECellFormulaType, eDefValue>
		{
		public:
			CCellFormulaType() {}
			CCellFormulaType(const ECellFormulaType & val)	{ this->m_eValue = val; }

			virtual ECellFormulaType FromString(const std::wstring &sValue);
			ECellFormulaType FromStringA(const char* sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CCellFormulaType)
		};
		enum EUpdateLinksType
		{
			updatelinksAlways			=  0,
			updatelinksNever			=  1,
			updatelinksUserSet			=  2
		};
		template<EUpdateLinksType eDefValue = updatelinksAlways>
		class CUpdateLinksType : public CSimpleType<EUpdateLinksType, eDefValue>
		{
		public:
			CUpdateLinksType() {}
			CUpdateLinksType(const EUpdateLinksType & val)	{ this->m_eValue = val; }

			virtual EUpdateLinksType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CUpdateLinksType)
		};
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
		template<ECellTypeType eDefValue = celltypeNumber>
		class CCellTypeType : public CSimpleType<ECellTypeType, eDefValue>
		{
		public:
			CCellTypeType() {}
			CCellTypeType(const ECellTypeType & val)	{ this->m_eValue = val; }

			virtual ECellTypeType FromString(const std::wstring &sValue);
			ECellTypeType FromStringA(const char* sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CCellTypeType)
		};
		enum ECellAnchorType
		{
			cellanchorAbsolute			=  0,
			cellanchorOneCell			=  1,
			cellanchorTwoCell			=  2
		};
		template<ECellAnchorType eDefValue = cellanchorTwoCell>
		class CCellAnchorType : public CSimpleType<ECellAnchorType, eDefValue>
		{
		public:
			CCellAnchorType() {}
			CCellAnchorType(const ECellAnchorType & val)	{ this->m_eValue = val; }

			virtual ECellAnchorType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CCellAnchorType)
		};

		enum ESheetViewType
		{
			sheetviewNormal				=  0,
			sheetviewPageBreakPreview	=  1,
			sheetviewPageLayout			=  2
		};
		template<ESheetViewType eDefValue = sheetviewNormal>
		class CSheetViewType : public CSimpleType<ESheetViewType, eDefValue>
		{
		public:
			CSheetViewType() {}
			CSheetViewType(const ESheetViewType & val)	{ this->m_eValue = val; }

			virtual ESheetViewType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CSheetViewType)
		};
		enum EChartLegendPos
		{
			chartlegendposLeft		=  0,
			chartlegendposTop		=  1,
			chartlegendposRight		=  2,
			chartlegendposBottom	=  3,
			chartlegendposRightTop	=  4
		};
		template<EChartLegendPos eDefValue = chartlegendposTop>
		class CChartLegendPos : public CSimpleType<EChartLegendPos, eDefValue>
		{
		public:
			CChartLegendPos() {}
			CChartLegendPos(const EChartLegendPos & val)	{ this->m_eValue = val; }

			virtual EChartLegendPos FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CChartLegendPos)
		};
		enum EChartHMode
		{
			charthmodeFactor	=  0,
			charthmodeEdge		=  1
		};
		template<EChartHMode eDefValue = charthmodeFactor>
		class CChartHMode : public CSimpleType<EChartHMode, eDefValue>
		{
		public:
			CChartHMode() {}
			CChartHMode(const EChartHMode & val)	{ this->m_eValue = val; }

			virtual EChartHMode FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CChartHMode)
		};
		enum EChartLayoutTarget
		{
			chartlayouttargetInner	=  0,
			chartlayouttargetOuter	=  1
		};
		template<EChartLayoutTarget eDefValue = chartlayouttargetInner>
		class CChartLayoutTarget : public CSimpleType<EChartLayoutTarget, eDefValue>
		{
		public:
			CChartLayoutTarget() {}
			CChartLayoutTarget(const EChartLayoutTarget & val)	{ this->m_eValue = val; }

			virtual EChartLayoutTarget FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CChartLayoutTarget)
		};
		enum EChartAxPos
		{
			chartaxposLeft	=  0,
			chartaxposTop	=  1,
			chartaxposRight	=  2,
			chartaxposBottom=  3
		};
		template<EChartAxPos eDefValue = chartaxposLeft>
		class CChartAxPos : public CSimpleType<EChartAxPos, eDefValue>
		{
		public:
			CChartAxPos() {}
			CChartAxPos(const EChartAxPos & val)	{ this->m_eValue = val; }

			virtual EChartAxPos FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CChartAxPos)
		};
		enum EChartBarGrouping
		{
			chartbargroupingClustered	=  0,
			chartbargroupingPercentStacked	=  1,
			chartbargroupingStacked	=  2,
			chartbargroupingStandard=  3
		};
		template<EChartBarGrouping eDefValue = chartbargroupingClustered>
		class CChartBarGrouping : public CSimpleType<EChartBarGrouping, eDefValue>
		{
		public:
			CChartBarGrouping() {}
			CChartBarGrouping(const EChartBarGrouping & val)	{ this->m_eValue = val; }

			virtual EChartBarGrouping FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CChartBarGrouping)
		};
		enum EChartBarDirection
		{
			chartbardirectionBar	=  0,
			chartbardirectionCol	=  1
		};
		template<EChartBarDirection eDefValue = chartbardirectionBar>
		class CChartBarDirection : public CSimpleType<EChartBarDirection, eDefValue>
		{
		public:
			CChartBarDirection() {}
			CChartBarDirection(const EChartBarDirection & val)	{ this->m_eValue = val; }

			virtual EChartBarDirection FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CChartBarDirection)
		};
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
		template<EChartSymbol eDefValue = chartsymbolNone>
		class CChartSymbol : public CSimpleType<EChartSymbol, eDefValue>
		{
		public:
			CChartSymbol() {}
			CChartSymbol(const EChartSymbol & val)	{ this->m_eValue = val; }

			virtual EChartSymbol FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CChartSymbol)
		};
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
		template<EPageSize eDefValue = pagesizeA4Paper>
		class CPageSize : public CSimpleType<EPageSize, eDefValue>
		{
		public:
			CPageSize() {}
			CPageSize(const EPageSize & val)	{ this->m_eValue = val; }

			virtual EPageSize FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPageSize)
		};
		
		enum EPageUnits
		{
			mm		= 0,
			cm		= 1,
			inch	= 2,
			pt		= 3,
			px		= 4,
			emu		= 5

		};
		template<EPageUnits eDefValue = mm>
		class CPageUnits : public CSimpleType<EPageUnits, eDefValue>
		{
		public:
			CPageUnits() {}
			CPageUnits(const EPageUnits & val)	{ this->m_eValue = val; }

			virtual EPageUnits FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPageUnits)
		};

		enum ECellComments
		{
			cellcommentsNone = 0,
			cellcommentsAsDisplayed = 1,
			cellcommentsAtEnd = 2
		};

		template<ECellComments eDefValue = cellcommentsNone>
		class CCellComments : public CSimpleType<ECellComments, eDefValue>
		{
		public:
			CCellComments() {}
			CCellComments(const ECellComments & val)	{ this->m_eValue = val; }
			virtual ECellComments FromString(const std::wstring &sValue);
			virtual std::wstring          ToString  () const;

			SimpleTypes_Default(CCellComments)
		};

		enum EPrintError
		{
			printerrorDisplayed  = 0,
			printerrorBlank = 1,
			printerrorDash  = 2,
			printerrorNA  = 3
		};

		template<EPrintError eDefValue = printerrorDisplayed>
		class CPrintError : public CSimpleType<EPrintError, eDefValue>
		{
		public:
			CPrintError() {}
			CPrintError(const EPrintError & val)	{ this->m_eValue = val; }
			virtual EPrintError FromString(const std::wstring &sValue);
			virtual std::wstring          ToString  () const;

			SimpleTypes_Default(CPrintError)
		};

		enum EPageOrder
		{
			pageorderDownThenOver = 0,
			pageorderOverThenDown  = 1
		};

		template<EPageOrder eDefValue = pageorderDownThenOver>
		class CPageOrder : public CSimpleType<EPageOrder, eDefValue>
		{
		public:
			CPageOrder() {}
			CPageOrder(const EPageOrder & val)	{ this->m_eValue = val; }
			virtual EPageOrder FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPageOrder)
		};
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

		template<ETotalsRowFunction eDefValue = totalrowfunctionNone>
		class CTotalsRowFunction : public CSimpleType<ETotalsRowFunction, eDefValue>
		{
		public:
			CTotalsRowFunction() {}
			CTotalsRowFunction(const ETotalsRowFunction & val)	{ this->m_eValue = val; }

			virtual ETotalsRowFunction FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CTotalsRowFunction)
		};
		enum ESortBy
		{
			sortbyCellColor	=  1,
			sortbyFontColor =  2,
			sortbyIcon =  3,
			sortbyValue =  4
		};
		template<ESortBy eDefValue = sortbyValue>
		class CSortBy : public CSimpleType<ESortBy, eDefValue>
		{
		public:
			CSortBy() {}
			CSortBy(const ESortBy & val)	{ this->m_eValue = val; }

			virtual ESortBy FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CSortBy)
		};
		enum ECustomFilter
		{
			customfilterEqual	=  1,
			customfilterGreaterThan =  2,
			customfilterGreaterThanOrEqual =  3,
			customfilterLessThan =  4,
			customfilterLessThanOrEqual =  5,
			customfilterNotEqual =  6
		};
		template<ECustomFilter eDefValue = customfilterEqual>
		class CCustomFilter : public CSimpleType<ECustomFilter, eDefValue>
		{
		public:
			CCustomFilter() {}
			CCustomFilter(const ECustomFilter & val)	{ this->m_eValue = val; }

			virtual ECustomFilter FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CCustomFilter)
		};
		enum EDateTimeGroup
		{
			datetimegroupDay	=  1,
			datetimegroupHour =  2,
			datetimegroupMinute =  3,
			datetimegroupMonth =  4,
			datetimegroupSecond =  5,
			datetimegroupYear =  6
		};
		template<EDateTimeGroup eDefValue = datetimegroupDay>
		class CDateTimeGroup : public CSimpleType<EDateTimeGroup, eDefValue>
		{
		public:
			CDateTimeGroup() {}
			CDateTimeGroup(const EDateTimeGroup & val)	{ this->m_eValue = val; }

			virtual EDateTimeGroup FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CDateTimeGroup)
		};
		enum ESortMethod
		{
			sortmethodNone = 1,
			sortmethodPinYin = 2,
			sortmethodStroke = 3,
		};
		template<ESortMethod eDefValue = sortmethodNone>
		class CSortMethod : public CSimpleType<ESortMethod, eDefValue>
		{
		public:
			CSortMethod() {}
			CSortMethod(const ESortMethod & val)	{ this->m_eValue = val; }

			virtual ESortMethod FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CSortMethod)
		};
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
		template<EDynamicFilterType eDefValue = dynamicfiltertypeNull>
		class CDynamicFilterType : public CSimpleType<EDynamicFilterType, eDefValue>
		{
		public:
			CDynamicFilterType() {}
			CDynamicFilterType(const EDynamicFilterType & val)	{ this->m_eValue = val; }

			virtual EDynamicFilterType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(CDynamicFilterType)
		};
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
		template<EIconSetType eDefValue = Arrows3>
		class ST_IconSetType : public CSimpleType<EIconSetType, eDefValue>
		{
		public:
			ST_IconSetType() {}
			ST_IconSetType(const EIconSetType & val)	{ this->m_eValue = val; }

			virtual EIconSetType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(ST_IconSetType)
		};
		
		//--------------------------------------------------------
		//	ST_DataBarAxisPosition
		//--------------------------------------------------------
		enum EDataBarAxisPosition
		{
			automatic_pos	= 0,
			middle_pos		= 1,
			none_pos		= 2
		};
		template<EDataBarAxisPosition eDefValue = automatic_pos>
		class ST_DataBarAxisPosition : public CSimpleType<EDataBarAxisPosition, eDefValue>
		{
		public:
			ST_DataBarAxisPosition() {}
			ST_DataBarAxisPosition(const EDataBarAxisPosition & val)	{ this->m_eValue = val; }

			virtual EDataBarAxisPosition FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(ST_DataBarAxisPosition)
		};

		//--------------------------------------------------------
		//	ST_DataBarDirection
		//--------------------------------------------------------
		enum EDataBarDirection
		{
			context_direction	= 0,
			leftToRight	= 1,
			rightToLeft	= 2
		};
		template<EDataBarDirection eDefValue = context_direction>
		class ST_DataBarDirection : public CSimpleType<EDataBarDirection, eDefValue>
		{
		public:
			ST_DataBarDirection() {}
			ST_DataBarDirection(const EDataBarDirection & val)	{ this->m_eValue = val; }

			virtual EDataBarDirection FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(ST_DataBarDirection)
		};
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
		template<ECfOperator eDefValue = Operator_equal>
		class ST_CfOperator : public CSimpleType<ECfOperator, eDefValue>
		{
		public:
			ST_CfOperator() {}
			ST_CfOperator(const ECfOperator & val)	{ this->m_eValue = val; }

			virtual ECfOperator FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(ST_CfOperator)
		};

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
		template<ETimePeriod eDefValue = last7Days>
		class ST_TimePeriod : public CSimpleType<ETimePeriod, eDefValue>
		{
		public:
			ST_TimePeriod() {}
			ST_TimePeriod(const ETimePeriod & val)	{ this->m_eValue = val; }

			virtual ETimePeriod FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(ST_TimePeriod)
		};

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
		template<ECfType eDefValue = dataBar>
		class ST_CfType : public CSimpleType<ECfType, eDefValue>
		{
		public:
			ST_CfType() {}
			ST_CfType(const ECfType & val)	{ this->m_eValue = val; }

			virtual ECfType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(ST_CfType)
		};

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
		template<ECfvoType eDefValue = Number>
		class ST_CfvoType : public CSimpleType<ECfvoType, eDefValue>
		{
		public:
			ST_CfvoType() {}
			ST_CfvoType(const ECfvoType & val)	{ this->m_eValue = val; }

			virtual ECfvoType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(ST_CfvoType)
		};

		enum ESparklineType
		{
			Line					= 0,
			Column					= 1,
			Stacked					= 2
		};
		template<ESparklineType eDefValue = Line>
		class ST_SparklineType : public CSimpleType<ESparklineType, eDefValue>
		{
		public:
			ST_SparklineType() {}
			ST_SparklineType(const ESparklineType & val)	{ this->m_eValue = val; }

			virtual ESparklineType FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(ST_SparklineType)
		};

		enum ESparklineAxisMinMax
		{
			Individual				= 0,
			Group					= 1,
			Custom					= 2
		};
		template<ESparklineAxisMinMax eDefValue = Individual>
		class ST_SparklineAxisMinMax : public CSimpleType<ESparklineAxisMinMax, eDefValue>
		{
		public:
			ST_SparklineAxisMinMax() {}
			ST_SparklineAxisMinMax(const ESparklineAxisMinMax & val)	{ this->m_eValue = val; }

			virtual ESparklineAxisMinMax FromString(const std::wstring &sValue);
			virtual std::wstring     ToString  () const;

			SimpleTypes_Default(ST_SparklineAxisMinMax)
		};
		enum EDvAspect
		{
			Content					= 0,
			Icon					= 1
		};
		template<EDvAspect eDefValue = Icon>
		class ST_DvAspect : public CSimpleType<EDvAspect, eDefValue>
		{
		public:
			ST_DvAspect() {}
			ST_DvAspect(const EDvAspect & val)	{ this->m_eValue = val; }

			virtual EDvAspect FromString(const std::wstring &sValue);
			virtual std::wstring	 ToString  () const;

			SimpleTypes_Default(ST_DvAspect)
		};
		enum EOleUpdate
		{
			Always = 0,
			OnCall = 1
		};
		template<EOleUpdate eDefValue = Always>
		class ST_OleUpdate : public CSimpleType<EOleUpdate, eDefValue>
		{
		public:
			ST_OleUpdate() {}
			ST_OleUpdate(const EOleUpdate & val)	{ this->m_eValue = val; }

			virtual EOleUpdate FromString(const std::wstring &sValue);
			virtual std::wstring	 ToString  () const;

			SimpleTypes_Default(ST_OleUpdate)
		};

		enum EActivePane
		{
			activepaneBottomLeft = 0,
			activepaneBottomRight  = 1,
			activepaneTopLeft = 2,
			activepaneTopRight = 3
		};

		template<EActivePane eDefValue = activepaneTopLeft>
		class CActivePane : public CSimpleType<EActivePane, eDefValue>
		{
		public:
			CActivePane() {}
			CActivePane(const EActivePane & val)	{ this->m_eValue = val; }

			virtual EActivePane FromString(const std::wstring &sValue);
			virtual std::wstring          ToString  () const;

			SimpleTypes_Default(CActivePane)
		};

		enum EPaneState
		{
			panestateFrozen = 0,
			panestateFrozenSplit  = 1,
			panestateSplit = 2
		};

		template<EPaneState eDefValue = panestateFrozen>
		class CPaneState : public CSimpleType<EPaneState, eDefValue>
		{
		public:
			CPaneState() {}
			CPaneState(const EPaneState & val)	{ this->m_eValue = val; }

			virtual EPaneState FromString(const std::wstring &sValue);
			virtual std::wstring          ToString  () const;

			SimpleTypes_Default(CPaneState)
		};

		enum EDdeValueType
		{
			ddevaluetypeNil = 0,
			ddevaluetypeB = 1,
			ddevaluetypeN = 2,
			ddevaluetypeE = 3,
			ddevaluetypeStr = 4
		};

		template<EDdeValueType eDefValue = ddevaluetypeNil>
		class CDdeValueType : public CSimpleType<EDdeValueType, eDefValue>
		{
		public:
			CDdeValueType() {}
			CDdeValueType(const EDdeValueType & val)	{ this->m_eValue = val; }

			virtual EDdeValueType FromString(const std::wstring &sValue);
			virtual std::wstring          ToString  () const;

			SimpleTypes_Default(CDdeValueType)
		};

		enum ECalcMode
		{
			calcmodeAuto			=  0,
			calcmodeAutoNoTable	=  1,
			calcmodeManual		=  2,
		};

		template<ECalcMode eDefValue = calcmodeAuto>
		class CCalcMode : public CSimpleType<ECalcMode, eDefValue>
		{
		public:
			CCalcMode() {}
			CCalcMode(const ECalcMode & val)	{ this->m_eValue = val; }

			virtual ECalcMode FromString(const std::wstring &sValue);
			virtual std::wstring  ToString  () const;

			SimpleTypes_Default(CCalcMode)
		};

		enum ERefMode
		{
			refmodeA1	=  0,
			refmodeR1C1	=  1
		};

		template<ERefMode eDefValue = refmodeA1>
		class CRefMode : public CSimpleType<ERefMode, eDefValue>
		{
		public:
			CRefMode() {}
			CRefMode(const ERefMode & val)	{ this->m_eValue = val; }

			virtual ERefMode FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CRefMode)
		};

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
		template<ESeriesLayout eDefValue = typeSeriesLayoutBoxWhisker>
		class CSeriesLayout : public CSimpleType<ESeriesLayout, eDefValue>
		{
		public:
			CSeriesLayout() {}
			CSeriesLayout(const ESeriesLayout & val)	{ this->m_eValue = val; }

			virtual ESeriesLayout FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CSeriesLayout)
		};
		
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
		template<EAxisUnit eDefValue = unitPercentage>
		class CAxisUnit : public CSimpleType<EAxisUnit, eDefValue>
		{
		public:
			CAxisUnit() {}
			CAxisUnit(const EAxisUnit & val)	{ this->m_eValue = val; }

			virtual EAxisUnit FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CAxisUnit)
		};
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
		template<EDimensionType eDefValue = typeDimensionVal>
		class CDimensionType : public CSimpleType<EDimensionType, eDefValue>
		{
		public:
			CDimensionType() {}
			CDimensionType(const EDimensionType & val)	{ this->m_eValue = val; }

			virtual EDimensionType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDimensionType)
		};
		enum EFormulaDirection
		{
			directionCol		=  0,
			directionRow		=  1
		};
		template<EFormulaDirection eDefValue = directionCol>
		class CFormulaDirection : public CSimpleType<EFormulaDirection, eDefValue>
		{
		public:
			CFormulaDirection() {}
			CFormulaDirection(const EFormulaDirection & val)	{ this->m_eValue = val; }

			virtual EFormulaDirection FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CFormulaDirection)
		};
		enum ESidePos
		{
			sideLeft		=  0,
			sideRight		=  1,
			sideTop			=  2,
			sideBottom		=  3
		};
		template<ESidePos eDefValue = sideTop>
		class CSidePos : public CSimpleType<ESidePos, eDefValue>
		{
		public:
			CSidePos() {}
			CSidePos(const ESidePos & val)	{ this->m_eValue = val; }

			virtual ESidePos FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CSidePos)
		};
		enum EPosAlign
		{
			posAlignMin		=  0,
			posAlignCtr		=  1,
			posAlignMax		=  2
		};
		template<EPosAlign eDefValue = posAlignCtr>
		class CPosAlign : public CSimpleType<EPosAlign, eDefValue>
		{
		public:
			CPosAlign() {}
			CPosAlign(const EPosAlign & val)	{ this->m_eValue = val; }

			virtual EPosAlign FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPosAlign)
		};
		enum ERegionLabelLayout
		{
			layoutNone			=  0,
			layoutBestFitOnly	=  1,
			layoutShowAll		=  2
		};
		template<ERegionLabelLayout eDefValue = layoutNone>
		class CRegionLabelLayout : public CSimpleType<ERegionLabelLayout, eDefValue>
		{
		public:
			CRegionLabelLayout() {}
			CRegionLabelLayout(const ERegionLabelLayout & val)	{ this->m_eValue = val; }

			virtual ERegionLabelLayout FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CRegionLabelLayout)
		};
		enum EParentLabelLayout
		{
			layoutNone_			=  0,
			layoutBanner		=  1,
			layoutOverlapping	=  2
		};
		template<EParentLabelLayout eDefValue = layoutNone_>
		class CParentLabelLayout : public CSimpleType<EParentLabelLayout, eDefValue>
		{
		public:
			CParentLabelLayout() {}
			CParentLabelLayout(const EParentLabelLayout & val)	{ this->m_eValue = val; }

			virtual EParentLabelLayout FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CParentLabelLayout)
		};
		
		enum EQuartileMethod
		{
			methodInclusive		=  0,
			methodExclusive		=  1,
		};
		template<EQuartileMethod eDefValue = methodInclusive>
		class CQuartileMethod : public CSimpleType<EQuartileMethod, eDefValue>
		{
		public:
			CQuartileMethod() {}
			CQuartileMethod(const EQuartileMethod & val)	{ this->m_eValue = val; }

			virtual EQuartileMethod FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CQuartileMethod)
		};

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
		template<EDataLabelPos eDefValue = posBestFit>
		class CDataLabelPos : public CSimpleType<EDataLabelPos, eDefValue>
		{
		public:
			CDataLabelPos() {}
			CDataLabelPos(const EDataLabelPos & val)	{ this->m_eValue = val; }

			virtual EDataLabelPos FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDataLabelPos)
		};

		template<ESidePos eDefValue = sideLeft>
		class CIntervalClosedSide : public CSimpleType<ESidePos, eDefValue>
		{
		public:
			CIntervalClosedSide() {}
			CIntervalClosedSide(const ESidePos & val)	{ this->m_eValue = val; }

			virtual ESidePos FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CIntervalClosedSide)
		};
		
		enum ETickMarksType
		{
			marksTypeIn		=  0,
			marksTypeOut	=  1,
			marksTypeCross	=  2,
			marksTypeNone	=  3

		};
		template<ETickMarksType eDefValue = marksTypeNone>
		class CTickMarksType : public CSimpleType<ETickMarksType, eDefValue>
		{
		public:
			CTickMarksType() {}
			CTickMarksType(const ETickMarksType & val)	{ this->m_eValue = val; }

			virtual ETickMarksType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CTickMarksType)
		};
		enum ESourceCacheType
		{
			typeSourceConsolidation	=  0,
			typeSourceExternal		=  1,
			typeSourceScenario		=  2,
			typeSourceWorksheet		=  3,
		};
		template<ESourceCacheType eDefValue = typeSourceWorksheet>
		class CSourceCacheType : public CSimpleType<ESourceCacheType, eDefValue>
		{
		public:
			CSourceCacheType() {}
			CSourceCacheType(const ESourceCacheType & val)	{ this->m_eValue = val; }

			virtual ESourceCacheType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CSourceCacheType)
		};
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
		template<EValuesGroupBy eDefValue = groupByMonths>
		class CValuesGroupBy : public CSimpleType<EValuesGroupBy, eDefValue>
		{
		public:
			CValuesGroupBy() {}
			CValuesGroupBy(const EValuesGroupBy & val)	{ this->m_eValue = val; }

			virtual EValuesGroupBy FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CValuesGroupBy)
		};
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
		template<EPivotItemType eDefValue = typeSum>
		class CPivotItemType : public CSimpleType<EPivotItemType, eDefValue>
		{
		public:
			CPivotItemType() {}
			CPivotItemType(const EPivotItemType & val)	{ this->m_eValue = val; }

			virtual EPivotItemType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPivotItemType)
		};
		enum EFieldSortType
		{
			sortManual		=  0,
			sortAscending	=  1,
			sortDescending	=  2
		};
		template<EFieldSortType eDefValue = sortManual>
		class CFieldSortType : public CSimpleType<EFieldSortType, eDefValue>
		{
		public:
			CFieldSortType() {}
			CFieldSortType(const EFieldSortType & val)	{ this->m_eValue = val; }

			virtual EFieldSortType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CFieldSortType)
		};
		
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
		template<EPivotAreaType eDefValue = areaNone>
		class CPivotAreaType : public CSimpleType<EPivotAreaType, eDefValue>
		{
		public:
			CPivotAreaType() {}
			CPivotAreaType(const EPivotAreaType & val)	{ this->m_eValue = val; }

			virtual EPivotAreaType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPivotAreaType)
		};
		enum EPivotAxisType
		{
			axisCol		=  0,
			axisPage	=  1,
			axisRow		=  2,
			axisValues	=  3
		};
		template<EPivotAxisType eDefValue = axisCol>
		class CPivotAxisType : public CSimpleType<EPivotAxisType, eDefValue>
		{
		public:
			CPivotAxisType() {}
			CPivotAxisType(const EPivotAxisType & val)	{ this->m_eValue = val; }

			virtual EPivotAxisType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CPivotAxisType)
		};
		enum EFormatAction
		{
			actionBlank		=  0,
			actionDrill		=  1,
			actionFormatting=  2,
			actionFormula	=  3
		};
		template<EFormatAction eDefValue = actionBlank>
		class CFormatAction : public CSimpleType<EFormatAction, eDefValue>
		{
		public:
			CFormatAction() {}
			CFormatAction(const EFormatAction & val)	{ this->m_eValue = val; }

			virtual EFormatAction FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CFormatAction)
		};
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
		template<EShowDataAs eDefValue = dataAsRunTotal>
		class CShowDataAs : public CSimpleType<EShowDataAs, eDefValue>
		{
		public:
			CShowDataAs() {}
			CShowDataAs(const EShowDataAs & val)	{ this->m_eValue = val; }

			virtual EShowDataAs FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CShowDataAs)
		};
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
		template<EDataConsolidateFunction eDefValue = functionSum>
		class CDataConsolidateFunction : public CSimpleType<EDataConsolidateFunction, eDefValue>
		{
		public:
			CDataConsolidateFunction() {}
			CDataConsolidateFunction(const EDataConsolidateFunction & val)	{ this->m_eValue = val; }

			virtual EDataConsolidateFunction FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDataConsolidateFunction)
		};
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
		template<EDataValidationType eDefValue = validationTypeNone>
		class CDataValidationType : public CSimpleType<EDataValidationType, eDefValue>
		{
		public:
			CDataValidationType() {}
			CDataValidationType(const EDataValidationType & val)	{ this->m_eValue = val; }

			virtual EDataValidationType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDataValidationType)
		};
		enum EDataValidationErrorStyle
		{
			errorStyleStop			=  0,
			errorStyleWarning		=  1,
			errorStyleInformation	=  2
		};
		template<EDataValidationErrorStyle eDefValue = errorStyleStop>
		class CDataValidationErrorStyle : public CSimpleType<EDataValidationErrorStyle, eDefValue>
		{
		public:
			CDataValidationErrorStyle() {}
			CDataValidationErrorStyle(const EDataValidationErrorStyle & val)	{ this->m_eValue = val; }

			virtual EDataValidationErrorStyle FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDataValidationErrorStyle)
		};
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
		template<EDataValidationOperator eDefValue = operatorBetween>
		class CDataValidationOperator : public CSimpleType<EDataValidationOperator, eDefValue>
		{
		public:
			CDataValidationOperator() {}
			CDataValidationOperator(const EDataValidationOperator & val)	{ this->m_eValue = val; }

			virtual EDataValidationOperator FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;
		};
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
		template<EDataValidationImeMode eDefValue = imeModeNoControl>
		class CDataValidationImeMode : public CSimpleType<EDataValidationImeMode, eDefValue>
		{
		public:
			CDataValidationImeMode() {}
			CDataValidationImeMode(const EDataValidationImeMode & val)	{ this->m_eValue = val; }

			virtual EDataValidationImeMode FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDataValidationImeMode)
		};
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
		template<EObjectType eDefValue = objectButton>
		class CObjectType : public CSimpleType<EObjectType, eDefValue>
		{
		public:
			CObjectType() {}
			CObjectType(const EObjectType & val)	{ this->m_eValue = val; }

			virtual EObjectType FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CObjectType)
		};
		enum EChecked
		{
			valUnchecked	=  0,
			valChecked		=  1,
			valMixed		=  2
		};
		template<EChecked eDefValue = valUnchecked>
		class CChecked : public CSimpleType<EChecked, eDefValue>
		{
		public:
			CChecked() {}
			CChecked(const EChecked & val)	{ this->m_eValue = val; }

			virtual EChecked FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CChecked)
		};
		enum EDropStyle
		{
			valCombo		=  0,
			valComboedit	=  1,
			valSimple		=  2
		};
		template<EDropStyle eDefValue = valSimple>
		class CDropStyle : public CSimpleType<EDropStyle, eDefValue>
		{
		public:
			CDropStyle() {}
			CDropStyle(const EDropStyle & val)	{ this->m_eValue = val; }

			virtual EDropStyle FromString(const std::wstring &sValue);
			std::wstring ToVmlString() const;
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CDropStyle)
		};
		enum ESelType
		{
			valSingle	=  0,
			valMulti	=  1,
			valExtended	=  2
		};
		template<ESelType eDefValue = valSingle>
		class CSelType : public CSimpleType<ESelType, eDefValue>
		{
		public:
			CSelType() {}
			CSelType(const ESelType & val)	{ this->m_eValue = val; }

			virtual ESelType FromString(const std::wstring &sValue);
			std::wstring ToVmlString() const;
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CSelType)
		};
		enum EEditValidation
		{
			editText		=  0,
			editInteger		=  1,
			editNumber		=  2,
			editReference	=  3,
			editFormula		=  4
		};
		template<EEditValidation eDefValue = editText>
		class CEditValidation : public CSimpleType<EEditValidation, eDefValue>
		{
		public:
			CEditValidation() {}
			CEditValidation(const EEditValidation & val)	{ this->m_eValue = val; }

			virtual EEditValidation FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_Default(CEditValidation)
		};

		enum ETabularSlicerCacheSortOrder
		{
			ctabularslicercachesortorderAscending = 0,
			ctabularslicercachesortorderDescending = 1
		};

		template<ETabularSlicerCacheSortOrder eDefValue = ctabularslicercachesortorderAscending>
		class CTabularSlicerCacheSortOrder : public CSimpleType<ETabularSlicerCacheSortOrder, eDefValue>
		{
		public:
			CTabularSlicerCacheSortOrder(){}
			CTabularSlicerCacheSortOrder(const ETabularSlicerCacheSortOrder & val)	{ this->m_eValue = val; }
			virtual ETabularSlicerCacheSortOrder FromString(const std::wstring &sValue);
			ETabularSlicerCacheSortOrder FromStringA(const char* sValue);
			virtual std::wstring ToString() const;

			SimpleTypes_Default(CTabularSlicerCacheSortOrder)
		};
		enum ESlicerCacheCrossFilter
		{
			cslicercachecrossfilterNone = 0,
			cslicercachecrossfilterShowItemsWithDataAtTop = 1,
			cslicercachecrossfilterShowItemsWithNoData = 2
		};

		template<ESlicerCacheCrossFilter eDefValue = cslicercachecrossfilterNone>
		class CSlicerCacheCrossFilter : public CSimpleType<ESlicerCacheCrossFilter, eDefValue>
		{
		public:
			CSlicerCacheCrossFilter(){}
			CSlicerCacheCrossFilter(const ESlicerCacheCrossFilter & val)	{ this->m_eValue = val; }
			virtual ESlicerCacheCrossFilter FromString(const std::wstring &sValue);
			ESlicerCacheCrossFilter FromStringA(const char* sValue);
			virtual std::wstring ToString() const;

			SimpleTypes_Default(CSlicerCacheCrossFilter)
		};
		enum EOlapSlicerCacheSortOrder
		{
			colapslicercachesortorderNatural = 0,
			colapslicercachesortorderAscending = 1,
			colapslicercachesortorderDescending = 2
		};

		template<EOlapSlicerCacheSortOrder eDefValue = colapslicercachesortorderNatural>
		class COlapSlicerCacheSortOrder : public CSimpleType<EOlapSlicerCacheSortOrder, eDefValue>
		{
		public:
			COlapSlicerCacheSortOrder(){}
			COlapSlicerCacheSortOrder(const EOlapSlicerCacheSortOrder & val)	{ this->m_eValue = val; }
			virtual EOlapSlicerCacheSortOrder FromString(const std::wstring &sValue);
			EOlapSlicerCacheSortOrder FromStringA(const char* sValue);
			virtual std::wstring ToString() const;

			SimpleTypes_Default(COlapSlicerCacheSortOrder)
		};

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

		template<ESlicerStyleType eDefValue = cslicerstyletypeUnselectedItemWithData>
		class CSlicerStyleType : public CSimpleType<ESlicerStyleType, eDefValue>
		{
		public:
			CSlicerStyleType(){}
			virtual ESlicerStyleType FromString(const std::wstring &sValue);
			ESlicerStyleType FromStringA(const char* sValue);
			virtual std::wstring ToString() const;

			SimpleTypes_Default(CSlicerStyleType)
		};

	}// Spreadsheet
} // SimpleTypes
