/*
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

using namespace OOX::Spreadsheet;
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
		
		int ReadCT_ChartSpace			(long length, CT_ChartSpace* poResult);
		int ReadCT_RelId				(long length, CT_RelId* poResult);
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
		void WriteCT_extLst		(CT_extLst& oVal);
		void WriteCT_ChartSpace	(OOX::Spreadsheet::CChartSpace& ChartSpace);
		void WriteCT_Boolean	(CT_Boolean& oVal);
		void WriteCT_RelId		(CT_RelId& oVal);
		void WriteCT_PageSetup	(CT_PageSetup& oVal);
		void WriteCT_PageMargins(CT_PageMargins& oVal);
		void WriteCT_HeaderFooter(CT_HeaderFooter& oVal);
		void WriteCT_PrintSettings(CT_PrintSettings& oVal);
		void WriteCT_ExternalData(CT_ExternalData& oVal);
		void WriteCT_DispBlanksAs(CT_DispBlanksAs& oVal);
		void WriteCT_LegendEntry(CT_LegendEntry& oVal);
		void WriteCT_UnsignedInt(CT_UnsignedInt& oVal);
		void WriteCT_Extension	(CT_Extension& oVal);
		void WriteCT_LegendPos	(CT_LegendPos& oVal);
		void WriteCT_Legend		(CT_Legend& oVal);
		void WriteCT_Layout		(CT_Layout& oVal);
		void WriteCT_ManualLayout(CT_ManualLayout& oVal);
		void WriteCT_LayoutTarget(CT_LayoutTarget& oVal);
		void WriteCT_LayoutMode	(CT_LayoutMode& oVal);
		void WriteCT_Double		(CT_Double& oVal);
		void WriteCT_DTable		(CT_DTable& oVal);
		void WriteCT_SerAx		(CT_SerAx& oVal);
		void WriteCT_Scaling	(CT_Scaling& oVal);
		void WriteCT_LogBase	(CT_LogBase& oVal);
		void WriteCT_Orientation(CT_Orientation& oVal);
		void WriteCT_AxPos		(CT_AxPos& oVal);
		void WriteCT_ChartLines	(CT_ChartLines& oVal);
		void WriteCT_Title		(CT_Title& oVal);
		void WriteCT_Tx			(CT_Tx& oVal);
		void WriteCT_StrRef(CT_StrRef& oVal);
		void WriteCT_StrData(CT_StrData& oVal);
		void WriteCT_StrVal(CT_StrVal& oVal);
		void WriteCT_NumFmt(CT_NumFmt& oVal);
		void WriteCT_TickMark(CT_TickMark& oVal);
		void WriteCT_TickLblPos(CT_TickLblPos& oVal);
		void WriteCT_Crosses(CT_Crosses& oVal);
		void WriteCT_Skip(CT_Skip& oVal);
		void WriteCT_TimeUnit(CT_TimeUnit& oVal);
		void WriteCT_DateAx(CT_DateAx& oVal);
		void WriteCT_LblOffset(CT_LblOffset& oVal);
		void WriteCT_AxisUnit(CT_AxisUnit& oVal);
		void WriteCT_LblAlgn(CT_LblAlgn& oVal);
		void WriteCT_CatAx(CT_CatAx& oVal);
		void WriteCT_DispUnitsLbl(CT_DispUnitsLbl& oVal);
		void WriteCT_BuiltInUnit(CT_BuiltInUnit& oVal);
		void WriteCT_DispUnits(CT_DispUnits& oVal);
		void WriteCT_CrossBetween(CT_CrossBetween& oVal);
		void WriteCT_ValAx(CT_ValAx& oVal);
		void WriteCT_SizeRepresents(CT_SizeRepresents& oVal);
		void WriteCT_BubbleScale(CT_BubbleScale& oVal);
		void WriteCT_BubbleSer(CT_BubbleSer& oVal);
		void WriteCT_SerTx(CT_SerTx& oVal);
		void WriteCT_DPt(CT_DPt& oVal);
		void WriteCT_Marker(CT_Marker& oVal);
		void WriteCT_MarkerStyle(CT_MarkerStyle& oVal);
		void WriteCT_MarkerSize(CT_MarkerSize& oVal);
		void WriteCT_PictureOptions(CT_PictureOptions& oVal);
		void WriteCT_PictureFormat(CT_PictureFormat& oVal);
		void WriteCT_PictureStackUnit(CT_PictureStackUnit& oVal);
		void WriteCT_DLbls(CT_DLbls& oVal);
		void WriteCT_DLbl(CT_DLbl& oVal);
		void WriteCT_DLblPos(CT_DLblPos& oVal);
		void WriteCT_Trendline(CT_Trendline& oVal);
		void WriteCT_TrendlineType(CT_TrendlineType& oVal);
		void WriteCT_Order(CT_Order& oVal);
		void WriteCT_Period(CT_Period& oVal);
		void WriteCT_TrendlineLbl(CT_TrendlineLbl& oVal);
		void WriteCT_ErrBars(CT_ErrBars& oVal);
		void WriteCT_ErrDir(CT_ErrDir& oVal);
		void WriteCT_ErrBarType(CT_ErrBarType& oVal);
		void WriteCT_ErrValType(CT_ErrValType& oVal);
		void WriteCT_NumDataSource(CT_NumDataSource& oVal);
		void WriteCT_NumData(CT_NumData& oVal);
		void WriteCT_NumVal(CT_NumVal& oVal);
		void WriteCT_NumRef(CT_NumRef& oVal);
		void WriteCT_AxDataSource(CT_AxDataSource& oVal);
		void WriteCT_MultiLvlStrRef(CT_MultiLvlStrRef& oVal);
		void WriteCT_lvl(CT_lvl& oVal);
		void WriteCT_MultiLvlStrData(CT_MultiLvlStrData& oVal);
		void WriteCT_BubbleChart(CT_BubbleChart& oVal);
		void WriteCT_bandFmts(CT_bandFmts& oVal);
		void WriteCT_Surface3DChart(CT_Surface3DChart& oVal);
		void WriteCT_SurfaceSer(CT_SurfaceSer& oVal);
		void WriteCT_BandFmt(CT_BandFmt& oVal);
		void WriteCT_SurfaceChart(CT_SurfaceChart& oVal);
		void WriteCT_SecondPieSize(CT_SecondPieSize& oVal);
		void WriteCT_SplitType(CT_SplitType& oVal);
		void WriteCT_OfPieType(CT_OfPieType& oVal);
		void WriteCT_custSplit(CT_custSplit& oVal);
		void WriteCT_OfPieChart(CT_OfPieChart& oVal);
		void WriteCT_PieSer(CT_PieSer& oVal);
		void WriteCT_GapAmount(CT_GapAmount& oVal);
		void WriteCT_Bar3DChart(CT_Bar3DChart& oVal);
		void WriteCT_BarDir(CT_BarDir& oVal);
		void WriteCT_BarGrouping(CT_BarGrouping& oVal);
		void WriteCT_BarSer(CT_BarSer& oVal);
		void WriteCT_Shape(CT_Shape& oVal);
		void WriteCT_Overlap(CT_Overlap& oVal);
		void WriteCT_BarChart(CT_BarChart& oVal);
		void WriteCT_HoleSize(CT_HoleSize& oVal);
		void WriteCT_DoughnutChart(CT_DoughnutChart& oVal);
		void WriteCT_FirstSliceAng(CT_FirstSliceAng& oVal);
		void WriteCT_Pie3DChart(CT_Pie3DChart& oVal);
		void WriteCT_PieChart(CT_PieChart& oVal);
		void WriteCT_ScatterSer(CT_ScatterSer& oVal);
		void WriteCT_ScatterStyle(CT_ScatterStyle& oVal);
		void WriteCT_ScatterChart(CT_ScatterChart& oVal);
		void WriteCT_RadarSer(CT_RadarSer& oVal);
		void WriteCT_RadarStyle(CT_RadarStyle& oVal);
		void WriteCT_RadarChart(CT_RadarChart& oVal);
		void WriteCT_StockChart(CT_StockChart& oVal);
		void WriteCT_LineSer(CT_LineSer& oVal);
		void WriteCT_UpDownBars(CT_UpDownBars& oVal);
		void WriteCT_UpDownBar(CT_UpDownBar& oVal);
		void WriteCT_Line3DChart(CT_Line3DChart& oVal);
		void WriteCT_Grouping(CT_Grouping& oVal);
		void WriteCT_LineChart(CT_LineChart& oVal);
		void WriteCT_Area3DChart(CT_Area3DChart& oVal);
		void WriteCT_AreaSer(CT_AreaSer& oVal);
		void WriteCT_AreaChart(CT_AreaChart& oVal);
		void WriteCT_PlotArea(CT_PlotArea& oVal);
		void WriteCT_Thickness(CT_Thickness& oVal);
		void WriteCT_Surface(CT_Surface& oVal);
		void WriteCT_Perspective(CT_Perspective& oVal);
		void WriteCT_DepthPercent(CT_DepthPercent& oVal);
		void WriteCT_RotY(CT_RotY& oVal);
		void WriteCT_HPercent(CT_HPercent& oVal);
		void WriteCT_RotX(CT_RotX& oVal);
		void WriteCT_View3D(CT_View3D& oVal);
		void WriteCT_PivotFmt(CT_PivotFmt& oVal);
		void WriteCT_pivotFmts(CT_pivotFmts& oVal);
		void WriteCT_Chart(CT_Chart& oVal);
		void WriteCT_Protection(CT_Protection& oVal);
		void WriteCT_PivotSource(CT_PivotSource& oVal);
		void WriteCT_Style1(CT_Style1& oVal);
		void WriteCT_Style(CT_Style& oVal);
		void WriteCT_TextLanguageID(CT_TextLanguageID& oVal);
		void WriteAlternateContent(AlternateContent& oVal);
		void WriteAlternateContentChoice(AlternateContentChoice& oVal);
		void WriteAlternateContentFallback(AlternateContentFallback& oVal);

	private: 
		void toBin(ItemsChoiceType5 eType, void* pVal);
		void toBin(ItemsChoiceType6 eType, void* pVal);
		void toBin(ItemsChoiceType4 eType, void* pVal);
		void toBin(ItemsChoiceType3 eType, void* pVal);
	};
}
#endif	// #ifndef CHART_FROM_TO_BINARY
