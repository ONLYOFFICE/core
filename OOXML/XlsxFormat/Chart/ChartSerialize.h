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

#include "../CommonInclude.h"

#include "ChartStyle.h"
#include "ChartColors.h"

namespace OOX
{
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
		enum ItemsChoiceType5
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

		class CT_extLst;
		class CT_ChartSpace;
		class CT_Boolean;
		class CT_RelId;
		class CT_PageSetup;
		class CT_PageMargins;
		class CT_HeaderFooter;
		class CT_PrintSettings;
		class CT_ExternalData;
		class CT_DispBlanksAs;
		class CT_LegendEntry;
		class CT_UnsignedInt;
		class CT_Extension;
		class CT_LegendPos;
		class CT_Legend;
		class CT_Layout;
		class CT_ManualLayout;
		class CT_LayoutTarget;
		class CT_LayoutMode;
		class CT_Double;
		class CT_DTable;
		class CT_SerAx;
		class CT_Scaling;
		class CT_LogBase;
		class CT_Orientation;
		class CT_AxPos;
		class CT_ChartLines;
		class CT_Title;
		class CT_Tx;
		class CT_StrRef;
		class CT_StrData;
		class CT_StrVal;
		class CT_NumFmt;
		class CT_TickMark;
		class CT_TickLblPos;
		class CT_Crosses;
		class CT_Skip;
		class CT_TimeUnit;
		class CT_DateAx;
		class CT_LblOffset;
		class CT_AxisUnit;
		class CT_LblAlgn;
		class CT_CatAx;
		class CT_DispUnitsLbl;
		class CT_BuiltInUnit;
		class CT_DispUnits;
		class CT_CrossBetween;
		class CT_ValAx;
		class CT_SizeRepresents;
		class CT_BubbleScale;
		class CT_BubbleSer;
		class CT_SerTx;
		class CT_DPt;
		class CT_Marker;
		class CT_MarkerStyle;
		class CT_MarkerSize;
		class CT_PictureOptions;
		class CT_PictureFormat;
		class CT_PictureStackUnit;
		class CT_DLbls;
		class CT_DLbl;
		class CT_DLblPos;
		class CT_Trendline;
		class CT_TrendlineType;
		class CT_Order;
		class CT_Period;
		class CT_TrendlineLbl;
		class CT_ErrBars;
		class CT_ErrDir;
		class CT_ErrBarType;
		class CT_ErrValType;
		class CT_NumDataSource;
		class CT_NumData;
		class CT_NumVal;
		class CT_NumRef;
		class CT_AxDataSource;
		class CT_MultiLvlStrRef;
		class CT_lvl;
		class CT_MultiLvlStrData;
		class CT_BubbleChart;
		class CT_bandFmts;
		class CT_Surface3DChart;
		class CT_SurfaceSer;
		class CT_BandFmt;
		class CT_SurfaceChart;
		class CT_SecondPieSize;
		class CT_SplitType;
		class CT_OfPieType;
		class CT_custSplit;
		class CT_OfPieChart;
		class CT_PieSer;
		class CT_GapAmount;
		class CT_Bar3DChart;
		class CT_BarDir;
		class CT_BarGrouping;
		class CT_BarSer;
		class CT_Shape;
		class CT_Overlap;
		class CT_BarChart;
		class CT_HoleSize;
		class CT_DoughnutChart;
		class CT_FirstSliceAng;
		class CT_Pie3DChart;
		class CT_PieChart;
		class CT_ScatterSer;
		class CT_ScatterStyle;
		class CT_ScatterChart;
		class CT_RadarSer;
		class CT_RadarStyle;
		class CT_RadarChart;
		class CT_StockChart;
		class CT_LineSer;
		class CT_UpDownBars;
		class CT_UpDownBar;
		class CT_Line3DChart;
		class CT_Grouping;
		class CT_LineChart;
		class CT_Area3DChart;
		class CT_AreaSer;
		class CT_AreaChart;
		class CT_PlotArea;
		class CT_Thickness;
		class CT_Surface;
		class CT_Perspective;
		class CT_DepthPercent;
		class CT_RotY;
		class CT_HPercent;
		class CT_RotX;
		class CT_View3D;
		class CT_PivotFmt;
		class CT_pivotFmts;
		class CT_Chart;
		class CT_Protection;
		class CT_PivotSource;
		class CT_Style1;
		class CT_Style;
		class CT_TextLanguageID;
		class AlternateContent;
		class AlternateContentChoice;
		class AlternateContentFallback;
		class CTextProperties;

