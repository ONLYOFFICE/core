/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "../CommonInclude.h"

#include "ChartStyle.h"
#include "ChartColors.h"

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}
	namespace Spreadsheet
	{
		enum ST_PageSetupOrientation
		{
			st_pagesetuporientationDEFAULT = 0,
			st_pagesetuporientationPORTRAIT = 1,
			st_pagesetuporientationLANDSCAPE = 2
		};
		enum ST_DispBlanksAs
		{
			st_dispblanksasSPAN = 0,
			st_dispblanksasGAP = 1,
			st_dispblanksasZERO = 2
		};
		enum ST_LegendPos
		{
			st_legendposB = 0,
			st_legendposTR = 1,
			st_legendposL = 2,
			st_legendposR = 3,
			st_legendposT = 4
		};
		enum ST_LayoutTarget
		{
			st_layouttargetINNER = 0,
			st_layouttargetOUTER = 1
		};
		enum ST_LayoutMode
		{
			st_layoutmodeEDGE = 0,
			st_layoutmodeFACTOR = 1
		};
		enum ST_Orientation
		{
			st_orientationMAXMIN = 0,
			st_orientationMINMAX = 1
		};
		enum ST_AxPos
		{
			st_axposB = 0,
			st_axposL = 1,
			st_axposR = 2,
			st_axposT = 3
		};
		enum ST_TickMark
		{
			st_tickmarkCROSS = 0,
			st_tickmarkIN = 1,
			st_tickmarkNONE = 2,
			st_tickmarkOUT = 3
		};
		enum ST_TickLblPos
		{
			st_ticklblposHIGH = 0,
			st_ticklblposLOW = 1,
			st_ticklblposNEXTTO = 2,
			st_ticklblposNONE = 3
		};
		enum ST_Crosses
		{
			st_crossesAUTOZERO = 0,
			st_crossesMAX = 1,
			st_crossesMIN = 2
		};
		enum ST_TimeUnit
		{
			st_timeunitDAYS = 0,
			st_timeunitMONTHS = 1,
			st_timeunitYEARS = 2
		};
		enum ST_LblAlgn
		{
			st_lblalgnCTR = 0,
			st_lblalgnL = 1,
			st_lblalgnR = 2
		};
		enum ST_BuiltInUnit
		{
			st_builtinunitHUNDREDS = 0,
			st_builtinunitTHOUSANDS = 1,
			st_builtinunitTENTHOUSANDS = 2,
			st_builtinunitHUNDREDTHOUSANDS = 3,
			st_builtinunitMILLIONS = 4,
			st_builtinunitTENMILLIONS = 5,
			st_builtinunitHUNDREDMILLIONS = 6,
			st_builtinunitBILLIONS = 7,
			st_builtinunitTRILLIONS = 8
		};
		enum ST_CrossBetween
		{
			st_crossbetweenBETWEEN = 0,
			st_crossbetweenMIDCAT = 1
		};
		enum ST_SizeRepresents
		{
			st_sizerepresentsAREA = 0,
			st_sizerepresentsW = 1
		};
		enum ST_MarkerStyle
		{
			st_markerstyleCIRCLE = 0,
			st_markerstyleDASH = 1,
			st_markerstyleDIAMOND = 2,
			st_markerstyleDOT = 3,
			st_markerstyleNONE = 4,
			st_markerstylePICTURE = 5,
			st_markerstylePLUS = 6,
			st_markerstyleSQUARE = 7,
			st_markerstyleSTAR = 8,
			st_markerstyleTRIANGLE = 9,
			st_markerstyleX = 10,
			st_markerstyleAUTO = 11
		};
		enum ST_PictureFormat
		{
			st_pictureformatSTRETCH = 0,
			st_pictureformatSTACK = 1,
			st_pictureformatSTACKSCALE = 2
		};
		enum ST_DLblPos
		{
			st_dlblposBESTFIT = 0,
			st_dlblposB = 1,
			st_dlblposCTR = 2,
			st_dlblposINBASE = 3,
			st_dlblposINEND = 4,
			st_dlblposL = 5,
			st_dlblposOUTEND = 6,
			st_dlblposR = 7,
			st_dlblposT = 8
		};
		enum ST_TrendlineType
		{
			st_trendlinetypeEXP = 0,
			st_trendlinetypeLINEAR = 1,
			st_trendlinetypeLOG = 2,
			st_trendlinetypeMOVINGAVG = 3,
			st_trendlinetypePOLY = 4,
			st_trendlinetypePOWER = 5
		};
		enum ST_ErrDir
		{
			st_errdirX = 0,
			st_errdirY = 1
		};
		enum ST_ErrBarType
		{
			st_errbartypeBOTH = 0,
			st_errbartypeMINUS = 1,
			st_errbartypePLUS = 2
		};
		enum ST_ErrValType
		{
			st_errvaltypeCUST = 0,
			st_errvaltypeFIXEDVAL = 1,
			st_errvaltypePERCENTAGE = 2,
			st_errvaltypeSTDDEV = 3,
			st_errvaltypeSTDERR = 4
		};
		enum ST_SplitType
		{
			st_splittypeAUTO = 0,
			st_splittypeCUST = 1,
			st_splittypePERCENT = 2,
			st_splittypePOS = 3,
			st_splittypeVAL = 4
		};
		enum ST_OfPieType
		{
			st_ofpietypePIE = 0,
			st_ofpietypeBAR = 1
		};
		enum ST_BarDir
		{
			st_bardirBAR = 0,
			st_bardirCOL = 1
		};
		enum ST_BarGrouping
		{
			st_bargroupingPERCENTSTACKED = 0,
			st_bargroupingCLUSTERED = 1,
			st_bargroupingSTANDARD = 2,
			st_bargroupingSTACKED = 3
		};
		enum ST_Shape
		{
			st_shapeCONE = 0,
			st_shapeCONETOMAX = 1,
			st_shapeBOX = 2,
			st_shapeCYLINDER = 3,
			st_shapePYRAMID = 4,
			st_shapePYRAMIDTOMAX = 5
		};
		enum ST_ScatterStyle
		{
			st_scatterstyleNONE = 0,
			st_scatterstyleLINE = 1,
			st_scatterstyleLINEMARKER = 2,
			st_scatterstyleMARKER = 3,
			st_scatterstyleSMOOTH = 4,
			st_scatterstyleSMOOTHMARKER = 5
		};
		enum ST_RadarStyle
		{
			st_radarstyleSTANDARD = 0,
			st_radarstyleMARKER = 1,
			st_radarstyleFILLED = 2
		};
		enum ST_Grouping
		{
			st_groupingPERCENTSTACKED = 0,
			st_groupingSTANDARD = 1,
			st_groupingSTACKED = 2
		};
		bool ToXml_ST_DispBlanksAs(ST_DispBlanksAs val, std::wstring& sOut);
		bool FromXml_ST_DispBlanksAs(const std::wstring& val, ST_DispBlanksAs& eOut);
		
		DEFINE_SIMPLE_TYPE(CGrouping, ST_Grouping, st_groupingSTANDARD)
		DEFINE_SIMPLE_TYPE(CRadarStyle, ST_RadarStyle, st_radarstyleSTANDARD)
		DEFINE_SIMPLE_TYPE(CScatterStyle, ST_ScatterStyle, st_scatterstyleMARKER)
		DEFINE_SIMPLE_TYPE(CShapeType, ST_Shape, st_shapeBOX)
		DEFINE_SIMPLE_TYPE(CBarGrouping, ST_BarGrouping, st_bargroupingCLUSTERED)
		DEFINE_SIMPLE_TYPE(CBarDir, ST_BarDir, st_bardirBAR)
		DEFINE_SIMPLE_TYPE(COfPieType, ST_OfPieType, st_ofpietypePIE)
		DEFINE_SIMPLE_TYPE(CSplitType, ST_SplitType, st_splittypeAUTO)
		DEFINE_SIMPLE_TYPE(CErrValType, ST_ErrValType, st_errvaltypeSTDERR)
		DEFINE_SIMPLE_TYPE(CErrBarType, ST_ErrBarType, st_errbartypeBOTH)
		DEFINE_SIMPLE_TYPE(CErrDir, ST_ErrDir, st_errdirX)
		DEFINE_SIMPLE_TYPE(CTrendlineType, ST_TrendlineType, st_trendlinetypeLINEAR)
		DEFINE_SIMPLE_TYPE(CDLblPos, ST_DLblPos, st_dlblposBESTFIT)		
		DEFINE_SIMPLE_TYPE(CPictureFormat, ST_PictureFormat, st_pictureformatSTRETCH)
		DEFINE_SIMPLE_TYPE(CMarkerStyle, ST_MarkerStyle, st_markerstyleAUTO)
		DEFINE_SIMPLE_TYPE(CSizeRepresents, ST_SizeRepresents, st_sizerepresentsAREA)
		DEFINE_SIMPLE_TYPE(CCrossBetween, ST_CrossBetween, st_crossbetweenBETWEEN)
		DEFINE_SIMPLE_TYPE(CBuiltInUnit, ST_BuiltInUnit, st_builtinunitHUNDREDS)
		DEFINE_SIMPLE_TYPE(CLblAlgn, ST_LblAlgn, st_lblalgnCTR)
		DEFINE_SIMPLE_TYPE(CTimeUnit, ST_TimeUnit, st_timeunitYEARS)
		DEFINE_SIMPLE_TYPE(CCrosses, ST_Crosses, st_crossesAUTOZERO)
		DEFINE_SIMPLE_TYPE(CTickLblPos, ST_TickLblPos, st_ticklblposNEXTTO)
		DEFINE_SIMPLE_TYPE(CTickMark, ST_TickMark, st_tickmarkCROSS)
		DEFINE_SIMPLE_TYPE(CAxPos, ST_AxPos, st_axposB)
		DEFINE_SIMPLE_TYPE(COrientation, ST_Orientation, st_orientationMINMAX)
		DEFINE_SIMPLE_TYPE(CLayoutMode, ST_LayoutMode, st_layoutmodeEDGE)
		DEFINE_SIMPLE_TYPE(CLayoutTarget, ST_LayoutTarget, st_layouttargetINNER)
		DEFINE_SIMPLE_TYPE(CLegendPos, ST_LegendPos, st_legendposR)
		DEFINE_SIMPLE_TYPE(CDispBlanksAs, ST_DispBlanksAs, st_dispblanksasZERO)
		DEFINE_SIMPLE_TYPE(CPageSetupOrientation, ST_PageSetupOrientation, st_pagesetuporientationDEFAULT)
//----------------------------
		template<class T>
		class nullableComplexVal : public nullable<T>
		{
		public:
			nullableComplexVal() : nullable<T>()
			{
			}
			nullableComplexVal(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsValid())
				{
					T* pType = new T();

					WritingElement_ReadAttributes_Start_No_NS(oReader)
						WritingElement_ReadAttributes_ReadSingle(oReader, L"val", *pType)
					WritingElement_ReadAttributes_End_No_NS(oReader)

					if (!oReader.IsEmptyNode())
						oReader.ReadTillEnd();

					this->m_pPointer = pType;
				}
				else
					this->m_pPointer = NULL;
			}
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
			{
				if (this->m_pPointer)
				{
					writer.WriteString(L"<" + sNodeName + L" val=\"" + this->m_pPointer->ToString() + L"\"/>");
				}
			}
		};
		class nullableBoolVal : public nullable_bool
		{
		public:
			nullableBoolVal() : nullable_bool()
			{
			}
			void operator=(const bool& value)
			{
				nullable_bool::operator=(value);
			}
			nullableBoolVal(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsValid())
				{
					WritingElement_ReadAttributes_Start_No_NS(oReader)
						if (L"val" == wsName)
						{
							this->m_pPointer = new bool(XmlUtils::GetBoolean2(oReader.GetText()));
							break;
						}
					WritingElement_ReadAttributes_End_No_NS(oReader)

					if (!oReader.IsEmptyNode())
						oReader.ReadTillEnd();
				}
				else
					this->m_pPointer = NULL;
			}
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
			{
				if (this->m_pPointer)
				{
					writer.WriteString(L"<" + sNodeName + L" val=\"" + (*this->m_pPointer ? L"1" : L"0") + L"\"/>");
				}
			}
		};
		class nullableIntVal : public nullable_int
		{
		public:
			nullableIntVal() : nullable_int()
			{
			}
			void operator=(const int& value)
			{
				nullable_int::operator=(value);
			}
			nullableIntVal(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsValid())
				{
					WritingElement_ReadAttributes_Start_No_NS(oReader)
						if (L"val" == wsName)
						{
							this->m_pPointer = new int(XmlUtils::GetInteger(oReader.GetText()));
							break;
						}
					WritingElement_ReadAttributes_End_No_NS(oReader)

						if (!oReader.IsEmptyNode())
							oReader.ReadTillEnd();
				}
				else
					this->m_pPointer = NULL;
			}
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
			{
				if (this->m_pPointer)
				{
					writer.WriteString(L"<" + sNodeName + L" val=\"" + std::to_wstring(*this->m_pPointer) + L"\"/>");
				}
			}
		};
		class nullableUintVal : public nullable_uint
		{
		public:
			nullableUintVal() : nullable_uint()
			{
			}
			void operator=(const unsigned int& value)
			{
				nullable_uint::operator=(value);
			}
			nullableUintVal(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsValid())
				{
					WritingElement_ReadAttributes_Start_No_NS(oReader)
						if (L"val" == wsName)
						{
							this->m_pPointer = new unsigned int(XmlUtils::GetUInteger(oReader.GetText()));
							break;
						}
					WritingElement_ReadAttributes_End_No_NS(oReader)

					if (!oReader.IsEmptyNode())
						oReader.ReadTillEnd();
				}
				else
					this->m_pPointer = NULL;
			}
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
			{
				if (this->m_pPointer)
				{
					writer.WriteString(L"<" + sNodeName + L" val=\"" + std::to_wstring(*this->m_pPointer) + L"\"/>");
				}
			}
		};
		class nullableDoubleVal : public nullable_double
		{
		public:
			nullableDoubleVal() : nullable_double()
			{
			}
			void operator=(const double& value)
			{
				nullable_double::operator=(value);
			}
			nullableDoubleVal(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsValid())
				{
					WritingElement_ReadAttributes_Start_No_NS(oReader)
						if (L"val" == wsName)
						{
							this->m_pPointer = new double(XmlUtils::GetDouble(oReader.GetText()));
							break; 
						}
					WritingElement_ReadAttributes_End_No_NS(oReader)

					if (!oReader.IsEmptyNode())
						oReader.ReadTillEnd();
				}
				else
					this->m_pPointer = NULL;
			}
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
			{
				if (this->m_pPointer)
				{
					writer.WriteString(L"<" + sNodeName + L" val=\"" + std::to_wstring(*this->m_pPointer) + L"\"/>");
				}
			}
		};
		class nullableStringVal : public nullable_string
		{
		public:
			nullableStringVal() : nullable_string()
			{
			}
			void operator=(const std::wstring& value)
			{
				nullable_string::operator=(value);
			}
			nullableStringVal(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsValid())
				{
					std::wstring* pType = new std::wstring();

					WritingElement_ReadAttributes_Start_No_NS(oReader)
						WritingElement_ReadAttributes_ReadSingle(oReader, L"val", *pType)
					WritingElement_ReadAttributes_End_No_NS(oReader)

					if (!oReader.IsEmptyNode())
						oReader.ReadTillEnd();

					this->m_pPointer = pType;
				}
				else
					this->m_pPointer = NULL;
			}
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
			{
				if (this->m_pPointer)
				{
					writer.WriteString(L"<" + sNodeName + L" val=\"" + XmlUtils::EncodeXmlString(*this->m_pPointer) + L"\"/>");
				}
			}
		};
		//----------------------------		
		enum ItemsChoiceType3
		{
			itemschoicetype3DLBLPOS = 0,
			itemschoicetype3DELETE = 1,
			itemschoicetype3LEADERLINES = 2,
			itemschoicetype3NUMFMT = 3,
			itemschoicetype3SEPARATOR = 4,
			itemschoicetype3SHOWBUBBLESIZE = 5,
			itemschoicetype3SHOWCATNAME = 6,
			itemschoicetype3SHOWLEADERLINES = 7,
			itemschoicetype3SHOWLEGENDKEY = 8,
			itemschoicetype3SHOWPERCENT = 9,
			itemschoicetype3SHOWSERNAME = 10,
			itemschoicetype3SHOWVAL = 11,
			itemschoicetype3SPPR = 12,
			itemschoicetype3TXPR = 13
		};
		enum ItemsChoiceType4
		{
			itemschoicetype4DLBLPOS = 0,
			itemschoicetype4DELETE = 1,
			itemschoicetype4LAYOUT = 2,
			itemschoicetype4NUMFMT = 3,
			itemschoicetype4SEPARATOR = 4,
			itemschoicetype4SHOWBUBBLESIZE = 5,
			itemschoicetype4SHOWCATNAME = 6,
			itemschoicetype4SHOWLEGENDKEY = 7,
			itemschoicetype4SHOWPERCENT = 8,
			itemschoicetype4SHOWSERNAME = 9,
			itemschoicetype4SHOWVAL = 10,
			itemschoicetype4SPPR = 11,
			itemschoicetype4TX = 12,
			itemschoicetype4TXPR = 13
		};
		enum ItemsChoiceType5 // -> to BaseChart
		{
			itemschoicetype5AREA3DCHART = 0,
			itemschoicetype5AREACHART = 1,
			itemschoicetype5BAR3DCHART = 2,
			itemschoicetype5BARCHART = 3,
			itemschoicetype5BUBBLECHART = 4,
			itemschoicetype5DOUGHNUTCHART = 5,
			itemschoicetype5LINE3DCHART = 6,
			itemschoicetype5LINECHART = 7,
			itemschoicetype5OFPIECHART = 8,
			itemschoicetype5PIE3DCHART = 9,
			itemschoicetype5PIECHART = 10,
			itemschoicetype5RADARCHART = 11,
			itemschoicetype5SCATTERCHART = 12,
			itemschoicetype5STOCKCHART = 13,
			itemschoicetype5SURFACE3DCHART = 14,
			itemschoicetype5SURFACECHART = 15
		};
		enum ItemsChoiceType6
		{
			itemschoicetype6CATAX = 0,
			itemschoicetype6DATEAX = 1,
			itemschoicetype6SERAX = 2,
			itemschoicetype6VALAX = 3
		};
