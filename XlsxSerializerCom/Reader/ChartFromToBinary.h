﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef CHART_FROM_TO_BINARY
#define CHART_FROM_TO_BINARY

#include "CommonWriter.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Chart/ChartSerialize.h"
#include "../Writer/BinaryCommonReader.h"
#include "../../ASCOfficePPTXFile/ASCOfficeDrawingConverter.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartSpace;
	}
}

namespace BinXlsxRW
{
	class SaveParams
	{
	public:
		SaveParams	(const std::wstring& _sThemePath, OOX::CContentTypes *pContentTypes);

		smart_ptr<PPTX::Theme>			pTheme;
		std::wstring					sThemePath;
		OOX::CContentTypes*				pContentTypes;
		int								nThemeOverrideCount;
	};

	class BinaryChartReader : public Binary_CommonReader<BinaryChartReader>
	{
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		SaveParams&						m_oSaveParams;
	public:
		BinaryChartReader	(NSBinPptxRW::CBinaryFileReader& oBufferedStream, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		
		int ReadCT_ChartSpace			(long length, OOX::Spreadsheet::CT_ChartSpace* poResult);
		int ReadCT_RelId				(long length, OOX::Spreadsheet::CT_RelId* poResult);
	private: 
		int ReadCT_extLst				(BYTE type, long length, void* poResult);
		int ReadCT_ChartSpace			(BYTE type, long length, void* poResult);
		int ReadCT_Boolean				(BYTE type, long length, void* poResult);
		int ReadCT_RelId				(BYTE type, long length, void* poResult);
		int ReadCT_PageSetup			(BYTE type, long length, void* poResult);
		int ReadCT_PageMargins			(BYTE type, long length, void* poResult);
		int ReadCT_HeaderFooter			(BYTE type, long length, void* poResult);
		int ReadCT_PrintSettings		(BYTE type, long length, void* poResult);
		int ReadCT_ExternalData			(BYTE type, long length, void* poResult);
		int ReadCT_DispBlanksAs			(BYTE type, long length, void* poResult);
		int ReadCT_LegendEntry			(BYTE type, long length, void* poResult);
		int ReadCT_UnsignedInt			(BYTE type, long length, void* poResult);
		int ReadCT_Extension			(BYTE type, long length, void* poResult);
		int ReadCT_LegendPos			(BYTE type, long length, void* poResult);
		int ReadCT_Legend				(BYTE type, long length, void* poResult);
		int ReadCT_Layout				(BYTE type, long length, void* poResult);
		int ReadCT_ManualLayout			(BYTE type, long length, void* poResult);
		int ReadCT_LayoutTarget			(BYTE type, long length, void* poResult);
		int ReadCT_LayoutMode			(BYTE type, long length, void* poResult);
		int ReadCT_Double				(BYTE type, long length, void* poResult);
		int ReadCT_DTable				(BYTE type, long length, void* poResult);
		int ReadCT_SerAx				(BYTE type, long length, void* poResult);
		int ReadCT_Scaling				(BYTE type, long length, void* poResult);
		int ReadCT_LogBase				(BYTE type, long length, void* poResult);
		int ReadCT_Orientation			(BYTE type, long length, void* poResult);
		int ReadCT_AxPos				(BYTE type, long length, void* poResult);
		int ReadCT_ChartLines			(BYTE type, long length, void* poResult);
		int ReadCT_Title				(BYTE type, long length, void* poResult);
		int ReadCT_Tx					(BYTE type, long length, void* poResult);
		int ReadCT_StrRef				(BYTE type, long length, void* poResult);
		int ReadCT_StrData				(BYTE type, long length, void* poResult);
		int ReadCT_StrVal				(BYTE type, long length, void* poResult);
		int ReadCT_NumFmt				(BYTE type, long length, void* poResult);
		int ReadCT_TickMark				(BYTE type, long length, void* poResult);
		int ReadCT_TickLblPos			(BYTE type, long length, void* poResult);
		int ReadCT_Crosses				(BYTE type, long length, void* poResult);
		int ReadCT_Skip					(BYTE type, long length, void* poResult);
		int ReadCT_TimeUnit				(BYTE type, long length, void* poResult);
		int ReadCT_DateAx				(BYTE type, long length, void* poResult);
		int ReadCT_LblOffset			(BYTE type, long length, void* poResult);
		int ReadCT_AxisUnit				(BYTE type, long length, void* poResult);
		int ReadCT_LblAlgn				(BYTE type, long length, void* poResult);
		int ReadCT_CatAx				(BYTE type, long length, void* poResult);
		int ReadCT_DispUnitsLbl			(BYTE type, long length, void* poResult);
		int ReadCT_BuiltInUnit			(BYTE type, long length, void* poResult);
		int ReadCT_DispUnits			(BYTE type, long length, void* poResult);
		int ReadCT_CrossBetween			(BYTE type, long length, void* poResult);
		int ReadCT_ValAx				(BYTE type, long length, void* poResult);
		int ReadCT_SizeRepresents		(BYTE type, long length, void* poResult);
		int ReadCT_BubbleScale			(BYTE type, long length, void* poResult);
		int ReadCT_BubbleSer			(BYTE type, long length, void* poResult);
		int ReadCT_SerTx				(BYTE type, long length, void* poResult);
		int ReadCT_DPt					(BYTE type, long length, void* poResult);
		int ReadCT_Marker				(BYTE type, long length, void* poResult);
		int ReadCT_MarkerStyle			(BYTE type, long length, void* poResult);
		int ReadCT_MarkerSize			(BYTE type, long length, void* poResult);
		int ReadCT_PictureOptions		(BYTE type, long length, void* poResult);
		int ReadCT_PictureFormat		(BYTE type, long length, void* poResult);
		int ReadCT_PictureStackUnit		(BYTE type, long length, void* poResult);
		int ReadCT_DLbls				(BYTE type, long length, void* poResult);
		int ReadCT_DLbl					(BYTE type, long length, void* poResult);
		int ReadCT_DLblPos				(BYTE type, long length, void* poResult);
		int ReadCT_Trendline			(BYTE type, long length, void* poResult);
		int ReadCT_TrendlineType		(BYTE type, long length, void* poResult);
		int ReadCT_Order				(BYTE type, long length, void* poResult);
		int ReadCT_Period				(BYTE type, long length, void* poResult);
		int ReadCT_TrendlineLbl			(BYTE type, long length, void* poResult);
		int ReadCT_ErrBars				(BYTE type, long length, void* poResult);
		int ReadCT_ErrDir				(BYTE type, long length, void* poResult);
		int ReadCT_ErrBarType			(BYTE type, long length, void* poResult);
		int ReadCT_ErrValType			(BYTE type, long length, void* poResult);
		int ReadCT_NumDataSource		(BYTE type, long length, void* poResult);
		int ReadCT_NumData				(BYTE type, long length, void* poResult);
		int ReadCT_NumVal				(BYTE type, long length, void* poResult);
		int ReadCT_NumRef				(BYTE type, long length, void* poResult);
		int ReadCT_AxDataSource			(BYTE type, long length, void* poResult);
		int ReadCT_MultiLvlStrRef		(BYTE type, long length, void* poResult);
		int ReadCT_lvl					(BYTE type, long length, void* poResult);
		int ReadCT_MultiLvlStrData		(BYTE type, long length, void* poResult);
		int ReadCT_BubbleChart			(BYTE type, long length, void* poResult);
		int ReadCT_bandFmts				(BYTE type, long length, void* poResult);
		int ReadCT_Surface3DChart		(BYTE type, long length, void* poResult);
		int ReadCT_SurfaceSer			(BYTE type, long length, void* poResult);
		int ReadCT_BandFmt				(BYTE type, long length, void* poResult);
		int ReadCT_SurfaceChart			(BYTE type, long length, void* poResult);
		int ReadCT_SecondPieSize		(BYTE type, long length, void* poResult);
		int ReadCT_SplitType			(BYTE type, long length, void* poResult);
		int ReadCT_OfPieType			(BYTE type, long length, void* poResult);
		int ReadCT_custSplit			(BYTE type, long length, void* poResult);
		int ReadCT_OfPieChart			(BYTE type, long length, void* poResult);
		int ReadCT_PieSer				(BYTE type, long length, void* poResult);
		int ReadCT_GapAmount			(BYTE type, long length, void* poResult);
		int ReadCT_Bar3DChart			(BYTE type, long length, void* poResult);
		int ReadCT_BarDir				(BYTE type, long length, void* poResult);
		int ReadCT_BarGrouping			(BYTE type, long length, void* poResult);
		int ReadCT_BarSer				(BYTE type, long length, void* poResult);
		int ReadCT_Shape				(BYTE type, long length, void* poResult);
		int ReadCT_Overlap				(BYTE type, long length, void* poResult);
		int ReadCT_BarChart				(BYTE type, long length, void* poResult);
		int ReadCT_HoleSize				(BYTE type, long length, void* poResult);
		int ReadCT_DoughnutChart		(BYTE type, long length, void* poResult);
		int ReadCT_FirstSliceAng		(BYTE type, long length, void* poResult);
		int ReadCT_Pie3DChart			(BYTE type, long length, void* poResult);
		int ReadCT_PieChart				(BYTE type, long length, void* poResult);
		int ReadCT_ScatterSer			(BYTE type, long length, void* poResult);
		int ReadCT_ScatterStyle			(BYTE type, long length, void* poResult);
		int ReadCT_ScatterChart			(BYTE type, long length, void* poResult);
		int ReadCT_RadarSer				(BYTE type, long length, void* poResult);
		int ReadCT_RadarStyle			(BYTE type, long length, void* poResult);
		int ReadCT_RadarChart			(BYTE type, long length, void* poResult);
		int ReadCT_StockChart			(BYTE type, long length, void* poResult);
		int ReadCT_LineSer				(BYTE type, long length, void* poResult);
		int ReadCT_UpDownBars			(BYTE type, long length, void* poResult);
		int ReadCT_UpDownBar			(BYTE type, long length, void* poResult);
		int ReadCT_Line3DChart			(BYTE type, long length, void* poResult);
		int ReadCT_Grouping				(BYTE type, long length, void* poResult);
		int ReadCT_LineChart			(BYTE type, long length, void* poResult);
		int ReadCT_Area3DChart			(BYTE type, long length, void* poResult);
		int ReadCT_AreaSer				(BYTE type, long length, void* poResult);
		int ReadCT_AreaChart			(BYTE type, long length, void* poResult);
		int ReadCT_PlotArea				(BYTE type, long length, void* poResult);
		int ReadCT_Thickness			(BYTE type, long length, void* poResult);
		int ReadCT_Surface				(BYTE type, long length, void* poResult);
		int ReadCT_Perspective			(BYTE type, long length, void* poResult);
		int ReadCT_DepthPercent			(BYTE type, long length, void* poResult);
		int ReadCT_RotY					(BYTE type, long length, void* poResult);
		int ReadCT_HPercent				(BYTE type, long length, void* poResult);
		int ReadCT_RotX					(BYTE type, long length, void* poResult);
		int ReadCT_View3D				(BYTE type, long length, void* poResult);
		int ReadCT_PivotFmt				(BYTE type, long length, void* poResult);
		int ReadCT_pivotFmts			(BYTE type, long length, void* poResult);
		int ReadCT_Chart				(BYTE type, long length, void* poResult);
		int ReadCT_Protection			(BYTE type, long length, void* poResult);
		int ReadCT_PivotSource			(BYTE type, long length, void* poResult);
		int ReadCT_Style1				(BYTE type, long length, void* poResult);
		int ReadCT_Style				(BYTE type, long length, void* poResult);
		int ReadCT_TextLanguageID		(BYTE type, long length, void* poResult);
		int ReadAlternateContent		(BYTE type, long length, void* poResult);
		int ReadAlternateContentChoice	(BYTE type, long length, void* poResult);
		int ReadAlternateContentFallback(BYTE type, long length, void* poResult);
		int ReadCT_SpPr					(BYTE type, long length, void* poResult);
		int ReadCT_TxPr					(BYTE type, long length, void* poResult);
	};
	class BinaryChartWriter
	{
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		BinaryCommonWriter				m_oBcw;
	public:
		BinaryChartWriter		(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		void WriteCT_extLst		(OOX::Spreadsheet::CT_extLst& oVal);
		void WriteCT_ChartSpace	(OOX::Spreadsheet::CChartSpace& ChartSpace);
		void WriteCT_Boolean	(OOX::Spreadsheet::CT_Boolean& oVal);
		void WriteCT_RelId		(OOX::Spreadsheet::CT_RelId& oVal);
		void WriteCT_PageSetup	(OOX::Spreadsheet::CT_PageSetup& oVal);
		void WriteCT_PageMargins	(OOX::Spreadsheet::CT_PageMargins& oVal);
		void WriteCT_HeaderFooter	(OOX::Spreadsheet::CT_HeaderFooter& oVal);
		void WriteCT_PrintSettings	(OOX::Spreadsheet::CT_PrintSettings& oVal);
		void WriteCT_ExternalData	(OOX::Spreadsheet::CT_ExternalData& oVal);
		void WriteCT_DispBlanksAs	(OOX::Spreadsheet::CT_DispBlanksAs& oVal);
		void WriteCT_LegendEntry(OOX::Spreadsheet::CT_LegendEntry& oVal);
		void WriteCT_UnsignedInt(OOX::Spreadsheet::CT_UnsignedInt& oVal);
		void WriteCT_Extension	(OOX::Spreadsheet::CT_Extension& oVal);
		void WriteCT_LegendPos	(OOX::Spreadsheet::CT_LegendPos& oVal);
		void WriteCT_Legend		(OOX::Spreadsheet::CT_Legend& oVal);
		void WriteCT_Layout			(OOX::Spreadsheet::CT_Layout& oVal);
		void WriteCT_ManualLayout	(OOX::Spreadsheet::CT_ManualLayout& oVal);
		void WriteCT_LayoutTarget	(OOX::Spreadsheet::CT_LayoutTarget& oVal);
		void WriteCT_LayoutMode		(OOX::Spreadsheet::CT_LayoutMode& oVal);
		void WriteCT_Double		(OOX::Spreadsheet::CT_Double& oVal);
		void WriteCT_DTable		(OOX::Spreadsheet::CT_DTable& oVal);
		void WriteCT_SerAx		(OOX::Spreadsheet::CT_SerAx& oVal);
		void WriteCT_Scaling	(OOX::Spreadsheet::CT_Scaling& oVal);
		void WriteCT_LogBase	(OOX::Spreadsheet::CT_LogBase& oVal);
		void WriteCT_Orientation(OOX::Spreadsheet::CT_Orientation& oVal);
		void WriteCT_AxPos		(OOX::Spreadsheet::CT_AxPos& oVal);
		void WriteCT_ChartLines	(OOX::Spreadsheet::CT_ChartLines& oVal);
		void WriteCT_Title		(OOX::Spreadsheet::CT_Title& oVal);
		void WriteCT_Tx			(OOX::Spreadsheet::CT_Tx& oVal);
		void WriteCT_StrRef		(OOX::Spreadsheet::CT_StrRef& oVal);
		void WriteCT_StrData	(OOX::Spreadsheet::CT_StrData& oVal);
		void WriteCT_StrVal		(OOX::Spreadsheet::CT_StrVal& oVal);
		void WriteCT_NumFmt		(OOX::Spreadsheet::CT_NumFmt& oVal);
		void WriteCT_TickMark	(OOX::Spreadsheet::CT_TickMark& oVal);
		void WriteCT_TickLblPos	(OOX::Spreadsheet::CT_TickLblPos& oVal);
		void WriteCT_Crosses	(OOX::Spreadsheet::CT_Crosses& oVal);
		void WriteCT_Skip		(OOX::Spreadsheet::CT_Skip& oVal);
		void WriteCT_TimeUnit	(OOX::Spreadsheet::CT_TimeUnit& oVal);
		void WriteCT_DateAx		(OOX::Spreadsheet::CT_DateAx& oVal);
		void WriteCT_LblOffset	(OOX::Spreadsheet::CT_LblOffset& oVal);
		void WriteCT_AxisUnit	(OOX::Spreadsheet::CT_AxisUnit& oVal);
		void WriteCT_LblAlgn	(OOX::Spreadsheet::CT_LblAlgn& oVal);
		void WriteCT_CatAx			(OOX::Spreadsheet::CT_CatAx& oVal);
		void WriteCT_DispUnitsLbl	(OOX::Spreadsheet::CT_DispUnitsLbl& oVal);
		void WriteCT_BuiltInUnit	(OOX::Spreadsheet::CT_BuiltInUnit& oVal);
		void WriteCT_DispUnits		(OOX::Spreadsheet::CT_DispUnits& oVal);
		void WriteCT_CrossBetween	(OOX::Spreadsheet::CT_CrossBetween& oVal);
		void WriteCT_ValAx			(OOX::Spreadsheet::CT_ValAx& oVal);
		void WriteCT_SizeRepresents	(OOX::Spreadsheet::CT_SizeRepresents& oVal);
		void WriteCT_BubbleScale	(OOX::Spreadsheet::CT_BubbleScale& oVal);
		void WriteCT_BubbleSer		(OOX::Spreadsheet::CT_BubbleSer& oVal);
		void WriteCT_SerTx			(OOX::Spreadsheet::CT_SerTx& oVal);
		void WriteCT_DPt			(OOX::Spreadsheet::CT_DPt& oVal);
		void WriteCT_Marker			(OOX::Spreadsheet::CT_Marker& oVal);
		void WriteCT_MarkerStyle	(OOX::Spreadsheet::CT_MarkerStyle& oVal);
		void WriteCT_MarkerSize		(OOX::Spreadsheet::CT_MarkerSize& oVal);
		void WriteCT_PictureOptions	(OOX::Spreadsheet::CT_PictureOptions& oVal);
		void WriteCT_PictureFormat	(OOX::Spreadsheet::CT_PictureFormat& oVal);
		void WriteCT_PictureStackUnit(OOX::Spreadsheet::CT_PictureStackUnit& oVal);
		void WriteCT_DLbls			(OOX::Spreadsheet::CT_DLbls& oVal);
		void WriteCT_DLbl			(OOX::Spreadsheet::CT_DLbl& oVal);
		void WriteCT_DLblPos		(OOX::Spreadsheet::CT_DLblPos& oVal);
		void WriteCT_Trendline		(OOX::Spreadsheet::CT_Trendline& oVal);
		void WriteCT_TrendlineType	(OOX::Spreadsheet::CT_TrendlineType& oVal);
		void WriteCT_Order			(OOX::Spreadsheet::CT_Order& oVal);
		void WriteCT_Period			(OOX::Spreadsheet::CT_Period& oVal);
		void WriteCT_TrendlineLbl	(OOX::Spreadsheet::CT_TrendlineLbl& oVal);
		void WriteCT_ErrBars		(OOX::Spreadsheet::CT_ErrBars& oVal);
		void WriteCT_ErrDir			(OOX::Spreadsheet::CT_ErrDir& oVal);
		void WriteCT_ErrBarType		(OOX::Spreadsheet::CT_ErrBarType& oVal);
		void WriteCT_ErrValType		(OOX::Spreadsheet::CT_ErrValType& oVal);
		void WriteCT_NumDataSource	(OOX::Spreadsheet::CT_NumDataSource& oVal);
		void WriteCT_NumData		(OOX::Spreadsheet::CT_NumData& oVal);
		void WriteCT_NumVal			(OOX::Spreadsheet::CT_NumVal& oVal);
		void WriteCT_NumRef			(OOX::Spreadsheet::CT_NumRef& oVal);
		void WriteCT_AxDataSource	(OOX::Spreadsheet::CT_AxDataSource& oVal);
		void WriteCT_MultiLvlStrRef	(OOX::Spreadsheet::CT_MultiLvlStrRef& oVal);
		void WriteCT_lvl			(OOX::Spreadsheet::CT_lvl& oVal);
		void WriteCT_MultiLvlStrData(OOX::Spreadsheet::CT_MultiLvlStrData& oVal);
		void WriteCT_BubbleChart	(OOX::Spreadsheet::CT_BubbleChart& oVal);
		void WriteCT_bandFmts		(OOX::Spreadsheet::CT_bandFmts& oVal);
		void WriteCT_Surface3DChart	(OOX::Spreadsheet::CT_Surface3DChart& oVal);
		void WriteCT_SurfaceSer		(OOX::Spreadsheet::CT_SurfaceSer& oVal);
		void WriteCT_BandFmt		(OOX::Spreadsheet::CT_BandFmt& oVal);
		void WriteCT_SurfaceChart	(OOX::Spreadsheet::CT_SurfaceChart& oVal);
		void WriteCT_SecondPieSize	(OOX::Spreadsheet::CT_SecondPieSize& oVal);
		void WriteCT_SplitType		(OOX::Spreadsheet::CT_SplitType& oVal);
		void WriteCT_OfPieType		(OOX::Spreadsheet::CT_OfPieType& oVal);
		void WriteCT_custSplit		(OOX::Spreadsheet::CT_custSplit& oVal);
		void WriteCT_OfPieChart		(OOX::Spreadsheet::CT_OfPieChart& oVal);
		void WriteCT_PieSer			(OOX::Spreadsheet::CT_PieSer& oVal);
		void WriteCT_GapAmount		(OOX::Spreadsheet::CT_GapAmount& oVal);
		void WriteCT_Bar3DChart		(OOX::Spreadsheet::CT_Bar3DChart& oVal);
		void WriteCT_BarDir			(OOX::Spreadsheet::CT_BarDir& oVal);
		void WriteCT_BarGrouping	(OOX::Spreadsheet::CT_BarGrouping& oVal);
		void WriteCT_BarSer			(OOX::Spreadsheet::CT_BarSer& oVal);
		void WriteCT_Shape			(OOX::Spreadsheet::CT_Shape& oVal);
		void WriteCT_Overlap		(OOX::Spreadsheet::CT_Overlap& oVal);
		void WriteCT_BarChart		(OOX::Spreadsheet::CT_BarChart& oVal);
		void WriteCT_HoleSize		(OOX::Spreadsheet::CT_HoleSize& oVal);
		void WriteCT_DoughnutChart	(OOX::Spreadsheet::CT_DoughnutChart& oVal);
		void WriteCT_FirstSliceAng	(OOX::Spreadsheet::CT_FirstSliceAng& oVal);
		void WriteCT_Pie3DChart		(OOX::Spreadsheet::CT_Pie3DChart& oVal);
		void WriteCT_PieChart		(OOX::Spreadsheet::CT_PieChart& oVal);
		void WriteCT_ScatterSer		(OOX::Spreadsheet::CT_ScatterSer& oVal);
		void WriteCT_ScatterStyle	(OOX::Spreadsheet::CT_ScatterStyle& oVal);
		void WriteCT_ScatterChart	(OOX::Spreadsheet::CT_ScatterChart& oVal);
		void WriteCT_RadarSer		(OOX::Spreadsheet::CT_RadarSer& oVal);
		void WriteCT_RadarStyle		(OOX::Spreadsheet::CT_RadarStyle& oVal);
		void WriteCT_RadarChart		(OOX::Spreadsheet::CT_RadarChart& oVal);
		void WriteCT_StockChart		(OOX::Spreadsheet::CT_StockChart& oVal);
		void WriteCT_LineSer		(OOX::Spreadsheet::CT_LineSer& oVal);
		void WriteCT_UpDownBars		(OOX::Spreadsheet::CT_UpDownBars& oVal);
		void WriteCT_UpDownBar		(OOX::Spreadsheet::CT_UpDownBar& oVal);
		void WriteCT_Line3DChart	(OOX::Spreadsheet::CT_Line3DChart& oVal);
		void WriteCT_Grouping		(OOX::Spreadsheet::CT_Grouping& oVal);
		void WriteCT_LineChart		(OOX::Spreadsheet::CT_LineChart& oVal);
		void WriteCT_Area3DChart	(OOX::Spreadsheet::CT_Area3DChart& oVal);
		void WriteCT_AreaSer		(OOX::Spreadsheet::CT_AreaSer& oVal);
		void WriteCT_AreaChart		(OOX::Spreadsheet::CT_AreaChart& oVal);
		void WriteCT_PlotArea		(OOX::Spreadsheet::CT_PlotArea& oVal);
		void WriteCT_Thickness		(OOX::Spreadsheet::CT_Thickness& oVal);
		void WriteCT_Surface		(OOX::Spreadsheet::CT_Surface& oVal);
		void WriteCT_Perspective	(OOX::Spreadsheet::CT_Perspective& oVal);
		void WriteCT_DepthPercent	(OOX::Spreadsheet::CT_DepthPercent& oVal);
		void WriteCT_RotY			(OOX::Spreadsheet::CT_RotY& oVal);
		void WriteCT_HPercent		(OOX::Spreadsheet::CT_HPercent& oVal);
		void WriteCT_RotX			(OOX::Spreadsheet::CT_RotX& oVal);
		void WriteCT_View3D			(OOX::Spreadsheet::CT_View3D& oVal);
		void WriteCT_PivotFmt		(OOX::Spreadsheet::CT_PivotFmt& oVal);
		void WriteCT_pivotFmts		(OOX::Spreadsheet::CT_pivotFmts& oVal);
		void WriteCT_Chart			(OOX::Spreadsheet::CT_Chart& oVal);
		void WriteCT_Protection		(OOX::Spreadsheet::CT_Protection& oVal);
		void WriteCT_PivotSource	(OOX::Spreadsheet::CT_PivotSource& oVal);
		void WriteCT_Style1			(OOX::Spreadsheet::CT_Style1& oVal);
		void WriteCT_Style			(OOX::Spreadsheet::CT_Style& oVal);
		void WriteCT_TextLanguageID	(OOX::Spreadsheet::CT_TextLanguageID& oVal);
	
		void WriteAlternateContent			(OOX::Spreadsheet::AlternateContent& oVal);
		void WriteAlternateContentChoice	(OOX::Spreadsheet::AlternateContentChoice& oVal);
		void WriteAlternateContentFallback	(OOX::Spreadsheet::AlternateContentFallback& oVal);

	private: 
		void toBin(OOX::Spreadsheet::ItemsChoiceType5 eType, void* pVal);
		void toBin(OOX::Spreadsheet::ItemsChoiceType6 eType, void* pVal);
		void toBin(OOX::Spreadsheet::ItemsChoiceType4 eType, void* pVal);
		void toBin(OOX::Spreadsheet::ItemsChoiceType3 eType, void* pVal);
	};
}
#endif	// #ifndef CHART_FROM_TO_BINARY