		bool FromXml_ST_DispBlanksAs(std::wstring& val, ST_DispBlanksAs& eOut);
		bool ToXml_ST_DispBlanksAs(ST_DispBlanksAs& val, std::wstring& sOut);
	///////////////////////////////////////////////////
		class CT_extLst
		{
		public:
			//Member
            std::vector<CT_Extension*> m_ext;
			CT_extLst();
			~CT_extLst();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ChartSpace
		{
		public:
			CT_Boolean*								m_date1904;
			CT_TextLanguageID*						m_lang;
			CT_Boolean*								m_roundedCorners;
			AlternateContent*						m_AlternateContent;
			CT_Style*								m_style;
			CT_PivotSource*							m_pivotSource;
			CT_Protection*							m_protection;
			CT_Chart*								m_chart;

			nullable<PPTX::Logic::ClrMap>			m_oClrMapOvr;

			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	

			CT_ExternalData*						m_externalData;
			CT_PrintSettings*						m_printSettings;
			CT_RelId*								m_userShapes;
			CT_extLst*								m_extLst;

			CT_ChartSpace();
			~CT_ChartSpace();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Boolean
		{
		public:
			//Attribute
			bool* m_val;
			CT_Boolean();
			~CT_Boolean();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_RelId
		{
		public:
			//Attribute
			std::wstring* m_id;
			CT_RelId();
			~CT_RelId();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_PageSetup
		{
		public:
			//Attribute
			unsigned long* m_paperSize;
			//Attribute
			std::wstring* m_paperHeight;
			//Attribute
			std::wstring* m_paperWidth;
			//Attribute
			unsigned long* m_firstPageNumber;
			//Attribute
			ST_PageSetupOrientation* m_orientation;
			//Attribute
			bool* m_blackAndWhite;
			//Attribute
			bool* m_draft;
			//Attribute
			bool* m_useFirstPageNumber;
			//Attribute
			long* m_horizontalDpi;
			//Attribute
			long* m_verticalDpi;
			//Attribute
			unsigned long* m_copies;
			CT_PageSetup();
			~CT_PageSetup();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_PageMargins
		{
		public:
			//Attribute
			double* m_l;
			//Attribute
			double* m_r;
			//Attribute
			double* m_t;
			//Attribute
			double* m_b;
			//Attribute
			double* m_header;
			//Attribute
			double* m_footer;
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
			//Member
			std::wstring* m_oddHeader;
			//Member
			std::wstring* m_oddFooter;
			//Member
			std::wstring* m_evenHeader;
			//Member
			std::wstring* m_evenFooter;
			//Member
			std::wstring* m_firstHeader;
			//Member
			std::wstring* m_firstFooter;
			//Attribute
			bool* m_alignWithMargins;
			//Attribute
			bool* m_differentOddEven;
			//Attribute
			bool* m_differentFirst;
			CT_HeaderFooter();
			~CT_HeaderFooter();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_PrintSettings
		{
		public:
			//Member
			CT_HeaderFooter* m_headerFooter;
			//Member
			CT_PageMargins* m_pageMargins;
			//Member
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
			//Member
			CT_Boolean* m_autoUpdate;
			//Attribute
			std::wstring* m_id;
			CT_ExternalData();
			~CT_ExternalData();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_DispBlanksAs
		{
		public:
			//Attribute
			ST_DispBlanksAs* m_val;
			CT_DispBlanksAs();
			~CT_DispBlanksAs();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_LegendEntry
		{
		public:
			CT_UnsignedInt*					m_idx;
			CT_Boolean*						m_delete;
			nullable<PPTX::Logic::TxBody>	m_oTxPr;	
			CT_extLst*						m_extLst;

			CT_LegendEntry();
			~CT_LegendEntry();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_UnsignedInt
		{
		public:
			//Attribute
			unsigned long* m_val;
			CT_UnsignedInt();
			~CT_UnsignedInt();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Extension
		{
		public:
			//Member
			std::wstring* m_Any;
			//Attribute
			std::wstring* m_uri;
			CT_Extension();
			~CT_Extension();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_LegendPos
		{
		public:
			//Attribute
			ST_LegendPos* m_val;
			CT_LegendPos();
			~CT_LegendPos();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Legend
		{
		public:
			CT_LegendPos							*m_legendPos;
            std::vector<CT_LegendEntry*>			m_legendEntry;
			CT_Layout								*m_layout;
			CT_Boolean								*m_overlay;
			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	
			CT_extLst								* m_extLst;
		/*--------------------------------------------------*/
			CT_Legend();
			~CT_Legend();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Layout
		{
		public:
			//Member
			CT_ManualLayout* m_manualLayout;
			//Member
			CT_extLst* m_extLst;
			CT_Layout();
			~CT_Layout();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ManualLayout
		{
		public:
			//Member
			CT_LayoutTarget* m_layoutTarget;
			//Member
			CT_LayoutMode* m_xMode;
			//Member
			CT_LayoutMode* m_yMode;
			//Member
			CT_LayoutMode* m_wMode;
			//Member
			CT_LayoutMode* m_hMode;
			//Member
			CT_Double* m_x;
			//Member
			CT_Double* m_y;
			//Member
			CT_Double* m_w;
			//Member
			CT_Double* m_h;
			//Member
			CT_extLst* m_extLst;
			CT_ManualLayout();
			~CT_ManualLayout();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_LayoutTarget
		{
		public:
			//Attribute
			ST_LayoutTarget* m_val;
			CT_LayoutTarget();
			~CT_LayoutTarget();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_LayoutMode
		{
		public:
			//Attribute
			ST_LayoutMode* m_val;
			CT_LayoutMode();
			~CT_LayoutMode();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Double
		{
		public:
			double* m_val;
			CT_Double();
			~CT_Double();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_DTable
		{
		public:
			CT_Boolean* m_showHorzBorder;
			CT_Boolean* m_showVertBorder;
			CT_Boolean* m_showOutline;
			CT_Boolean* m_showKeys;
			nullable<PPTX::Logic::SpPr>		m_oSpPr;
			nullable<PPTX::Logic::TxBody>	m_oTxPr;	
			CT_extLst*						m_extLst;
			
			CT_DTable();
			~CT_DTable();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SerAx
		{
		public:
			CT_UnsignedInt							*m_axId;
			CT_Scaling								*m_scaling;
			CT_Boolean								*m_delete;
			CT_AxPos								*m_axPos;
			CT_ChartLines							*m_majorGridlines;
			CT_ChartLines							*m_minorGridlines;
			CT_Title								*m_title;
			CT_NumFmt								*m_numFmt;
			CT_TickMark								*m_majorTickMark;
			CT_TickMark								*m_minorTickMark;
			CT_TickLblPos							*m_tickLblPos;
			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	
			CT_UnsignedInt							*m_crossAx;
			CT_Crosses								*m_crosses;
			CT_Double								*m_crossesAt;
			CT_Skip									*m_tickLblSkip;
			CT_Skip									*m_tickMarkSkip;
			CT_extLst								*m_extLst;
		/*--------------------------------------------------------------*/
			CT_SerAx();
			~CT_SerAx();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Scaling
		{
		public:
			//Member
			CT_LogBase* m_logBase;
			//Member
			CT_Orientation* m_orientation;
			//Member
			CT_Double* m_max;
			//Member
			CT_Double* m_min;
			//Member
			CT_extLst* m_extLst;
			CT_Scaling();
			~CT_Scaling();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_LogBase
		{
		public:
			//Attribute
			double	* m_val;
			CT_LogBase();
			~CT_LogBase();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Orientation
		{
		public:
			//Attribute
			ST_Orientation	* m_val;
			CT_Orientation();
			~CT_Orientation();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_AxPos
		{
		public:
			//Attribute
			ST_AxPos	* m_val;
			CT_AxPos();
			~CT_AxPos();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_ChartLines
		{
		public:
			nullable<PPTX::Logic::SpPr>     m_oSpPr;
		/*--------------------------------------------------------------*/
			CT_ChartLines();
			~CT_ChartLines();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Title
		{
		public:
			CT_Tx								*m_tx;
			CT_Layout							*m_layout;
			CT_Boolean							*m_overlay;
			nullable<PPTX::Logic::SpPr>			m_oSpPr;
			nullable<PPTX::Logic::TxBody>		m_oTxPr;	
			CT_extLst							*m_extLst;
		/*--------------------------------------------------------------*/
			CT_Title();
			~CT_Title();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		//--------------------------------------------------------------------------------
		// 21.2.2.214 tx (Chart Text)
		//--------------------------------------------------------------------------------	
		class CT_Tx
		{
		public:
			nullable<PPTX::Logic::TxBody>	m_oRich;	
			CT_StrRef						*m_strRef;
		/*--------------------------------------------------------------*/
			CT_Tx();
			~CT_Tx();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_StrRef
		{
		public:
			std::wstring	* m_f;
			CT_StrData		* m_strCache;
			CT_extLst		* m_extLst;
		/*--------------------------------------------------------------*/
			CT_StrRef();
			~CT_StrRef();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_StrData
		{
		public:
			//Member
			CT_UnsignedInt* m_ptCount;
			//Member
            std::vector<CT_StrVal*> m_pt;
			//Member
			CT_extLst* m_extLst;
			CT_StrData();
			~CT_StrData();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_StrVal
		{
		public:
			//Member
			std::wstring* m_v;
			//Attribute
			unsigned long* m_idx;
			CT_StrVal();
			~CT_StrVal();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_NumFmt
		{
		public:
			//Attribute
			std::wstring* m_formatCode;
			//Attribute
			bool* m_sourceLinked;
			CT_NumFmt();
			~CT_NumFmt();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_TickMark
		{
		public:
			//Attribute
			ST_TickMark* m_val;
			CT_TickMark();
			~CT_TickMark();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_TickLblPos
		{
		public:
			//Attribute
			ST_TickLblPos* m_val;
			CT_TickLblPos();
			~CT_TickLblPos();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Crosses
		{
		public:
			//Attribute
			ST_Crosses* m_val;
			CT_Crosses();
			~CT_Crosses();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Skip
		{
		public:
			//Attribute
			unsigned long* m_val;
			CT_Skip();
			~CT_Skip();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_TimeUnit
		{
		public:
			//Attribute
			ST_TimeUnit* m_val;
			CT_TimeUnit();
			~CT_TimeUnit();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_DateAx
		{
		public:
			CT_UnsignedInt*							m_axId;
			CT_Scaling*								m_scaling;
			CT_Boolean*								m_delete;
			CT_AxPos*								m_axPos;
			CT_ChartLines*							m_majorGridlines;
			CT_ChartLines*							m_minorGridlines;
			CT_Title*								m_title;
			CT_NumFmt*								m_numFmt;
			CT_TickMark*							m_majorTickMark;
			CT_TickMark*							m_minorTickMark;
			CT_TickLblPos*							m_tickLblPos;
			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	
			CT_UnsignedInt*							m_crossAx;
			CT_Crosses*								m_crosses;
			CT_Double*								m_crossesAt;
			CT_Boolean*								m_auto;
			CT_LblOffset*							m_lblOffset;
			CT_TimeUnit*							m_baseTimeUnit;
			CT_AxisUnit*							m_majorUnit;
			CT_TimeUnit*							m_majorTimeUnit;
			CT_AxisUnit*							m_minorUnit;
			CT_TimeUnit*							m_minorTimeUnit;
			CT_extLst*								m_extLst;

			CT_DateAx();
			~CT_DateAx();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_LblOffset
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_LblOffset();
			~CT_LblOffset();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_AxisUnit
		{
		public:
			double* m_val;
			CT_AxisUnit();
			~CT_AxisUnit();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_LblAlgn
		{
		public:
			//Attribute
			ST_LblAlgn* m_val;
			CT_LblAlgn();
			~CT_LblAlgn();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_CatAx
		{
		public:
			//Member
			CT_UnsignedInt* m_axId;
			//Member
			CT_Scaling* m_scaling;
			//Member
			CT_Boolean* m_delete;
			//Member
			CT_AxPos* m_axPos;
			//Member
			CT_ChartLines* m_majorGridlines;
			//Member
			CT_ChartLines* m_minorGridlines;
			//Member
			CT_Title* m_title;
			//Member
			CT_NumFmt* m_numFmt;
			//Member
			CT_TickMark* m_majorTickMark;
			//Member
			CT_TickMark* m_minorTickMark;
			//Member
			CT_TickLblPos* m_tickLblPos;
			nullable<PPTX::Logic::SpPr>		m_oSpPr;
			nullable<PPTX::Logic::TxBody>	m_oTxPr;	
			CT_UnsignedInt* m_crossAx;
			//Member
			CT_Crosses* m_crosses;
			//Member
			CT_Double* m_crossesAt;
			//Member
			CT_Boolean* m_auto;
			//Member
			CT_LblAlgn* m_lblAlgn;
			//Member
			CT_LblOffset* m_lblOffset;
			//Member
			CT_Skip* m_tickLblSkip;
			//Member
			CT_Skip* m_tickMarkSkip;
			//Member
			CT_Boolean* m_noMultiLvlLbl;
			//Member
			CT_extLst* m_extLst;
			CT_CatAx();
			~CT_CatAx();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DispUnitsLbl
		{
		public:
			CT_Layout*								m_layout;
			CT_Tx*									m_tx;
			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	

			CT_DispUnitsLbl();
			~CT_DispUnitsLbl();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_BuiltInUnit
		{
		public:
			//Attribute
			ST_BuiltInUnit* m_val;
			CT_BuiltInUnit();
			~CT_BuiltInUnit();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_DispUnits
		{
		public:
			//Member
			CT_BuiltInUnit* m_builtInUnit;
			//Member
			CT_Double* m_custUnit;
			//Member
			CT_DispUnitsLbl* m_dispUnitsLbl;
			//Member
			CT_extLst* m_extLst;
			CT_DispUnits();
			~CT_DispUnits();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_CrossBetween
		{
		public:
			//Attribute
			ST_CrossBetween* m_val;
			CT_CrossBetween();
			~CT_CrossBetween();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_ValAx
		{
		public:
			CT_UnsignedInt*							m_axId;
			CT_Scaling*								m_scaling;
			CT_Boolean*								m_delete;
			CT_AxPos*								m_axPos;
			CT_ChartLines*							m_majorGridlines;
			CT_ChartLines*							m_minorGridlines;
			CT_Title*								m_title;
			CT_NumFmt*								m_numFmt;
			CT_TickMark*							m_majorTickMark;
			CT_TickMark*							m_minorTickMark;
			CT_TickLblPos*							m_tickLblPos;
			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	
			CT_UnsignedInt*							m_crossAx;
			CT_Crosses*								m_crosses;
			CT_Double*								m_crossesAt;
			CT_CrossBetween*						m_crossBetween;
			CT_AxisUnit*							m_majorUnit;
			CT_AxisUnit*							m_minorUnit;
			CT_DispUnits*							m_dispUnits;
			CT_extLst*								m_extLst;

			CT_ValAx();
			~CT_ValAx();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SizeRepresents
		{
		public:
			//Attribute
			ST_SizeRepresents* m_val;
			CT_SizeRepresents();
			~CT_SizeRepresents();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_BubbleScale
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_BubbleScale();
			~CT_BubbleScale();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_BubbleSer
		{
		public:
			CT_UnsignedInt* m_idx;
			CT_UnsignedInt* m_order;
			CT_SerTx*		m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			//Member
			CT_Boolean* m_invertIfNegative;
			//Member
            std::vector<CT_DPt*> m_dPt;
			//Member
			CT_DLbls* m_dLbls;
			//Member
            std::vector<CT_Trendline*> m_trendline;
			//Member
            std::vector<CT_ErrBars*> m_errBars;
			//Member
			CT_AxDataSource* m_xVal;
			//Member
			CT_NumDataSource* m_yVal;
			//Member
			CT_NumDataSource* m_bubbleSize;
			//Member
			CT_Boolean* m_bubble3D;
			//Member
			CT_extLst* m_extLst;
			CT_BubbleSer();
			~CT_BubbleSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SerTx
		{
		public:
			//Member
			CT_StrRef* m_strRef;
			//Member
			std::wstring* m_v;
			CT_SerTx();
			~CT_SerTx();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_DPt
		{
		public:
			CT_UnsignedInt*				m_idx;
			CT_Boolean*					m_invertIfNegative;
			CT_Marker*					m_marker;
			CT_Boolean*					m_bubble3D;
			CT_UnsignedInt*				m_explosion;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			CT_PictureOptions*			m_pictureOptions;
			CT_extLst*					m_extLst;
			
			CT_DPt();
			~CT_DPt();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Marker
		{
		public:
			CT_MarkerStyle*				m_symbol;
			CT_MarkerSize*				m_size;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			CT_extLst*					m_extLst;

			CT_Marker();
			~CT_Marker();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_MarkerStyle
		{
		public:
			//Attribute
			ST_MarkerStyle* m_val;
			CT_MarkerStyle();
			~CT_MarkerStyle();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_MarkerSize
		{
		public:
			//Attribute
			unsigned char* m_val;
			CT_MarkerSize();
			~CT_MarkerSize();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_PictureOptions
		{
		public:
			//Member
			CT_Boolean* m_applyToFront;
			//Member
			CT_Boolean* m_applyToSides;
			//Member
			CT_Boolean* m_applyToEnd;
			//Member
			CT_PictureFormat* m_pictureFormat;
			//Member
			CT_PictureStackUnit* m_pictureStackUnit;
			CT_PictureOptions();
			~CT_PictureOptions();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PictureFormat
		{
		public:
			//Attribute
			ST_PictureFormat* m_val;
			CT_PictureFormat();
			~CT_PictureFormat();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_PictureStackUnit
		{
		public:
			//Attribute
			double* m_val;
			CT_PictureStackUnit();
			~CT_PictureStackUnit();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_DLbls
		{
		public:
            std::vector<CT_DLbl*>			m_dLbl;
            std::vector<ItemsChoiceType3*>	m_ItemsElementName0;
            std::vector<void*>				m_Items;
			CT_extLst*						m_extLst;
			
			CT_DLbls();
			~CT_DLbls();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType3 eType, void* pVal) const;
		private: void toDelete(ItemsChoiceType3 eType, void* pVal);
		};
		class CT_DLbl
		{
		public:
			//Member
			CT_UnsignedInt* m_idx;
			//Member
            std::vector<ItemsChoiceType4*> m_ItemsElementName0;
			//Member
            std::vector<void*> m_Items;
			//Member
			CT_extLst* m_extLst;
			CT_DLbl();
			~CT_DLbl();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType4 eType, void* pVal) const;
		private: void toDelete(ItemsChoiceType4 eType, void* pVal);
		};
		class CT_DLblPos
		{
		public:
			//Attribute
			ST_DLblPos* m_val;
			CT_DLblPos();
			~CT_DLblPos();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Trendline
		{
		public:
			std::wstring*				m_name;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			CT_TrendlineType*			m_trendlineType;
			//Member
			CT_Order* m_order;
			//Member
			CT_Period* m_period;
			//Member
			CT_Double* m_forward;
			//Member
			CT_Double* m_backward;
			//Member
			CT_Double* m_intercept;
			//Member
			CT_Boolean* m_dispRSqr;
			//Member
			CT_Boolean* m_dispEq;
			//Member
			CT_TrendlineLbl* m_trendlineLbl;
			//Member
			CT_extLst* m_extLst;
			CT_Trendline();
			~CT_Trendline();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_TrendlineType
		{
		public:
			//Attribute
			ST_TrendlineType* m_val;
			CT_TrendlineType();
			~CT_TrendlineType();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Order
		{
		public:
			//Attribute
			unsigned char* m_val;
			CT_Order();
			~CT_Order();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Period
		{
		public:
			//Attribute
			unsigned long* m_val;
			CT_Period();
			~CT_Period();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_TrendlineLbl
		{
		public:
			CT_Layout*								m_layout;
			CT_Tx*									m_tx;
			CT_NumFmt*								m_numFmt;
			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	
			CT_extLst*								m_extLst;

			CT_TrendlineLbl();
			~CT_TrendlineLbl();

			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ErrBars
		{
		public:
			//Member
			CT_ErrDir* m_errDir;
			//Member
			CT_ErrBarType* m_errBarType;
			//Member
			CT_ErrValType* m_errValType;
			//Member
			CT_Boolean* m_noEndCap;
			//Member
			CT_NumDataSource* m_plus;
			//Member
			CT_NumDataSource* m_minus;
			//Member
			CT_Double* m_val;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			CT_extLst*					m_extLst;

			CT_ErrBars();
			~CT_ErrBars();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ErrDir
		{
		public:
			//Attribute
			ST_ErrDir* m_val;
			CT_ErrDir();
			~CT_ErrDir();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_ErrBarType
		{
		public:
			//Attribute
			ST_ErrBarType* m_val;
			CT_ErrBarType();
			~CT_ErrBarType();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_ErrValType
		{
		public:
			//Attribute
			ST_ErrValType* m_val;
			CT_ErrValType();
			~CT_ErrValType();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_NumDataSource
		{
		public:
			//Member
			CT_NumData* m_numLit;
			//Member
			CT_NumRef* m_numRef;
			CT_NumDataSource();
			~CT_NumDataSource();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_NumData
		{
		public:
			//Member
			std::wstring* m_formatCode;
			//Member
			CT_UnsignedInt* m_ptCount;
			//Member
            std::vector<CT_NumVal*> m_pt;
			//Member
			CT_extLst* m_extLst;
			CT_NumData();
			~CT_NumData();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_NumVal
		{
		public:
			//Member
			std::wstring* m_v;
			//Attribute
			unsigned long* m_idx;
			//Attribute
			std::wstring* m_formatCode;
			CT_NumVal();
			~CT_NumVal();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_NumRef
		{
		public:
			//Member
			std::wstring* m_f;
			//Member
			CT_NumData* m_numCache;
			//Member
			CT_extLst* m_extLst;
			CT_NumRef();
			~CT_NumRef();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_AxDataSource
		{
		public:
			//Member
			CT_MultiLvlStrRef* m_multiLvlStrRef;
			//Member
			CT_NumData* m_numLit;
			//Member
			CT_NumRef* m_numRef;
			//Member
			CT_StrData* m_strLit;
			//Member
			CT_StrRef* m_strRef;
			CT_AxDataSource();
			~CT_AxDataSource();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_MultiLvlStrRef
		{
		public:
			//Member
			std::wstring* m_f;
			//Member
			CT_MultiLvlStrData* m_multiLvlStrCache;
			//Member
			CT_extLst* m_extLst;
			CT_MultiLvlStrRef();
			~CT_MultiLvlStrRef();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_lvl
		{
		public:
			//Member
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
			//Member
			CT_UnsignedInt* m_ptCount;
			//Member
            std::vector<CT_lvl*> m_lvl;
			//Member
			CT_extLst* m_extLst;
			CT_MultiLvlStrData();
			~CT_MultiLvlStrData();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_BubbleChart
		{
		public:
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_BubbleSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_Boolean* m_bubble3D;
			//Member
			CT_BubbleScale* m_bubbleScale;
			//Member
			CT_Boolean* m_showNegBubbles;
			//Member
			CT_SizeRepresents* m_sizeRepresents;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_BubbleChart();
			~CT_BubbleChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_bandFmts
		{
		public:
			//Member
            std::vector<CT_BandFmt*> m_bandFmt;
			CT_bandFmts();
			~CT_bandFmts();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Surface3DChart
		{
		public:
			//Member
			CT_Boolean* m_wireframe;
			//Member
            std::vector<CT_SurfaceSer*> m_ser;
			//Member
			CT_bandFmts* m_bandFmts;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_Surface3DChart();
			~CT_Surface3DChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SurfaceSer
		{
		public:
			CT_UnsignedInt*				m_idx;
			CT_UnsignedInt*				m_order;
			CT_SerTx*					m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			CT_AxDataSource*			m_cat;
			CT_NumDataSource*			m_val;
			CT_extLst*					m_extLst;

			CT_SurfaceSer();
			~CT_SurfaceSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_BandFmt
		{
		public:
			CT_UnsignedInt*				m_idx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			
			CT_BandFmt();
			~CT_BandFmt();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SurfaceChart
		{
		public:
			//Member
			CT_Boolean* m_wireframe;
			//Member
            std::vector<CT_SurfaceSer*> m_ser;
			//Member
			CT_bandFmts* m_bandFmts;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_SurfaceChart();
			~CT_SurfaceChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_SecondPieSize
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_SecondPieSize();
			~CT_SecondPieSize();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_SplitType
		{
		public:
			//Attribute
			ST_SplitType* m_val;
			CT_SplitType();
			~CT_SplitType();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_OfPieType
		{
		public:
			//Attribute
			ST_OfPieType* m_val;
			CT_OfPieType();
			~CT_OfPieType();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_custSplit
		{
		public:
			//Member
            std::vector<CT_UnsignedInt*> m_secondPiePt;
			CT_custSplit();
			~CT_custSplit();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_OfPieChart
		{
		public:
			//Member
			CT_OfPieType* m_ofPieType;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_PieSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_GapAmount* m_gapWidth;
			//Member
			CT_SplitType* m_splitType;
			//Member
			CT_Double* m_splitPos;
			//Member
			CT_custSplit* m_custSplit;
			//Member
			CT_SecondPieSize* m_secondPieSize;
			//Member
            std::vector<CT_ChartLines*> m_serLines;
			//Member
			CT_extLst* m_extLst;
			CT_OfPieChart();
			~CT_OfPieChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PieSer
		{
		public:
			CT_UnsignedInt*				m_idx;
			CT_UnsignedInt*				m_order;
			CT_SerTx*					m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			CT_UnsignedInt*				m_explosion;
			//Member
            std::vector<CT_DPt*> m_dPt;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_AxDataSource* m_cat;
			//Member
			CT_NumDataSource* m_val;
			//Member
			CT_extLst* m_extLst;
			CT_PieSer();
			~CT_PieSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_GapAmount
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_GapAmount();
			~CT_GapAmount();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Bar3DChart
		{
		public:
			//Member
			CT_BarDir* m_barDir;
			//Member
			CT_BarGrouping* m_grouping;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_BarSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_GapAmount* m_gapWidth;
			//Member
			CT_GapAmount* m_gapDepth;
			//Member
			CT_Shape* m_shape;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_Bar3DChart();
			~CT_Bar3DChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_BarDir
		{
		public:
			//Attribute
			ST_BarDir* m_val;
			CT_BarDir();
			~CT_BarDir();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_BarGrouping
		{
		public:
			//Attribute
			ST_BarGrouping* m_val;
			CT_BarGrouping();
			~CT_BarGrouping();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_BarSer
		{
		public:
			//Member
			CT_UnsignedInt* m_idx;
			//Member
			CT_UnsignedInt* m_order;
			//Member
			CT_SerTx*					m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			//Member
			CT_Boolean* m_invertIfNegative;
			//Member
			CT_PictureOptions* m_pictureOptions;
			//Member
            std::vector<CT_DPt*> m_dPt;
			//Member
			CT_DLbls* m_dLbls;
			//Member
            std::vector<CT_Trendline*> m_trendline;
			//Member
			CT_ErrBars* m_errBars;
			//Member
			CT_AxDataSource* m_cat;
			//Member
			CT_NumDataSource* m_val;
			//Member
			CT_Shape* m_shape;
			//Member
			CT_extLst* m_extLst;
			CT_BarSer();
			~CT_BarSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Shape
		{
		public:
			//Attribute
			ST_Shape* m_val;
			CT_Shape();
			~CT_Shape();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Overlap
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_Overlap();
			~CT_Overlap();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_BarChart
		{
		public:
			//Member
			CT_BarDir* m_barDir;
			//Member
			CT_BarGrouping* m_grouping;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_BarSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_GapAmount* m_gapWidth;
			//Member
			CT_Overlap* m_overlap;
			//Member
            std::vector<CT_ChartLines*> m_serLines;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_BarChart();
			~CT_BarChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_HoleSize
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_HoleSize();
			~CT_HoleSize();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_DoughnutChart
		{
		public:
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_PieSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_FirstSliceAng* m_firstSliceAng;
			//Member
			CT_HoleSize* m_holeSize;
			//Member
			CT_extLst* m_extLst;
			CT_DoughnutChart();
			~CT_DoughnutChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_FirstSliceAng
		{
		public:
			//Attribute
			unsigned long* m_val;
			CT_FirstSliceAng();
			~CT_FirstSliceAng();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Pie3DChart
		{
		public:
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_PieSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_extLst* m_extLst;
			CT_Pie3DChart();
			~CT_Pie3DChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PieChart
		{
		public:
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_PieSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_FirstSliceAng* m_firstSliceAng;
			//Member
			CT_extLst* m_extLst;
			CT_PieChart();
			~CT_PieChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ScatterSer
		{
		public:
			//Member
			CT_UnsignedInt* m_idx;
			//Member
			CT_UnsignedInt* m_order;
			//Member
			CT_SerTx* m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			//Member
			CT_Marker* m_marker;
			//Member
            std::vector<CT_DPt*> m_dPt;
			//Member
			CT_DLbls* m_dLbls;
			//Member
            std::vector<CT_Trendline*> m_trendline;
			//Member
            std::vector<CT_ErrBars*> m_errBars;
			//Member
			CT_AxDataSource* m_xVal;
			//Member
			CT_NumDataSource* m_yVal;
			//Member
			CT_Boolean* m_smooth;
			//Member
			CT_extLst* m_extLst;
			CT_ScatterSer();
			~CT_ScatterSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_ScatterStyle
		{
		public:
			//Attribute
			ST_ScatterStyle* m_val;
			CT_ScatterStyle();
			~CT_ScatterStyle();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_ScatterChart
		{
		public:
			//Member
			CT_ScatterStyle* m_scatterStyle;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_ScatterSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_ScatterChart();
			~CT_ScatterChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_RadarSer
		{
		public:
			//Member
			CT_UnsignedInt* m_idx;
			//Member
			CT_UnsignedInt* m_order;
			//Member
			CT_SerTx*					m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			//Member
			CT_Marker* m_marker;
			//Member
            std::vector<CT_DPt*> m_dPt;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_AxDataSource* m_cat;
			//Member
			CT_NumDataSource* m_val;
			//Member
			CT_extLst* m_extLst;
			CT_RadarSer();
			~CT_RadarSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_RadarStyle
		{
		public:
			//Attribute
			ST_RadarStyle* m_val;
			CT_RadarStyle();
			~CT_RadarStyle();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_RadarChart
		{
		public:
			//Member
			CT_RadarStyle* m_radarStyle;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_RadarSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_RadarChart();
			~CT_RadarChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_StockChart
		{
		public:
			//Member
            std::vector<CT_LineSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_ChartLines* m_dropLines;
			//Member
			CT_ChartLines* m_hiLowLines;
			//Member
			CT_UpDownBars* m_upDownBars;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_StockChart();
			~CT_StockChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_LineSer
		{
		public:
			//Member
			CT_UnsignedInt* m_idx;
			//Member
			CT_UnsignedInt* m_order;
			//Member
			CT_SerTx*					m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			//Member
			CT_Marker* m_marker;
			//Member
            std::vector<CT_DPt*> m_dPt;
			//Member
			CT_DLbls* m_dLbls;
			//Member
            std::vector<CT_Trendline*> m_trendline;
			//Member
			CT_ErrBars* m_errBars;
			//Member
			CT_AxDataSource* m_cat;
			//Member
			CT_NumDataSource* m_val;
			//Member
			CT_Boolean* m_smooth;
			//Member
			CT_extLst* m_extLst;
			CT_LineSer();
			~CT_LineSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_UpDownBars
		{
		public:
			CT_GapAmount*	m_gapWidth;
			CT_UpDownBar*	m_upBars;
			CT_UpDownBar*	m_downBars;
			CT_extLst*		m_extLst;

			CT_UpDownBars();
			~CT_UpDownBars();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_UpDownBar
		{
		public:
			nullable<PPTX::Logic::SpPr>	m_oSpPr;

			CT_UpDownBar();
			~CT_UpDownBar();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Line3DChart
		{
		public:
			//Member
			CT_Grouping* m_grouping;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_LineSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_ChartLines* m_dropLines;
			//Member
			CT_GapAmount* m_gapDepth;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_Line3DChart();
			~CT_Line3DChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Grouping
		{
		public:
			//Attribute
			ST_Grouping* m_val;
			CT_Grouping();
			~CT_Grouping();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_LineChart
		{
		public:
			//Member
			CT_Grouping* m_grouping;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_LineSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_ChartLines* m_dropLines;
			//Member
			CT_ChartLines* m_hiLowLines;
			//Member
			CT_UpDownBars* m_upDownBars;
			//Member
			CT_Boolean* m_marker;
			//Member
			CT_Boolean* m_smooth;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_LineChart();
			~CT_LineChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Area3DChart
		{
		public:
			//Member
			CT_Grouping* m_grouping;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_AreaSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_ChartLines* m_dropLines;
			//Member
			CT_GapAmount* m_gapDepth;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_Area3DChart();
			~CT_Area3DChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_AreaSer
		{
		public:
			//Member
			CT_UnsignedInt* m_idx;
			//Member
			CT_UnsignedInt* m_order;
			//Member
			CT_SerTx* m_tx;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			//Member
			CT_PictureOptions* m_pictureOptions;
			//Member
            std::vector<CT_DPt*> m_dPt;
			//Member
			CT_DLbls* m_dLbls;
			//Member
            std::vector<CT_Trendline*> m_trendline;
			//Member
            std::vector<CT_ErrBars*> m_errBars;
			//Member
			CT_AxDataSource* m_cat;
			//Member
			CT_NumDataSource* m_val;
			//Member
			CT_extLst* m_extLst;
			CT_AreaSer();
			~CT_AreaSer();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_AreaChart
		{
		public:
			//Member
			CT_Grouping* m_grouping;
			//Member
			CT_Boolean* m_varyColors;
			//Member
            std::vector<CT_AreaSer*> m_ser;
			//Member
			CT_DLbls* m_dLbls;
			//Member
			CT_ChartLines* m_dropLines;
			//Member
            std::vector<CT_UnsignedInt*> m_axId;
			//Member
			CT_extLst* m_extLst;
			CT_AreaChart();
			~CT_AreaChart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PlotArea
		{
		public:
			//Member
			CT_Layout* m_layout;
			//Member
            std::vector<ItemsChoiceType5*> m_ItemsElementName0;
			//Member
            std::vector<void*> m_Items;
			//Member
            std::vector<ItemsChoiceType6*> m_ItemsElementName1;
			//Member
            std::vector<void*> m_Items1;
			//Member
			CT_DTable* m_dTable;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			//Member
			CT_extLst* m_extLst;
			CT_PlotArea();
			~CT_PlotArea();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType5 eType, void* pVal) const;
		private: void toDelete(ItemsChoiceType5 eType, void* pVal);
		private: void toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType6 eType, void* pVal) const;
 		private: void toDelete(ItemsChoiceType6 eType, void* pVal);
		};
		class CT_Thickness
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_Thickness();
			~CT_Thickness();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_Surface
		{
		public:
			CT_Thickness*				m_thickness;
			nullable<PPTX::Logic::SpPr>	m_oSpPr;
			CT_PictureOptions*			m_pictureOptions;
			CT_extLst*					m_extLst;

			CT_Surface();
			~CT_Surface();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Perspective
		{
		public:
			//Attribute
			unsigned char* m_val;
			CT_Perspective();
			~CT_Perspective();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_DepthPercent
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_DepthPercent();
			~CT_DepthPercent();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_RotY
		{
		public:
			//Attribute
			unsigned long* m_val;
			CT_RotY();
			~CT_RotY();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_HPercent
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_HPercent();
			~CT_HPercent();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_RotX
		{
		public:
			//Attribute
			signed char* m_val;
			CT_RotX();
			~CT_RotX();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_View3D
		{
		public:
			//Member
			CT_RotX* m_rotX;
			//Member
			CT_HPercent* m_hPercent;
			//Member
			CT_RotY* m_rotY;
			//Member
			CT_DepthPercent* m_depthPercent;
			//Member
			CT_Boolean* m_rAngAx;
			//Member
			CT_Perspective* m_perspective;
			//Member
			CT_extLst* m_extLst;
			CT_View3D();
			~CT_View3D();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PivotFmt
		{
		public:
			CT_UnsignedInt*							m_idx;
			nullable<PPTX::Logic::SpPr>				m_oSpPr;
			nullable<PPTX::Logic::TxBody>			m_oTxPr;	
			CT_Marker*								m_marker;
			CT_DLbl*								m_dLbl;
			CT_extLst*								m_extLst;

			CT_PivotFmt();
			~CT_PivotFmt();
			
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_pivotFmts
		{
		public:
			//Member
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
			//Member
			CT_Title* m_title;
			//Member
			CT_Boolean* m_autoTitleDeleted;
			//Member
			CT_pivotFmts* m_pivotFmts;
			//Member
			CT_View3D* m_view3D;
			//Member
			CT_Surface* m_floor;
			//Member
			CT_Surface* m_sideWall;
			//Member
			CT_Surface* m_backWall;
			//Member
			CT_PlotArea* m_plotArea;
			//Member
			CT_Legend* m_legend;
			//Member
			CT_Boolean* m_plotVisOnly;
			//Member
			CT_DispBlanksAs* m_dispBlanksAs;
			//Member
			CT_Boolean* m_showDLblsOverMax;
			//Member
			CT_extLst* m_extLst;
			CT_Chart();
			~CT_Chart();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Protection
		{
		public:
			//Member
			CT_Boolean* m_chartObject;
			//Member
			CT_Boolean* m_data;
			//Member
			CT_Boolean* m_formatting;
			//Member
			CT_Boolean* m_selection;
			//Member
			CT_Boolean* m_userInterface;
			CT_Protection();
			~CT_Protection();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_PivotSource
		{
		public:
			//Member
			std::wstring* m_name;
			//Member
			CT_UnsignedInt* m_fmtId;
			//Member
            std::vector<CT_extLst*> m_extLst;
			CT_PivotSource();
			~CT_PivotSource();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		};
		class CT_Style
		{
		public:
			std::wstring m_namespace;

			unsigned char* m_val;

			CT_Style();
			~CT_Style();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: 
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CT_TextLanguageID
		{
		public:
			//Attribute
			std::wstring* m_val;
			CT_TextLanguageID();
			~CT_TextLanguageID();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const;
			EElementType getType();
		private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
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
		class AlternateContentChoice
		{
		public:
			CT_Style* m_style;
			std::wstring* m_Requires;

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
	}
}

