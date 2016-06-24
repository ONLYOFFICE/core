/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
//Generated code
#ifndef CHART_FROM_TO_BINARY
#define CHART_FROM_TO_BINARY

#include "CommonWriter.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Chart/ChartSerialize.h"
#include "../Writer/BinaryCommonReader.h"
#include "../../ASCOfficePPTXFile/ASCOfficeDrawingConverter.h"

using namespace OOX::Spreadsheet;
namespace BinXlsxRW {
	class SaveParams{
	public:
		CString sThemePath;
		CString sAdditionalContentTypes;
		int nThemeOverrideCount;
		SaveParams(const CString& _sThemePath);
	};

	class BinaryChartReader : public Binary_CommonReader<BinaryChartReader>
	{
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		SaveParams& m_oSaveParams;
	public:
		BinaryChartReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
	private: CString* GetRecordXml(long length, int nRecordType);
	private: CString* GetTxBodyXml(long length);
	private: int ReadCT_extLst(BYTE type, long length, void* poResult);
	public: int ReadCT_ChartSpace(long length, CT_ChartSpace* poResult);
	private: int ReadCT_ChartSpace(BYTE type, long length, void* poResult);
	private: int ReadCT_Boolean(BYTE type, long length, void* poResult);
	public: int ReadCT_RelId(long length, CT_RelId* poResult);
	private: int ReadCT_RelId(BYTE type, long length, void* poResult);
	private: int ReadCT_PageSetup(BYTE type, long length, void* poResult);
	private: int ReadCT_PageMargins(BYTE type, long length, void* poResult);
	private: int ReadCT_HeaderFooter(BYTE type, long length, void* poResult);
	private: int ReadCT_PrintSettings(BYTE type, long length, void* poResult);
	private: int ReadCT_ExternalData(BYTE type, long length, void* poResult);
	private: int ReadCT_DispBlanksAs(BYTE type, long length, void* poResult);
	private: int ReadCT_LegendEntry(BYTE type, long length, void* poResult);
	private: int ReadCT_UnsignedInt(BYTE type, long length, void* poResult);
	private: int ReadCT_Extension(BYTE type, long length, void* poResult);
	private: int ReadCT_LegendPos(BYTE type, long length, void* poResult);
	private: int ReadCT_Legend(BYTE type, long length, void* poResult);
	private: int ReadCT_Layout(BYTE type, long length, void* poResult);
	private: int ReadCT_ManualLayout(BYTE type, long length, void* poResult);
	private: int ReadCT_LayoutTarget(BYTE type, long length, void* poResult);
	private: int ReadCT_LayoutMode(BYTE type, long length, void* poResult);
	private: int ReadCT_Double(BYTE type, long length, void* poResult);
	private: int ReadCT_DTable(BYTE type, long length, void* poResult);
	private: int ReadCT_SerAx(BYTE type, long length, void* poResult);
	private: int ReadCT_Scaling(BYTE type, long length, void* poResult);
	private: int ReadCT_LogBase(BYTE type, long length, void* poResult);
	private: int ReadCT_Orientation(BYTE type, long length, void* poResult);
	private: int ReadCT_AxPos(BYTE type, long length, void* poResult);
	private: int ReadCT_ChartLines(BYTE type, long length, void* poResult);
	private: int ReadCT_Title(BYTE type, long length, void* poResult);
	private: int ReadCT_Tx(BYTE type, long length, void* poResult);
	private: int ReadCT_StrRef(BYTE type, long length, void* poResult);
	private: int ReadCT_StrData(BYTE type, long length, void* poResult);
	private: int ReadCT_StrVal(BYTE type, long length, void* poResult);
	private: int ReadCT_NumFmt(BYTE type, long length, void* poResult);
	private: int ReadCT_TickMark(BYTE type, long length, void* poResult);
	private: int ReadCT_TickLblPos(BYTE type, long length, void* poResult);
	private: int ReadCT_Crosses(BYTE type, long length, void* poResult);
	private: int ReadCT_Skip(BYTE type, long length, void* poResult);
	private: int ReadCT_TimeUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_DateAx(BYTE type, long length, void* poResult);
	private: int ReadCT_LblOffset(BYTE type, long length, void* poResult);
	private: int ReadCT_AxisUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_LblAlgn(BYTE type, long length, void* poResult);
	private: int ReadCT_CatAx(BYTE type, long length, void* poResult);
	private: int ReadCT_DispUnitsLbl(BYTE type, long length, void* poResult);
	private: int ReadCT_BuiltInUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_DispUnits(BYTE type, long length, void* poResult);
	private: int ReadCT_CrossBetween(BYTE type, long length, void* poResult);
	private: int ReadCT_ValAx(BYTE type, long length, void* poResult);
	private: int ReadCT_SizeRepresents(BYTE type, long length, void* poResult);
	private: int ReadCT_BubbleScale(BYTE type, long length, void* poResult);
	private: int ReadCT_BubbleSer(BYTE type, long length, void* poResult);
	private: int ReadCT_SerTx(BYTE type, long length, void* poResult);
	private: int ReadCT_DPt(BYTE type, long length, void* poResult);
	private: int ReadCT_Marker(BYTE type, long length, void* poResult);
	private: int ReadCT_MarkerStyle(BYTE type, long length, void* poResult);
	private: int ReadCT_MarkerSize(BYTE type, long length, void* poResult);
	private: int ReadCT_PictureOptions(BYTE type, long length, void* poResult);
	private: int ReadCT_PictureFormat(BYTE type, long length, void* poResult);
	private: int ReadCT_PictureStackUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_DLbls(BYTE type, long length, void* poResult);
	private: int ReadCT_DLbl(BYTE type, long length, void* poResult);
	private: int ReadCT_DLblPos(BYTE type, long length, void* poResult);
	private: int ReadCT_Trendline(BYTE type, long length, void* poResult);
	private: int ReadCT_TrendlineType(BYTE type, long length, void* poResult);
	private: int ReadCT_Order(BYTE type, long length, void* poResult);
	private: int ReadCT_Period(BYTE type, long length, void* poResult);
	private: int ReadCT_TrendlineLbl(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrBars(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrDir(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrBarType(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrValType(BYTE type, long length, void* poResult);
	private: int ReadCT_NumDataSource(BYTE type, long length, void* poResult);
	private: int ReadCT_NumData(BYTE type, long length, void* poResult);
	private: int ReadCT_NumVal(BYTE type, long length, void* poResult);
	private: int ReadCT_NumRef(BYTE type, long length, void* poResult);
	private: int ReadCT_AxDataSource(BYTE type, long length, void* poResult);
	private: int ReadCT_MultiLvlStrRef(BYTE type, long length, void* poResult);
	private: int ReadCT_lvl(BYTE type, long length, void* poResult);
	private: int ReadCT_MultiLvlStrData(BYTE type, long length, void* poResult);
	private: int ReadCT_BubbleChart(BYTE type, long length, void* poResult);
	private: int ReadCT_bandFmts(BYTE type, long length, void* poResult);
	private: int ReadCT_Surface3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_SurfaceSer(BYTE type, long length, void* poResult);
	private: int ReadCT_BandFmt(BYTE type, long length, void* poResult);
	private: int ReadCT_SurfaceChart(BYTE type, long length, void* poResult);
	private: int ReadCT_SecondPieSize(BYTE type, long length, void* poResult);
	private: int ReadCT_SplitType(BYTE type, long length, void* poResult);
	private: int ReadCT_OfPieType(BYTE type, long length, void* poResult);
	private: int ReadCT_custSplit(BYTE type, long length, void* poResult);
	private: int ReadCT_OfPieChart(BYTE type, long length, void* poResult);
	private: int ReadCT_PieSer(BYTE type, long length, void* poResult);
	private: int ReadCT_GapAmount(BYTE type, long length, void* poResult);
	private: int ReadCT_Bar3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_BarDir(BYTE type, long length, void* poResult);
	private: int ReadCT_BarGrouping(BYTE type, long length, void* poResult);
	private: int ReadCT_BarSer(BYTE type, long length, void* poResult);
	private: int ReadCT_Shape(BYTE type, long length, void* poResult);
	private: int ReadCT_Overlap(BYTE type, long length, void* poResult);
	private: int ReadCT_BarChart(BYTE type, long length, void* poResult);
	private: int ReadCT_HoleSize(BYTE type, long length, void* poResult);
	private: int ReadCT_DoughnutChart(BYTE type, long length, void* poResult);
	private: int ReadCT_FirstSliceAng(BYTE type, long length, void* poResult);
	private: int ReadCT_Pie3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_PieChart(BYTE type, long length, void* poResult);
	private: int ReadCT_ScatterSer(BYTE type, long length, void* poResult);
	private: int ReadCT_ScatterStyle(BYTE type, long length, void* poResult);
	private: int ReadCT_ScatterChart(BYTE type, long length, void* poResult);
	private: int ReadCT_RadarSer(BYTE type, long length, void* poResult);
	private: int ReadCT_RadarStyle(BYTE type, long length, void* poResult);
	private: int ReadCT_RadarChart(BYTE type, long length, void* poResult);
	private: int ReadCT_StockChart(BYTE type, long length, void* poResult);
	private: int ReadCT_LineSer(BYTE type, long length, void* poResult);
	private: int ReadCT_UpDownBars(BYTE type, long length, void* poResult);
	private: int ReadCT_UpDownBar(BYTE type, long length, void* poResult);
	private: int ReadCT_Line3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_Grouping(BYTE type, long length, void* poResult);
	private: int ReadCT_LineChart(BYTE type, long length, void* poResult);
	private: int ReadCT_Area3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_AreaSer(BYTE type, long length, void* poResult);
	private: int ReadCT_AreaChart(BYTE type, long length, void* poResult);
	private: int ReadCT_PlotArea(BYTE type, long length, void* poResult);
	private: int ReadCT_Thickness(BYTE type, long length, void* poResult);
	private: int ReadCT_Surface(BYTE type, long length, void* poResult);
	private: int ReadCT_Perspective(BYTE type, long length, void* poResult);
	private: int ReadCT_DepthPercent(BYTE type, long length, void* poResult);
	private: int ReadCT_RotY(BYTE type, long length, void* poResult);
	private: int ReadCT_HPercent(BYTE type, long length, void* poResult);
	private: int ReadCT_RotX(BYTE type, long length, void* poResult);
	private: int ReadCT_View3D(BYTE type, long length, void* poResult);
	private: int ReadCT_PivotFmt(BYTE type, long length, void* poResult);
	private: int ReadCT_pivotFmts(BYTE type, long length, void* poResult);
	private: int ReadCT_Chart(BYTE type, long length, void* poResult);
	private: int ReadCT_Protection(BYTE type, long length, void* poResult);
	private: int ReadCT_PivotSource(BYTE type, long length, void* poResult);
	private: int ReadCT_Style1(BYTE type, long length, void* poResult);
	private: int ReadCT_Style(BYTE type, long length, void* poResult);
	private: int ReadCT_TextLanguageID(BYTE type, long length, void* poResult);
	private: int ReadAlternateContent(BYTE type, long length, void* poResult);
	private: int ReadAlternateContentChoice(BYTE type, long length, void* poResult);
	private: int ReadAlternateContentFallback(BYTE type, long length, void* poResult);
	};
	class BinaryChartWriter
	{
		BinaryCommonWriter m_oBcw;
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
	public:
		BinaryChartWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
	private: void GetRecordBinary(int nType, CString& sXml, int nRecordType);
	private: void GetTxBodyBinary(int nType, CString& sXml);
	public: void WriteCT_extLst(CT_extLst& oVal);
	public: void WriteCT_ChartSpace(OOX::Spreadsheet::CChartSpace& ChartSpace);
	public: void WriteCT_Boolean(CT_Boolean& oVal);
	public: void WriteCT_RelId(CT_RelId& oVal);
	public: void WriteCT_PageSetup(CT_PageSetup& oVal);
	public: void WriteCT_PageMargins(CT_PageMargins& oVal);
	public: void WriteCT_HeaderFooter(CT_HeaderFooter& oVal);
	public: void WriteCT_PrintSettings(CT_PrintSettings& oVal);
	public: void WriteCT_ExternalData(CT_ExternalData& oVal);
	public: void WriteCT_DispBlanksAs(CT_DispBlanksAs& oVal);
	public: void WriteCT_LegendEntry(CT_LegendEntry& oVal);
	public: void WriteCT_UnsignedInt(CT_UnsignedInt& oVal);
	public: void WriteCT_Extension(CT_Extension& oVal);
	public: void WriteCT_LegendPos(CT_LegendPos& oVal);
	public: void WriteCT_Legend(CT_Legend& oVal);
	public: void WriteCT_Layout(CT_Layout& oVal);
	public: void WriteCT_ManualLayout(CT_ManualLayout& oVal);
	public: void WriteCT_LayoutTarget(CT_LayoutTarget& oVal);
	public: void WriteCT_LayoutMode(CT_LayoutMode& oVal);
	public: void WriteCT_Double(CT_Double& oVal);
	public: void WriteCT_DTable(CT_DTable& oVal);
	public: void WriteCT_SerAx(CT_SerAx& oVal);
	public: void WriteCT_Scaling(CT_Scaling& oVal);
	public: void WriteCT_LogBase(CT_LogBase& oVal);
	public: void WriteCT_Orientation(CT_Orientation& oVal);
	public: void WriteCT_AxPos(CT_AxPos& oVal);
	public: void WriteCT_ChartLines(CT_ChartLines& oVal);
	public: void WriteCT_Title(CT_Title& oVal);
	public: void WriteCT_Tx(CT_Tx& oVal);
	public: void WriteCT_StrRef(CT_StrRef& oVal);
	public: void WriteCT_StrData(CT_StrData& oVal);
	public: void WriteCT_StrVal(CT_StrVal& oVal);
	public: void WriteCT_NumFmt(CT_NumFmt& oVal);
	public: void WriteCT_TickMark(CT_TickMark& oVal);
	public: void WriteCT_TickLblPos(CT_TickLblPos& oVal);
	public: void WriteCT_Crosses(CT_Crosses& oVal);
	public: void WriteCT_Skip(CT_Skip& oVal);
	public: void WriteCT_TimeUnit(CT_TimeUnit& oVal);
	public: void WriteCT_DateAx(CT_DateAx& oVal);
	public: void WriteCT_LblOffset(CT_LblOffset& oVal);
	public: void WriteCT_AxisUnit(CT_AxisUnit& oVal);
	public: void WriteCT_LblAlgn(CT_LblAlgn& oVal);
	public: void WriteCT_CatAx(CT_CatAx& oVal);
	public: void WriteCT_DispUnitsLbl(CT_DispUnitsLbl& oVal);
	public: void WriteCT_BuiltInUnit(CT_BuiltInUnit& oVal);
	public: void WriteCT_DispUnits(CT_DispUnits& oVal);
	public: void WriteCT_CrossBetween(CT_CrossBetween& oVal);
	public: void WriteCT_ValAx(CT_ValAx& oVal);
	public: void WriteCT_SizeRepresents(CT_SizeRepresents& oVal);
	public: void WriteCT_BubbleScale(CT_BubbleScale& oVal);
	public: void WriteCT_BubbleSer(CT_BubbleSer& oVal);
	public: void WriteCT_SerTx(CT_SerTx& oVal);
	public: void WriteCT_DPt(CT_DPt& oVal);
	public: void WriteCT_Marker(CT_Marker& oVal);
	public: void WriteCT_MarkerStyle(CT_MarkerStyle& oVal);
	public: void WriteCT_MarkerSize(CT_MarkerSize& oVal);
	public: void WriteCT_PictureOptions(CT_PictureOptions& oVal);
	public: void WriteCT_PictureFormat(CT_PictureFormat& oVal);
	public: void WriteCT_PictureStackUnit(CT_PictureStackUnit& oVal);
	public: void WriteCT_DLbls(CT_DLbls& oVal);
	private: void toBin(ItemsChoiceType3 eType, void* pVal);
	public: void WriteCT_DLbl(CT_DLbl& oVal);
	private: void toBin(ItemsChoiceType4 eType, void* pVal);
	public: void WriteCT_DLblPos(CT_DLblPos& oVal);
	public: void WriteCT_Trendline(CT_Trendline& oVal);
	public: void WriteCT_TrendlineType(CT_TrendlineType& oVal);
	public: void WriteCT_Order(CT_Order& oVal);
	public: void WriteCT_Period(CT_Period& oVal);
	public: void WriteCT_TrendlineLbl(CT_TrendlineLbl& oVal);
	public: void WriteCT_ErrBars(CT_ErrBars& oVal);
	public: void WriteCT_ErrDir(CT_ErrDir& oVal);
	public: void WriteCT_ErrBarType(CT_ErrBarType& oVal);
	public: void WriteCT_ErrValType(CT_ErrValType& oVal);
	public: void WriteCT_NumDataSource(CT_NumDataSource& oVal);
	public: void WriteCT_NumData(CT_NumData& oVal);
	public: void WriteCT_NumVal(CT_NumVal& oVal);
	public: void WriteCT_NumRef(CT_NumRef& oVal);
	public: void WriteCT_AxDataSource(CT_AxDataSource& oVal);
	public: void WriteCT_MultiLvlStrRef(CT_MultiLvlStrRef& oVal);
	public: void WriteCT_lvl(CT_lvl& oVal);
	public: void WriteCT_MultiLvlStrData(CT_MultiLvlStrData& oVal);
	public: void WriteCT_BubbleChart(CT_BubbleChart& oVal);
	public: void WriteCT_bandFmts(CT_bandFmts& oVal);
	public: void WriteCT_Surface3DChart(CT_Surface3DChart& oVal);
	public: void WriteCT_SurfaceSer(CT_SurfaceSer& oVal);
	public: void WriteCT_BandFmt(CT_BandFmt& oVal);
	public: void WriteCT_SurfaceChart(CT_SurfaceChart& oVal);
	public: void WriteCT_SecondPieSize(CT_SecondPieSize& oVal);
	public: void WriteCT_SplitType(CT_SplitType& oVal);
	public: void WriteCT_OfPieType(CT_OfPieType& oVal);
	public: void WriteCT_custSplit(CT_custSplit& oVal);
	public: void WriteCT_OfPieChart(CT_OfPieChart& oVal);
	public: void WriteCT_PieSer(CT_PieSer& oVal);
	public: void WriteCT_GapAmount(CT_GapAmount& oVal);
	public: void WriteCT_Bar3DChart(CT_Bar3DChart& oVal);
	public: void WriteCT_BarDir(CT_BarDir& oVal);
	public: void WriteCT_BarGrouping(CT_BarGrouping& oVal);
	public: void WriteCT_BarSer(CT_BarSer& oVal);
	public: void WriteCT_Shape(CT_Shape& oVal);
	public: void WriteCT_Overlap(CT_Overlap& oVal);
	public: void WriteCT_BarChart(CT_BarChart& oVal);
	public: void WriteCT_HoleSize(CT_HoleSize& oVal);
	public: void WriteCT_DoughnutChart(CT_DoughnutChart& oVal);
	public: void WriteCT_FirstSliceAng(CT_FirstSliceAng& oVal);
	public: void WriteCT_Pie3DChart(CT_Pie3DChart& oVal);
	public: void WriteCT_PieChart(CT_PieChart& oVal);
	public: void WriteCT_ScatterSer(CT_ScatterSer& oVal);
	public: void WriteCT_ScatterStyle(CT_ScatterStyle& oVal);
	public: void WriteCT_ScatterChart(CT_ScatterChart& oVal);
	public: void WriteCT_RadarSer(CT_RadarSer& oVal);
	public: void WriteCT_RadarStyle(CT_RadarStyle& oVal);
	public: void WriteCT_RadarChart(CT_RadarChart& oVal);
	public: void WriteCT_StockChart(CT_StockChart& oVal);
	public: void WriteCT_LineSer(CT_LineSer& oVal);
	public: void WriteCT_UpDownBars(CT_UpDownBars& oVal);
	public: void WriteCT_UpDownBar(CT_UpDownBar& oVal);
	public: void WriteCT_Line3DChart(CT_Line3DChart& oVal);
	public: void WriteCT_Grouping(CT_Grouping& oVal);
	public: void WriteCT_LineChart(CT_LineChart& oVal);
	public: void WriteCT_Area3DChart(CT_Area3DChart& oVal);
	public: void WriteCT_AreaSer(CT_AreaSer& oVal);
	public: void WriteCT_AreaChart(CT_AreaChart& oVal);
	public: void WriteCT_PlotArea(CT_PlotArea& oVal);
	private: void toBin(ItemsChoiceType5 eType, void* pVal);
	private: void toBin(ItemsChoiceType6 eType, void* pVal);
	public: void WriteCT_Thickness(CT_Thickness& oVal);
	public: void WriteCT_Surface(CT_Surface& oVal);
	public: void WriteCT_Perspective(CT_Perspective& oVal);
	public: void WriteCT_DepthPercent(CT_DepthPercent& oVal);
	public: void WriteCT_RotY(CT_RotY& oVal);
	public: void WriteCT_HPercent(CT_HPercent& oVal);
	public: void WriteCT_RotX(CT_RotX& oVal);
	public: void WriteCT_View3D(CT_View3D& oVal);
	public: void WriteCT_PivotFmt(CT_PivotFmt& oVal);
	public: void WriteCT_pivotFmts(CT_pivotFmts& oVal);
	public: void WriteCT_Chart(CT_Chart& oVal);
	public: void WriteCT_Protection(CT_Protection& oVal);
	public: void WriteCT_PivotSource(CT_PivotSource& oVal);
	public: void WriteCT_Style1(CT_Style1& oVal);
	public: void WriteCT_Style(CT_Style& oVal);
	public: void WriteCT_TextLanguageID(CT_TextLanguageID& oVal);
	public: void WriteAlternateContent(AlternateContent& oVal);
	public: void WriteAlternateContentChoice(AlternateContentChoice& oVal);
	public: void WriteAlternateContentFallback(AlternateContentFallback& oVal);
	};
}
#endif	// #ifndef CHART_FROM_TO_BINARY