//---------------------------------------------------------------------------------------------------
		class CT_Style
		{
		public:
			std::wstring m_namespace;
			nullable_uint m_val;

			CT_Style();
			~CT_Style();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class AlternateContentChoice
		{
		public:
			CT_Style* m_style;
			nullable_string m_Requires;

			AlternateContentChoice();
			~AlternateContentChoice();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class AlternateContentFallback
		{
		public:
			CT_Style* m_style;

			AlternateContentFallback();
			~AlternateContentFallback();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class AlternateContent
		{
		public:
			std::vector<AlternateContentChoice*> m_Choice;
			AlternateContentFallback* m_Fallback;

			AlternateContent();
			~AlternateContent();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};

		class CT_RelId
		{
		public:
			nullable_string m_id;

			CT_RelId();
			~CT_RelId();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: 
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CT_PageSetup
		{
		public:
			nullable_uint m_paperSize;
			nullable_string m_paperHeight;
			nullable_string m_paperWidth;
			nullable_uint m_firstPageNumber;
			nullable<CPageSetupOrientation> m_orientation;
			nullable_bool m_blackAndWhite;
			nullable_bool m_draft;
			nullable_bool m_useFirstPageNumber;
			nullable_uint m_horizontalDpi; // -?
			nullable_uint m_verticalDpi;
			nullable_uint m_copies;

			CT_PageSetup();
			~CT_PageSetup();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: 
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_PageMargins
		{
		public:
			nullable_double m_l;
			nullable_double m_r;
			nullable_double m_t;
			nullable_double m_b;
			nullable_double m_header;
			nullable_double m_footer;

			CT_PageMargins();
			~CT_PageMargins();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_HeaderFooter
		{
		public:
			nullable_string m_oddHeader;
			nullable_string m_oddFooter;
			nullable_string m_evenHeader;
			nullable_string m_evenFooter;
			nullable_string m_firstHeader;
			nullable_string m_firstFooter;

			nullable_bool m_alignWithMargins;
			nullable_bool m_differentOddEven;
			nullable_bool m_differentFirst;
			
			CT_HeaderFooter();
			~CT_HeaderFooter();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: 
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_PrintSettings
		{
		public:
			CT_HeaderFooter* m_headerFooter;
			CT_PageMargins* m_pageMargins;
			CT_PageSetup* m_pageSetup;
			
			CT_PrintSettings();
			~CT_PrintSettings();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ExternalData
		{
		public:
			nullableBoolVal m_autoUpdate;
			nullable_string m_id;

			CT_ExternalData();
			~CT_ExternalData();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_LegendEntry
		{
		public:
			nullableUintVal					m_idx;
			nullableBoolVal					m_delete;
			nullable<PPTX::Logic::TxBody>	m_txPr;	
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_LegendEntry();
			~CT_LegendEntry();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ManualLayout
		{
		public:
			nullableComplexVal<CLayoutTarget> m_layoutTarget;
			nullableComplexVal<CLayoutMode> m_xMode;
			nullableComplexVal<CLayoutMode> m_yMode;
			nullableComplexVal<CLayoutMode> m_wMode;
			nullableComplexVal<CLayoutMode> m_hMode;
			nullableDoubleVal m_x;
			nullableDoubleVal m_y;
			nullableDoubleVal m_w;
			nullableDoubleVal m_h;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_ManualLayout();
			~CT_ManualLayout();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Layout
		{
		public:
			CT_ManualLayout* m_manualLayout;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Layout();
			~CT_Layout();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Legend
		{
		public:
			nullableComplexVal<CLegendPos>	m_legendPos;
            std::vector<CT_LegendEntry*>	m_legendEntry;
			CT_Layout						*m_layout;
			nullableBoolVal					m_overlay;
			nullable<PPTX::Logic::SpPr>		m_spPr;
			nullable<PPTX::Logic::TxBody>	m_txPr;	
			
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;
	/*--------------------------------------------------*/
			CT_Legend();
			~CT_Legend();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};

		class CT_DTable
		{
		public:
			nullableBoolVal m_showHorzBorder;
			nullableBoolVal m_showVertBorder;
			nullableBoolVal m_showOutline;
			nullableBoolVal m_showKeys;
			nullable<PPTX::Logic::SpPr>		m_spPr;
			nullable<PPTX::Logic::TxBody>	m_txPr;	
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;
			
			CT_DTable();
			~CT_DTable();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Scaling
		{
		public:
			nullableDoubleVal					m_logBase;
			nullableComplexVal<COrientation>	m_orientation;
			nullableDoubleVal					m_max;
			nullableDoubleVal					m_min;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Scaling();
			~CT_Scaling();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ChartLines
		{
		public:
			nullable<PPTX::Logic::SpPr> m_spPr;
			/*--------------------------------------------------------------*/
			CT_ChartLines();
			~CT_ChartLines();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_StrVal
		{
		public:
			nullable_string m_v;
			nullable_uint m_idx;

			CT_StrVal();
			~CT_StrVal();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_StrData : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CT_StrData)

			nullableUintVal			m_ptCount;
			std::vector<CT_StrVal*> m_pt;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_StrData();
			~CT_StrData();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			virtual std::wstring toXML() const;
			void toXML(NSStringUtils::CStringBuilder& writer) const;

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_StrRef
		{
		public:
			nullable_string m_f;
			CT_StrData		* m_strCache;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_StrRef();
			~CT_StrRef();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Tx : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CT_Tx)

			nullable<PPTX::Logic::TxBody>	m_oRich;
			CT_StrRef						*m_strRef;

			CT_Tx();
			virtual ~CT_Tx();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			virtual std::wstring toXML() const;
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Title
		{
		public:
			CT_Tx								*m_tx;
			CT_Layout							*m_layout;
			nullableBoolVal						m_overlay;
			nullable<PPTX::Logic::SpPr>			m_spPr;
			nullable<PPTX::Logic::TxBody>		m_txPr;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Title();
			~CT_Title();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_NumFmt
		{
		public:
			nullable_string m_formatCode;
			nullable_bool m_sourceLinked;

			CT_NumFmt();
			~CT_NumFmt();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CBaseAx
		{
		public:
			CBaseAx();
			virtual ~CBaseAx();

			bool fromXML(const std::wstring & nodeName, XmlUtils::CXmlLiteReader& oReader);
			void toXML(NSStringUtils::CStringBuilder& writer) const;

			nullableIntVal					m_axId;
			CT_Scaling						*m_scaling;
			nullableBoolVal					m_delete;
			nullableComplexVal<CAxPos>		m_axPos;
			CT_ChartLines					*m_majorGridlines;
			CT_ChartLines					*m_minorGridlines;
			CT_Title						*m_title;
			CT_NumFmt						*m_numFmt;
			nullableComplexVal<CTickMark>	m_majorTickMark;
			nullableComplexVal<CTickMark>	m_minorTickMark;
			nullableComplexVal<CTickLblPos> m_tickLblPos;
			nullable<PPTX::Logic::SpPr>		m_spPr;
			nullable<PPTX::Logic::TxBody>	m_txPr;
			nullableIntVal					m_crossAx;
			nullableComplexVal<CCrosses>	m_crosses;
			nullableDoubleVal				m_crossesAt;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;
		};
		class CT_SerAx : public CBaseAx
		{
		public:
			nullableUintVal m_tickLblSkip;
			nullableUintVal m_tickMarkSkip;

			CT_SerAx();
			virtual ~CT_SerAx();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DateAx : public CBaseAx
		{
		public:
			nullableBoolVal					m_auto;
			nullableStringVal				m_lblOffset;
			nullableComplexVal<CTimeUnit>	m_baseTimeUnit;
			nullableDoubleVal				m_majorUnit;
			nullableComplexVal<CTimeUnit>	m_majorTimeUnit;
			nullableDoubleVal				m_minorUnit;
			nullableComplexVal<CTimeUnit>	m_minorTimeUnit;
			
			CT_DateAx();
			virtual ~CT_DateAx();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};

		class CT_CatAx : public CBaseAx
		{
		public:
			nullableBoolVal					m_auto;
			nullableComplexVal<CLblAlgn>	m_lblAlgn;
			nullableStringVal				m_lblOffset;
			nullableUintVal					m_tickLblSkip;
			nullableUintVal					m_tickMarkSkip;
			nullableBoolVal					m_noMultiLvlLbl;

			CT_CatAx();
			virtual ~CT_CatAx();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DispUnitsLbl
		{
		public:
			CT_Layout*						m_layout;
			CT_Tx*							m_tx;
			nullable<PPTX::Logic::SpPr>		m_spPr;
			nullable<PPTX::Logic::TxBody>	m_txPr;	

			CT_DispUnitsLbl();
			~CT_DispUnitsLbl();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DispUnits
		{
		public:
			nullableComplexVal<CBuiltInUnit> m_builtInUnit;
			nullableDoubleVal m_custUnit;
			CT_DispUnitsLbl* m_dispUnitsLbl;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_DispUnits();
			~CT_DispUnits();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ValAx : public CBaseAx
		{
		public:
			nullableComplexVal<CCrossBetween>	m_crossBetween;
			nullableDoubleVal					m_majorUnit;
			nullableDoubleVal					m_minorUnit;
			CT_DispUnits*						m_dispUnits;

			CT_ValAx();
			virtual ~CT_ValAx();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SerTx
		{
		public:
			CT_StrRef* m_strRef;
			nullable_string m_v;

			CT_SerTx();
			~CT_SerTx();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DLbl : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CT_DLbl)

			nullableUintVal m_idx;
			std::vector<ItemsChoiceType4*> m_ItemsElementName0;
			std::vector<void*> m_Items;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;
			//ext
			nullableBoolVal m_xForSave;
			nullableBoolVal m_showDataLabelsRange;
			//CT_DlblFieldTable* m_dlblFieldTable

			nullableBoolVal m_showLeaderLines;		//for CT_DLbls
			CT_ChartLines* m_leaderLines;		//for CT_DLbls

			CT_DLbl();
			~CT_DLbl();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			virtual std::wstring toXML() const;
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			void toXMLEntry(const std::wstring& sNodeNS, NSStringUtils::CStringBuilder& writer) const;

			EElementType getType();
		private:
			void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType4 eType, void* pVal, const std::wstring & nodeNS) const;
			void toDelete(ItemsChoiceType4 eType, void* pVal);
		};
		class CT_DLbls
		{
		public:
			std::vector<CT_DLbl*>			m_dLbl;
			std::vector<ItemsChoiceType3*>	m_ItemsElementName0;
			std::vector<void*>				m_Items;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_DLbls();
			~CT_DLbls();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private:
			void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType3 eType, void* pVal) const;
			void toDelete(ItemsChoiceType3 eType, void* pVal);
		};
		class CT_lvl
		{
		public:
			std::vector<CT_StrVal*> m_pt;

			CT_lvl();
			~CT_lvl();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_MultiLvlStrData
		{
		public:
			nullableUintVal m_ptCount;
			std::vector<CT_lvl*> m_lvl;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_MultiLvlStrData();
			~CT_MultiLvlStrData();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_MultiLvlStrRef
		{
		public:
			nullable_string m_f;
			CT_MultiLvlStrData* m_multiLvlStrCache;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_MultiLvlStrRef();
			~CT_MultiLvlStrRef();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_NumVal
		{
		public:
			nullable_string m_v;
			nullable_uint m_idx;
			nullable_string m_formatCode;

			CT_NumVal();
			~CT_NumVal();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_NumData
		{
		public:
			nullable_string m_formatCode;
			nullableUintVal m_ptCount;
			std::vector<CT_NumVal*> m_pt;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_NumData();
			~CT_NumData();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_NumRef
		{
		public:
			nullable_string m_f;
			CT_NumData* m_numCache;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_NumRef();
			~CT_NumRef();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_NumDataSource
		{
		public:
			CT_NumData* m_numLit;
			CT_NumRef* m_numRef;

			CT_NumDataSource();
			~CT_NumDataSource();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_AxDataSource : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CT_AxDataSource)
			
			CT_MultiLvlStrRef* m_multiLvlStrRef;
			CT_NumData* m_numLit;
			CT_NumRef* m_numRef;
			CT_StrData* m_strLit;
			CT_StrRef* m_strRef;

			CT_AxDataSource();
			virtual ~CT_AxDataSource();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			virtual std::wstring toXML() const;
			void toXML(NSStringUtils::CStringBuilder& writer) const;

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Marker : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CT_Marker)

			nullableComplexVal<CMarkerStyle>	m_symbol;
			nullableUintVal						m_size;
			nullable<PPTX::Logic::SpPr>			m_spPr;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Marker();
			~CT_Marker();
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			virtual std::wstring toXML() const;
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PictureOptions
		{
		public:
			nullableBoolVal m_applyToFront;
			nullableBoolVal m_applyToSides;
			nullableBoolVal m_applyToEnd;
			nullableComplexVal<CPictureFormat> m_pictureFormat;
			nullableDoubleVal m_pictureStackUnit;

			CT_PictureOptions();
			~CT_PictureOptions();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DPt
		{
		public:
			nullableUintVal				m_idx;
			nullableBoolVal				m_invertIfNegative;
			CT_Marker*					m_marker;
			nullableBoolVal				m_bubble3D;
			nullableUintVal				m_explosion;
			nullable<PPTX::Logic::SpPr>	m_spPr;
			CT_PictureOptions*			m_pictureOptions;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_DPt();
			~CT_DPt();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_TrendlineLbl
		{
		public:
			CT_Layout*								m_layout;
			CT_Tx*									m_tx;
			CT_NumFmt*								m_numFmt;
			nullable<PPTX::Logic::SpPr>				m_spPr;
			nullable<PPTX::Logic::TxBody>			m_txPr;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_TrendlineLbl();
			~CT_TrendlineLbl();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Trendline
		{
		public:
			nullable_string						m_name;
			nullable<PPTX::Logic::SpPr>			m_spPr;
			nullableComplexVal<CTrendlineType>	m_trendlineType;
			nullableUintVal		m_order;
			nullableUintVal		m_period;
			nullableDoubleVal	m_forward;
			nullableDoubleVal	m_backward;
			nullableDoubleVal	m_intercept;
			nullableBoolVal		m_dispRSqr;
			nullableBoolVal		m_dispEq;
			CT_TrendlineLbl* m_trendlineLbl;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Trendline();
			~CT_Trendline();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ErrBars
		{
		public:
			nullableComplexVal<CErrDir> m_errDir;
			nullableComplexVal<CErrBarType> m_errBarType;
			nullableComplexVal<CErrValType> m_errValType;
			nullableBoolVal m_noEndCap;
			CT_NumDataSource* m_plus;
			CT_NumDataSource* m_minus;
			nullableDoubleVal m_val;
			nullable<PPTX::Logic::SpPr>	m_spPr;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_ErrBars();
			~CT_ErrBars();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CBaseSer : public WritingElement
		{
		public:
			CBaseSer();
			virtual ~CBaseSer();

			nullableUintVal									m_idx;
			nullableUintVal									m_order;
			CT_SerTx*										m_tx;
			nullable<PPTX::Logic::SpPr>						m_spPr;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			bool fromXML(const std::wstring nodeName, XmlUtils::CXmlLiteReader& oReader);
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML()  const
			{
				return L"";
			}
		};
		//---------------------------------------------------------------------------------------------------
		class CT_BubbleSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_BubbleSer)

			nullableBoolVal				m_invertIfNegative;
            std::vector<CT_DPt*>		m_dPt;
            std::vector<CT_Trendline*>	m_trendline;
            std::vector<CT_ErrBars*>	m_errBars;
			CT_DLbls*					m_dLbls;
			CT_AxDataSource*			m_xVal;
			CT_NumDataSource*			m_yVal;
			CT_NumDataSource*			m_bubbleSize;
			nullableBoolVal				m_bubble3D;

			CT_BubbleSer();
			virtual  ~CT_BubbleSer();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};

		class CT_BubbleChart
		{
		public:
			nullableBoolVal m_varyColors;
            std::vector<CT_BubbleSer*> m_ser;
			CT_DLbls* m_dLbls;
			nullableBoolVal m_bubble3D;
			nullableStringVal m_bubbleScale;
			nullableBoolVal m_showNegBubbles;
			nullableComplexVal<CSizeRepresents> m_sizeRepresents;
			std::vector<_INT32> m_axId;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_BubbleChart();
			~CT_BubbleChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_BandFmt
		{
		public:
			nullableUintVal				m_idx;
			nullable<PPTX::Logic::SpPr>	m_spPr;

			CT_BandFmt();
			~CT_BandFmt();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_bandFmts
		{
		public:
            std::vector<CT_BandFmt*> m_bandFmt;

			CT_bandFmts();
			~CT_bandFmts();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SurfaceSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_SurfaceSer)

			CT_AxDataSource*	m_cat;
			CT_NumDataSource*	m_val;

			CT_SurfaceSer();
			virtual ~CT_SurfaceSer();			
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Surface3DChart
		{
		public:
			nullableBoolVal m_wireframe;
            std::vector<CT_SurfaceSer*> m_ser;
			CT_bandFmts* m_bandFmts;
			std::vector<_INT32> m_axId;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Surface3DChart();
			~CT_Surface3DChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};

		class CT_SurfaceChart
		{
		public:
			nullableBoolVal m_wireframe;
            std::vector<CT_SurfaceSer*> m_ser;
			CT_bandFmts* m_bandFmts;
			std::vector<_INT32> m_axId;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_SurfaceChart();
			~CT_SurfaceChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_custSplit
		{
		public:
            std::vector<unsigned int> m_secondPiePt;

			CT_custSplit();
			~CT_custSplit();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PieSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_PieSer)
			
			nullableUintVal m_explosion;
			std::vector<CT_DPt*> m_dPt;
			CT_DLbls* m_dLbls;
			CT_AxDataSource* m_cat;
			CT_NumDataSource* m_val;

			CT_PieSer();
			virtual ~CT_PieSer();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_OfPieChart
		{
		public:
			nullableComplexVal<COfPieType>	m_ofPieType;
			nullableBoolVal					m_varyColors;
            std::vector<CT_PieSer*>			m_ser;
			CT_DLbls*						m_dLbls;
			nullableStringVal				m_gapWidth;
			nullableComplexVal<CSplitType>	m_splitType;
			nullableDoubleVal				m_splitPos;
			CT_custSplit*					m_custSplit;
			nullableStringVal				m_secondPieSize;
            std::vector<CT_ChartLines*>		m_serLines;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_OfPieChart();
			~CT_OfPieChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_BarSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_BarSer)

			nullableBoolVal m_invertIfNegative;
			CT_PictureOptions* m_pictureOptions;
			std::vector<CT_DPt*> m_dPt;
			CT_DLbls* m_dLbls;
			std::vector<CT_Trendline*> m_trendline;
			CT_ErrBars* m_errBars;
			CT_AxDataSource* m_cat;
			CT_NumDataSource* m_val;
			nullableComplexVal<CShapeType> m_shape;

			CT_BarSer();
			virtual ~CT_BarSer();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			XLS::BaseObjectPtr GetXLSFormat()const;
			EElementType getType();
		};
		class CT_Bar3DChart
		{
		public:
			nullableComplexVal<CBarDir> m_barDir;
			nullableComplexVal<CBarGrouping> m_grouping;
			nullableBoolVal m_varyColors;
            std::vector<CT_BarSer*> m_ser;
			CT_DLbls* m_dLbls;
			nullableStringVal m_gapWidth;
			nullableStringVal m_gapDepth;
			nullableComplexVal<CShapeType> m_shape;
			std::vector<_INT32> m_axId;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Bar3DChart();
			~CT_Bar3DChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_BarChart
		{
		public:
			nullableComplexVal<CBarDir> m_barDir;
			nullableComplexVal<CBarGrouping> m_grouping;
			nullableBoolVal m_varyColors;
            std::vector<CT_BarSer*> m_ser;
			CT_DLbls* m_dLbls;
			nullableStringVal m_gapWidth;
			nullableStringVal m_overlap;
            std::vector<CT_ChartLines*> m_serLines;
			std::vector<_INT32> m_axId;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_BarChart();
			~CT_BarChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DoughnutChart
		{
		public:
			nullableBoolVal			m_varyColors;
            std::vector<CT_PieSer*> m_ser;
			CT_DLbls*				m_dLbls;
			nullableUintVal			m_firstSliceAng;
			nullableStringVal		m_holeSize;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_DoughnutChart();
			~CT_DoughnutChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Pie3DChart
		{
		public:
			nullableBoolVal m_varyColors;
            std::vector<CT_PieSer*> m_ser;
			CT_DLbls* m_dLbls;
			
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;
			
			CT_Pie3DChart();
			~CT_Pie3DChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PieChart
		{
		public:
			nullableBoolVal m_varyColors;
            std::vector<CT_PieSer*> m_ser;
			CT_DLbls* m_dLbls;
			nullableUintVal m_firstSliceAng;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_PieChart();
			~CT_PieChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ScatterSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_ScatterSer)

			CT_Marker* m_marker;
            std::vector<CT_DPt*> m_dPt;
			CT_DLbls* m_dLbls;
            std::vector<CT_Trendline*> m_trendline;
            std::vector<CT_ErrBars*> m_errBars;
			CT_AxDataSource* m_xVal;
			CT_NumDataSource* m_yVal;
			nullableBoolVal m_smooth;

			CT_ScatterSer();
			virtual ~CT_ScatterSer();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ScatterChart
		{
		public:
			nullableComplexVal<CScatterStyle> m_scatterStyle;
			nullableBoolVal m_varyColors;
            std::vector<CT_ScatterSer*> m_ser;
			CT_DLbls* m_dLbls;
			std::vector<_INT32> m_axId;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_ScatterChart();
			~CT_ScatterChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_RadarSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_RadarSer)

			CT_Marker* m_marker;
            std::vector<CT_DPt*> m_dPt;
			CT_DLbls* m_dLbls;
			CT_AxDataSource* m_cat;
			CT_NumDataSource* m_val;

			CT_RadarSer();
			virtual ~CT_RadarSer();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_RadarChart
		{
		public:
			nullableComplexVal<CRadarStyle> m_radarStyle;
			nullableBoolVal m_varyColors;
            std::vector<CT_RadarSer*> m_ser;
			CT_DLbls* m_dLbls;
			std::vector<_INT32> m_axId;
			
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;
			
			CT_RadarChart();
			~CT_RadarChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_UpDownBar
		{
		public:
			nullable<PPTX::Logic::SpPr>	m_spPr;

			CT_UpDownBar();
			~CT_UpDownBar();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_UpDownBars
		{
		public:
			nullableStringVal	m_gapWidth;
			CT_UpDownBar*	m_upBars;
			CT_UpDownBar*	m_downBars;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_UpDownBars();
			~CT_UpDownBars();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_LineSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_LineSer)

			CT_Marker* m_marker;
			std::vector<CT_DPt*> m_dPt;
			CT_DLbls* m_dLbls;
			std::vector<CT_Trendline*> m_trendline;
			CT_ErrBars* m_errBars;
			CT_AxDataSource* m_cat;
			CT_NumDataSource* m_val;
			nullableBoolVal m_smooth;

			CT_LineSer();
			virtual ~CT_LineSer();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};

		class CT_StockChart
		{
		public:
            std::vector<CT_LineSer*> m_ser;
			CT_DLbls* m_dLbls;
			CT_ChartLines* m_dropLines;
			CT_ChartLines* m_hiLowLines;
			CT_UpDownBars* m_upDownBars;
			std::vector<_INT32> m_axId;
			
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_StockChart();
			~CT_StockChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Line3DChart
		{
		public:
			nullableComplexVal<CGrouping> m_grouping;
			nullableBoolVal m_varyColors;
            std::vector<CT_LineSer*> m_ser;
			CT_DLbls* m_dLbls;
			CT_ChartLines* m_dropLines;
			nullableStringVal m_gapDepth;
			std::vector<_INT32> m_axId;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Line3DChart();
			~CT_Line3DChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_LineChart
		{
		public:
			nullableComplexVal<CGrouping>	m_grouping;
			nullableBoolVal					m_varyColors;
            std::vector<CT_LineSer*>		m_ser;
			CT_DLbls*						m_dLbls;
			CT_ChartLines*					m_dropLines;
			CT_ChartLines*					m_hiLowLines;
			CT_UpDownBars*					m_upDownBars;
			nullableBoolVal					m_marker;
			nullableBoolVal					m_smooth;
            std::vector<_INT32>				m_axId;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_LineChart();
			~CT_LineChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_AreaSer : public CBaseSer
		{
		public:
			WritingElement_AdditionMethods(CT_AreaSer)
				
			CT_PictureOptions* m_pictureOptions;
			std::vector<CT_DPt*> m_dPt;
			CT_DLbls* m_dLbls;
			std::vector<CT_Trendline*> m_trendline;
			std::vector<CT_ErrBars*> m_errBars;
			CT_AxDataSource* m_cat;
			CT_NumDataSource* m_val;

			CT_AreaSer();
			virtual ~CT_AreaSer();

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{}
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Area3DChart
		{
		public:
			nullableComplexVal<CGrouping>	m_grouping;
			nullableBoolVal					m_varyColors;
            std::vector<CT_AreaSer*>		m_ser;
			CT_DLbls*						m_dLbls;
			CT_ChartLines*					m_dropLines;
			nullableStringVal				m_gapDepth;
			std::vector<_INT32>				m_axId;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Area3DChart();
			~CT_Area3DChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_AreaChart
		{
		public:
			nullableComplexVal<CGrouping>	m_grouping;
			nullableBoolVal					m_varyColors;
            std::vector<CT_AreaSer*>		m_ser;
			CT_DLbls*						m_dLbls;
			CT_ChartLines*					m_dropLines;
			std::vector<_INT32>				m_axId;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_AreaChart();
			~CT_AreaChart();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PlotArea
		{
		public:
			CT_Layout* m_layout;
            std::vector<ItemsChoiceType5*> m_ItemsElementName0;
            std::vector<void*> m_Items;
            std::vector<ItemsChoiceType6*> m_ItemsElementName1;
            std::vector<void*> m_Items1;
			CT_DTable* m_dTable;
			nullable<PPTX::Logic::SpPr>	m_spPr;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_PlotArea();
			~CT_PlotArea();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private:
			void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType5 eType, void* pVal) const;
			void toDelete(ItemsChoiceType5 eType, void* pVal);
			void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType6 eType, void* pVal) const;
			void toDelete(ItemsChoiceType6 eType, void* pVal);
		};
		class CT_Surface
		{
		public:
			nullableStringVal			m_thickness;
			nullable<PPTX::Logic::SpPr>	m_spPr;
			CT_PictureOptions*			m_pictureOptions;
			
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Surface();
			~CT_Surface();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_View3D
		{
		public:
			nullableIntVal			m_rotX;
			nullableUintVal			m_rotY;
			nullableStringVal		m_hPercent;
			nullableStringVal		m_depthPercent;
			nullableBoolVal			m_rAngAx;
			nullableUintVal			m_perspective;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_View3D();
			~CT_View3D();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PivotFmt
		{
		public:
			nullableUintVal							m_idx;
			nullable<PPTX::Logic::SpPr>				m_spPr;
			nullable<PPTX::Logic::TxBody>			m_txPr;	
			CT_Marker*								m_marker;
			CT_DLbl*								m_dLbl;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_PivotFmt();
			~CT_PivotFmt();
			
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_pivotFmts
		{
		public:
            std::vector<CT_PivotFmt*> m_pivotFmt;

			CT_pivotFmts();
			~CT_pivotFmts();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Chart
		{
		public:
			CT_Title* m_title;
			CT_pivotFmts* m_pivotFmts;
			CT_View3D* m_view3D;
			CT_Surface* m_floor;
			CT_Surface* m_sideWall;
			CT_Surface* m_backWall;
			CT_PlotArea* m_plotArea;
			CT_Legend* m_legend;
			nullableBoolVal m_autoTitleDeleted;
			nullableBoolVal m_plotVisOnly;
			nullableComplexVal<CDispBlanksAs> m_dispBlanksAs;
			nullableBoolVal m_showDLblsOverMax;
		
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_Chart();
			~CT_Chart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Protection
		{
		public:
			nullableBoolVal m_chartObject;
			nullableBoolVal m_data;
			nullableBoolVal m_formatting;
			nullableBoolVal m_selection;
			nullableBoolVal m_userInterface;

			CT_Protection();
			~CT_Protection();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PivotSource
		{
		public:
			nullable_string m_name;
			nullableIntVal m_fmtId; 
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_PivotSource();
			~CT_PivotSource();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ChartSpace
		{
		public:
			nullableBoolVal							m_date1904;
			nullableStringVal						m_lang;
			nullableBoolVal							m_roundedCorners;
			AlternateContent*						m_AlternateContent;
			CT_Style*								m_style;
			CT_PivotSource*							m_pivotSource;
			CT_Protection*							m_protection;
			CT_Chart*								m_chart;

			nullable<PPTX::Logic::ClrMap>			m_oClrMapOvr;

			nullable<PPTX::Logic::SpPr>				m_spPr;
			nullable<PPTX::Logic::TxBody>			m_txPr;

			CT_ExternalData*						m_externalData;
			CT_PrintSettings*						m_printSettings;
			CT_RelId*								m_userShapes;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

			CT_ChartSpace();
			~CT_ChartSpace();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};

		class CSeriesDataLabelsRange : public WritingElement
		{//c15
		public:
			WritingElement_AdditionMethods(CSeriesDataLabelsRange)

			CSeriesDataLabelsRange();
			virtual ~CSeriesDataLabelsRange();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual EElementType getType() const;

			nullable<CT_StrData> m_dlblRangeCache;
			nullable_string m_f;
		};

		class CCategoryFilterException : public WritingElement
		{//c15
		public:
			WritingElement_AdditionMethods(CCategoryFilterException)

			CCategoryFilterException();
			virtual ~CCategoryFilterException();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual EElementType getType() const;

			nullable_string				m_sqref;
			nullable<PPTX::Logic::SpPr>	m_spPr;
			nullableUintVal				m_explosion;
			nullableBoolVal				m_invertIfNegative;
			nullableBoolVal				m_bubble3D;
			nullable<CT_Marker>			m_marker;
			nullable<CT_DLbl>			m_dLbl;

		};
		class CCategoryFilterExceptions : public WritingElementWithChilds<CCategoryFilterException>
		{//c15
		public:
			WritingElement_AdditionMethods(CCategoryFilterExceptions)

			CCategoryFilterExceptions();
			virtual ~CCategoryFilterExceptions();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual EElementType getType() const;
		};

		class CSeriesFiltering : public WritingElement
		{//c15
		public:
			WritingElement_AdditionMethods(CSeriesFiltering)

			CSeriesFiltering();
			virtual ~CSeriesFiltering();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual EElementType getType() const;

			nullable<CSeriesDataLabelsRange> m_dataLabelsRange;
			nullable<CT_Tx> m_filteredSeriesTitle;
			nullable<CT_AxDataSource> m_filteredCategoryTitle;

			nullable<CT_LineSer>	m_filteredLineSeries; 
			nullable<CT_ScatterSer> m_filteredScatterSeries;
			nullable<CT_RadarSer>	m_filteredRadarSeries;
			nullable<CT_BarSer>		m_filteredBarSeries;
			nullable<CT_AreaSer>	m_filteredAreaSeries;
			nullable<CT_BubbleSer>	m_filteredBubbleSeries;
			nullable<CT_SurfaceSer> m_filteredSurfaceSeries;
			nullable<CT_PieSer>		m_filteredPieSeries;

			nullable_string m_fullRef; 
			nullable_string m_levelRef;
			nullable_string m_formulaRef; 
			
			nullable<OOX::Spreadsheet::CCategoryFilterExceptions> m_categoryFilterExceptions;
		};
	}
}

