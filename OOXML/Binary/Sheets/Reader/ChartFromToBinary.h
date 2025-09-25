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

#include "CommonWriter.h"
#include "../../../XlsxFormat/Chart/ChartSerialize.h"
#include "../Writer/BinaryCommonReader.h"
#include "../../../PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h"
#include "../../Sheets/Writer/CSVWriter.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartFile;
		class CChartStyleFile;
		class CChartColorsFile;
		class CChartExFile;
		namespace ChartEx
		{
			class CChartData;
			class CChart;
			class CData;
			class CExternalData;
			class CStrDimension;
			class CNumDimension; 
			class CNumericLevel;
			class CStringLevel;
			class CNumericValue;
			class CStringValue;
			class CFormula;
			class CPlotArea;
			class CTitle;
			class CLegend;
			class CPlotAreaRegion;
			class CAxis;
			class CText;
			class CTextData;
			class CPlotSurface;
			class CSeries;
			class CDataPoint;
			class CDataLabels;
			class CDataLabel;
			class CDataLabelHidden;
			class CSeriesLayoutProperties;
			class CNumberFormat;
			class CDataLabelVisibilities;
			class CBinning;
			class CStatistics;
			class CSubtotals;
			class CSeriesElementVisibilities;
			class CCatScaling;
			class CValScaling;
			class CAxisUnit;
			class CTickMarks;
			class CGridlines;
			class CAxisUnitsLabel;
		}
	}
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}
	class CSizeAnchor;
	class CFromTo;
	class CExt;
}

namespace BinXlsxRW
{
	struct SaveParams
	{
		SaveParams(const std::wstring& _sDrawingsPath, const std::wstring& _sEmbeddingsPath, const std::wstring& _sThemePath, OOX::CContentTypes *pContentTypes, CSVWriter* pCSVWriter = NULL, bool bMacro = false);

		smart_ptr<PPTX::Theme>			pTheme;
		std::wstring					sThemePath;
		std::wstring					sDrawingsPath;
		std::wstring					sEmbeddingsPath;
		OOX::CContentTypes*				pContentTypes = NULL;
		CSVWriter*						pCSVWriter = NULL;
		bool							bMacroEnabled = false;
	};

	class BinaryChartReader : public Binary_CommonReader
	{
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		SaveParams& m_oSaveParams;
	public:
		BinaryChartReader (NSBinPptxRW::CBinaryFileReader& oBufferedStream, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		
		int ReadCT_ChartFile			(long length, OOX::Spreadsheet::CChartFile* poResult);
		int ReadCT_ChartExFile			(long length, OOX::Spreadsheet::CChartExFile* poResult);
	private:
		int ReadCT_Boolean				(BYTE type, long length, void* poResult);
		int ReadCT_Double				(BYTE type, long length, void* poResult);
		int ReadCT_UnsignedInt			(BYTE type, long length, void* poResult);
		int ReadCT_String				(BYTE type, long length, void* poResult);
		int ReadCT_UnsignedByte			(BYTE type, long length, void* poResult);
		int ReadCT_SignedByte			(BYTE type, long length, void* poResult);

		int ReadCT_ChartColors			(BYTE type, long length, void* poResult);
		int ReadCT_ColorsVariation		(BYTE type, long length, void* poResult);
		int ReadCT_ChartStyle			(BYTE type, long length, void* poResult);
		int ReadCT_StyleEntry			(BYTE type, long length, void* poResult);
		int ReadCT_StyleMarkerLayout	(BYTE type, long length, void* poResult);
		int ReadCT_ChartFileContent		(BYTE type, long length, void* poResult);
		int ReadCT_PageSetup			(BYTE type, long length, void* poResult);
		int ReadCT_PageMargins			(BYTE type, long length, void* poResult);
		int ReadCT_HeaderFooter			(BYTE type, long length, void* poResult);
		int ReadCT_PrintSettings		(BYTE type, long length, void* poResult);
		int ReadCT_ExternalData			(BYTE type, long length, void* poResult);
		int ReadCT_DispBlanksAs			(BYTE type, long length, void* poResult);
		int ReadCT_LegendEntry			(BYTE type, long length, void* poResult);
		int ReadCT_LegendPos			(BYTE type, long length, void* poResult);
		int ReadCT_Legend				(BYTE type, long length, void* poResult);
		int ReadCT_Layout				(BYTE type, long length, void* poResult);
		int ReadCT_ManualLayout			(BYTE type, long length, void* poResult);
		int ReadCT_LayoutTarget			(BYTE type, long length, void* poResult);
		int ReadCT_LayoutMode			(BYTE type, long length, void* poResult);
		int ReadCT_DTable				(BYTE type, long length, void* poResult);
		int ReadCT_SerAx				(BYTE type, long length, void* poResult);
		int ReadCT_Scaling				(BYTE type, long length, void* poResult);
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
		int ReadCT_TimeUnit				(BYTE type, long length, void* poResult);
		int ReadCT_DateAx				(BYTE type, long length, void* poResult);
		int ReadCT_LblAlgn				(BYTE type, long length, void* poResult);
		int ReadCT_CatAx				(BYTE type, long length, void* poResult);
		int ReadCT_DispUnitsLbl			(BYTE type, long length, void* poResult);
		int ReadCT_BuiltInUnit			(BYTE type, long length, void* poResult);
		int ReadCT_DispUnits			(BYTE type, long length, void* poResult);
		int ReadCT_CrossBetween			(BYTE type, long length, void* poResult);
		int ReadCT_ValAx				(BYTE type, long length, void* poResult);
		int ReadCT_SizeRepresents		(BYTE type, long length, void* poResult);
		int ReadCT_BubbleSer			(BYTE type, long length, void* poResult);
		int ReadCT_SerTx				(BYTE type, long length, void* poResult);
		int ReadCT_DPt					(BYTE type, long length, void* poResult);
		int ReadCT_Marker				(BYTE type, long length, void* poResult);
		int ReadCT_MarkerStyle			(BYTE type, long length, void* poResult);
		int ReadCT_PictureOptions		(BYTE type, long length, void* poResult);
		int ReadCT_PictureFormat		(BYTE type, long length, void* poResult);
		int ReadCT_DLbls				(BYTE type, long length, void* poResult);
		int ReadCT_DLbl					(BYTE type, long length, void* poResult);
		int ReadCT_DLblPos				(BYTE type, long length, void* poResult);
		int ReadCT_Trendline			(BYTE type, long length, void* poResult);
		int ReadCT_TrendlineType		(BYTE type, long length, void* poResult);
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
		int ReadCT_SplitType			(BYTE type, long length, void* poResult);
		int ReadCT_OfPieType			(BYTE type, long length, void* poResult);
		int ReadCT_custSplit			(BYTE type, long length, void* poResult);
		int ReadCT_OfPieChart			(BYTE type, long length, void* poResult);
		int ReadCT_PieSer				(BYTE type, long length, void* poResult);
		int ReadCT_Bar3DChart			(BYTE type, long length, void* poResult);
		int ReadCT_BarDir				(BYTE type, long length, void* poResult);
		int ReadCT_BarGrouping			(BYTE type, long length, void* poResult);
		int ReadCT_BarSer				(BYTE type, long length, void* poResult);
		int ReadCT_Shape				(BYTE type, long length, void* poResult);
		int ReadCT_BarChart				(BYTE type, long length, void* poResult);
		int ReadCT_DoughnutChart		(BYTE type, long length, void* poResult);
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
		int ReadCT_Surface				(BYTE type, long length, void* poResult);
		int ReadCT_View3D				(BYTE type, long length, void* poResult);
		int ReadCT_PivotFmt				(BYTE type, long length, void* poResult);
		int ReadCT_pivotFmts			(BYTE type, long length, void* poResult);
		int ReadCT_Chart				(BYTE type, long length, void* poResult);
		int ReadCT_Protection			(BYTE type, long length, void* poResult);
		int ReadCT_PivotSource			(BYTE type, long length, void* poResult);
		int ReadCT_Style				(BYTE type, long length, void* poResult);
		int ReadAlternateContent		(BYTE type, long length, void* poResult);
		int ReadAlternateContentChoice	(BYTE type, long length, void* poResult);
		int ReadAlternateContentFallback(BYTE type, long length, void* poResult);
		int ReadCT_PptxElement			(BYTE type, long length, void* poResult);

		int ReadCT_ChartExternalReference(BYTE type, long length, void* poResult);

		int ReadExtensions				(BYTE type, long length, void* poResult);

		int ReadCT_ChartFiltering		(BYTE type, long length, void* poResult);
		int ReadCT_filterExceptions		(BYTE type, long length, void* poResult);
		int ReadCT_filterException		(BYTE type, long length, void* poResult);
		int ReadCT_dataLabelsRange		(BYTE type, long length, void* poResult);

		int ReadCT_userShapes			(BYTE type, long length, void* poResult);
		int ReadCT_userShape			(BYTE type, long length, void* poResult);
		int ReadCT_FromTo				(BYTE type, long length, void* poResult);
		int ReadCT_Ext					(BYTE type, long length, void* poResult);
		
		int ReadCT_XlsxBin				(BYTE *pData, long length, NSCommon::smart_ptr<OOX::Media> & file);
		int ReadCT_XlsxZip				(BYTE* pData, long length, NSCommon::smart_ptr<OOX::Media>& file);

		int ReadCT_ChartExFileContent	(BYTE type, long length, void* poResult);
		int ReadCT_ChartExChart			(BYTE type, long length, void* poResult);
		int ReadCT_ChartExChartData		(BYTE type, long length, void* poResult);
		int ReadCT_ChartExData			(BYTE type, long length, void* poResult);
		int ReadCT_ChartExExternalData	(BYTE type, long length, void* poResult);
		int ReadCT_ChartExDataStrDimension	(BYTE type, long length, void* poResult);
		int ReadCT_ChartExDataNumDimension	(BYTE type, long length, void* poResult);
		int ReadCT_ChartExFormula (BYTE type, long length, void* poResult);
		int ReadCT_ChartExStringLevel (BYTE type, long length, void* poResult);
		int ReadCT_ChartExNumericLevel (BYTE type, long length, void* poResult);
		int ReadCT_ChartExStringValue (BYTE type, long length, void* poResult);
		int ReadCT_ChartExNumericValue (BYTE type, long length, void* poResult);
		int ReadCT_ChartExPlotArea(BYTE type, long length, void* poResult);
		int ReadCT_ChartExPlotAreaRegion(BYTE type, long length, void* poResult);
		int ReadCT_ChartExTitle(BYTE type, long length, void* poResult);
		int ReadCT_ChartExLegend(BYTE type, long length, void* poResult);
		int ReadCT_ChartExText(BYTE type, long length, void* poResult);
		int ReadCT_ChartExTextData(BYTE type, long length, void* poResult);
		int ReadCT_ChartExPlotSurface(BYTE type, long length, void* poResult);
		int ReadCT_ChartExSeries(BYTE type, long length, void* poResult);
		int ReadCT_ChartExDataPoint(BYTE type, long length, void* poResult);
		int ReadCT_ChartExDataLabels(BYTE type, long length, void* poResult);
		int ReadCT_ChartExDataLabel(BYTE type, long length, void* poResult);
		int ReadCT_ChartExDataLabelHidden(BYTE type, long length, void* poResult);
		int ReadCT_ChartExSeriesLayoutProperties(BYTE type, long length, void* poResult);
		int ReadCT_ChartExNumberFormat(BYTE type, long length, void* poResult);
		int ReadCT_ChartExDataLabelVisibilities(BYTE type, long length, void* poResult);
		int ReadCT_ChartExBinning(BYTE type, long length, void* poResult);
		int ReadCT_ChartExStatistics(BYTE type, long length, void* poResult);
		int ReadCT_ChartExSubtotals(BYTE type, long length, void* poResult);
		int ReadCT_ChartExSeriesElementVisibilities(BYTE type, long length, void* poResult);
		int ReadCT_ChartExAxis(BYTE type, long length, void* poResult);
		int ReadCT_ChartExCatScaling(BYTE type, long length, void* poResult);
		int ReadCT_ChartExValScaling(BYTE type, long length, void* poResult);
		int ReadCT_ChartExAxisUnit(BYTE type, long length, void* poResult);
		int ReadCT_ChartExTickMarks(BYTE type, long length, void* poResult);
		int ReadCT_ChartExGridlines(BYTE type, long length, void* poResult);
		int ReadCT_ChartExAxisUnitsLabel(BYTE type, long length, void* poResult);
	};
	class BinaryChartWriter
	{
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		BinaryCommonWriter				m_oBcw;
	public:
		BinaryChartWriter (NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		
		void WriteCT_ChartFile	(OOX::Spreadsheet::CChartFile& oChartFile);
		
		void WriteAlternateContent (OOX::Spreadsheet::AlternateContent& oVal);
		void WriteAlternateContentChoice (OOX::Spreadsheet::AlternateContentChoice& oVal);
		void WriteAlternateContentFallback (OOX::Spreadsheet::AlternateContentFallback& oVal);
//-------------------------------------------------------------------------------------------------------------
		void WriteCT_ChartStyle(OOX::Spreadsheet::CChartStyleFile & oVal);
//-------------------------------------------------------------------------------------------------------------
		void WriteCT_ChartColor(OOX::Spreadsheet::CChartColorsFile & oVal);
//-------------------------------------------------------------------------------------------------------------
		void WriteCT_ChartExFile(OOX::Spreadsheet::CChartExFile & oChartFile);
	private:
		void WriteCT_AxPos(OOX::Spreadsheet::CAxPos& oVal);
		void WriteCT_LegendPos(OOX::Spreadsheet::CLegendPos& oVal);
		void WriteCT_DispBlanksAs(OOX::Spreadsheet::CDispBlanksAs& oVal);
		void WriteCT_ErrDir(OOX::Spreadsheet::CErrDir& oVal);
		void WriteCT_ErrBarType(OOX::Spreadsheet::CErrBarType& oVal);
		void WriteCT_ErrValType(OOX::Spreadsheet::CErrValType& oVal);
		void WriteCT_LayoutTarget(OOX::Spreadsheet::CLayoutTarget& oVal);
		void WriteCT_LayoutMode(OOX::Spreadsheet::CLayoutMode& oVal);
		void WriteCT_Orientation(OOX::Spreadsheet::COrientation& oVal);
		void WriteCT_TickMark(OOX::Spreadsheet::CTickMark& oVal);
		void WriteCT_TickLblPos(OOX::Spreadsheet::CTickLblPos& oVal);
		void WriteCT_Crosses(OOX::Spreadsheet::CCrosses& oVal);
		void WriteCT_TimeUnit(OOX::Spreadsheet::CTimeUnit& oVal);
		void WriteCT_LblAlgn(OOX::Spreadsheet::CLblAlgn& oVal);
		void WriteCT_BuiltInUnit(OOX::Spreadsheet::CBuiltInUnit& oVal);
		void WriteCT_CrossBetween(OOX::Spreadsheet::CCrossBetween& oVal);
		void WriteCT_SizeRepresents(OOX::Spreadsheet::CSizeRepresents& oVal);
		void WriteCT_PictureFormat(OOX::Spreadsheet::CPictureFormat& oVal);
		void WriteCT_MarkerStyle(OOX::Spreadsheet::CMarkerStyle& oVal);
		void WriteCT_DLblPos(OOX::Spreadsheet::CDLblPos& oVal);
		void WriteCT_TrendlineType(OOX::Spreadsheet::CTrendlineType& oVal);
		void WriteCT_SplitType(OOX::Spreadsheet::CSplitType& oVal);
		void WriteCT_OfPieType(OOX::Spreadsheet::COfPieType& oVal);
		void WriteCT_BarDir(OOX::Spreadsheet::CBarDir& oVal);
		void WriteCT_BarGrouping(OOX::Spreadsheet::CBarGrouping& oVal);
		void WriteCT_Shape(OOX::Spreadsheet::CShapeType& oVal);
		void WriteCT_ScatterStyle(OOX::Spreadsheet::CScatterStyle& oVal);
		void WriteCT_RadarStyle(OOX::Spreadsheet::CRadarStyle& oVal);
		void WriteCT_Grouping(OOX::Spreadsheet::CGrouping& oVal);

		void WriteCT_SignedByte(char oVal);
		void WriteCT_UnsignedByte(unsigned char oVal);
		void WriteCT_UnsignedInt(unsigned int oVal);
		void WriteCT_Boolean(bool oVal);
		void WriteCT_Double(double oVal);
		void WriteCT_String(std::wstring oVal);

		void WriteCT_PageSetup(OOX::Spreadsheet::CT_PageSetup& oVal);
		void WriteCT_PageMargins(OOX::Spreadsheet::CT_PageMargins& oVal);
		void WriteCT_HeaderFooter(OOX::Spreadsheet::CT_HeaderFooter& oVal);
		void WriteCT_PrintSettings(OOX::Spreadsheet::CT_PrintSettings& oVal);
		void WriteCT_ExternalData(OOX::Spreadsheet::CT_ExternalData& oVal);
		void WriteCT_LegendEntry(OOX::Spreadsheet::CT_LegendEntry& oVal);
		void WriteCT_Legend(OOX::Spreadsheet::CT_Legend& oVal);
		void WriteCT_Layout(OOX::Spreadsheet::CT_Layout& oVal);
		void WriteCT_ManualLayout(OOX::Spreadsheet::CT_ManualLayout& oVal);

		void WriteCT_DTable(OOX::Spreadsheet::CT_DTable& oVal);
		void WriteCT_SerAx(OOX::Spreadsheet::CT_SerAx& oVal);
		void WriteCT_Scaling(OOX::Spreadsheet::CT_Scaling& oVal);
		void WriteCT_ChartLines(OOX::Spreadsheet::CT_ChartLines& oVal, bool bExt = false);
		void WriteCT_Title(OOX::Spreadsheet::CT_Title& oVal);
		void WriteCT_Tx(OOX::Spreadsheet::CT_Tx& oVal);
		void WriteCT_StrRef(OOX::Spreadsheet::CT_StrRef& oVal);
		void WriteCT_StrData(OOX::Spreadsheet::CT_StrData& oVal);
		void WriteCT_StrVal(OOX::Spreadsheet::CT_StrVal& oVal);
		void WriteCT_NumFmt(OOX::Spreadsheet::CT_NumFmt& oVal);
		void WriteCT_DateAx(OOX::Spreadsheet::CT_DateAx& oVal);
		void WriteCT_CatAx(OOX::Spreadsheet::CT_CatAx& oVal);
		void WriteCT_DispUnitsLbl(OOX::Spreadsheet::CT_DispUnitsLbl& oVal);
		void WriteCT_DispUnits(OOX::Spreadsheet::CT_DispUnits& oVal);
		void WriteCT_ValAx(OOX::Spreadsheet::CT_ValAx& oVal);
		void WriteCT_BubbleSer(OOX::Spreadsheet::CT_BubbleSer& oVal);
		void WriteCT_SerTx(OOX::Spreadsheet::CT_SerTx& oVal);
		void WriteCT_DPt(OOX::Spreadsheet::CT_DPt& oVal);
		void WriteCT_Marker(OOX::Spreadsheet::CT_Marker& oVal);
		void WriteCT_PictureOptions(OOX::Spreadsheet::CT_PictureOptions& oVal);
		void WriteCT_DLbls(OOX::Spreadsheet::CT_DLbls& oVal);
		void WriteCT_DLbl(OOX::Spreadsheet::CT_DLbl& oVal);
		void WriteCT_Trendline(OOX::Spreadsheet::CT_Trendline& oVal);
		void WriteCT_TrendlineLbl(OOX::Spreadsheet::CT_TrendlineLbl& oVal);
		void WriteCT_ErrBars(OOX::Spreadsheet::CT_ErrBars& oVal);
		void WriteCT_NumDataSource(OOX::Spreadsheet::CT_NumDataSource& oVal);
		void WriteCT_NumData(OOX::Spreadsheet::CT_NumData& oVal);
		void WriteCT_NumVal(OOX::Spreadsheet::CT_NumVal& oVal);
		void WriteCT_NumRef(OOX::Spreadsheet::CT_NumRef& oVal);
		void WriteCT_AxDataSource(OOX::Spreadsheet::CT_AxDataSource& oVal);
		void WriteCT_MultiLvlStrRef(OOX::Spreadsheet::CT_MultiLvlStrRef& oVal);
		void WriteCT_lvl(OOX::Spreadsheet::CT_lvl& oVal);
		void WriteCT_MultiLvlStrData(OOX::Spreadsheet::CT_MultiLvlStrData& oVal);
		void WriteCT_BubbleChart(OOX::Spreadsheet::CT_BubbleChart& oVal);
		void WriteCT_bandFmts(OOX::Spreadsheet::CT_bandFmts& oVal);
		void WriteCT_Surface3DChart(OOX::Spreadsheet::CT_Surface3DChart& oVal);
		void WriteCT_SurfaceSer(OOX::Spreadsheet::CT_SurfaceSer& oVal);
		void WriteCT_BandFmt(OOX::Spreadsheet::CT_BandFmt& oVal);
		void WriteCT_SurfaceChart(OOX::Spreadsheet::CT_SurfaceChart& oVal);
		void WriteCT_custSplit(OOX::Spreadsheet::CT_custSplit& oVal);
		void WriteCT_OfPieChart(OOX::Spreadsheet::CT_OfPieChart& oVal);
		void WriteCT_PieSer(OOX::Spreadsheet::CT_PieSer& oVal);
		void WriteCT_Bar3DChart(OOX::Spreadsheet::CT_Bar3DChart& oVal);
		void WriteCT_BarSer(OOX::Spreadsheet::CT_BarSer& oVal);
		void WriteCT_BarChart(OOX::Spreadsheet::CT_BarChart& oVal);
		void WriteCT_DoughnutChart(OOX::Spreadsheet::CT_DoughnutChart& oVal);
		void WriteCT_Pie3DChart(OOX::Spreadsheet::CT_Pie3DChart& oVal);
		void WriteCT_PieChart(OOX::Spreadsheet::CT_PieChart& oVal);
		void WriteCT_ScatterSer(OOX::Spreadsheet::CT_ScatterSer& oVal);
		void WriteCT_ScatterChart(OOX::Spreadsheet::CT_ScatterChart& oVal);
		void WriteCT_RadarSer(OOX::Spreadsheet::CT_RadarSer& oVal);
		void WriteCT_RadarChart(OOX::Spreadsheet::CT_RadarChart& oVal);
		void WriteCT_StockChart(OOX::Spreadsheet::CT_StockChart& oVal);
		void WriteCT_LineSer(OOX::Spreadsheet::CT_LineSer& oVal);
		void WriteCT_UpDownBars(OOX::Spreadsheet::CT_UpDownBars& oVal);
		void WriteCT_UpDownBar(OOX::Spreadsheet::CT_UpDownBar& oVal);
		void WriteCT_Line3DChart(OOX::Spreadsheet::CT_Line3DChart& oVal);
		void WriteCT_LineChart(OOX::Spreadsheet::CT_LineChart& oVal);
		void WriteCT_Area3DChart(OOX::Spreadsheet::CT_Area3DChart& oVal);
		void WriteCT_AreaSer(OOX::Spreadsheet::CT_AreaSer& oVal);
		void WriteCT_AreaChart(OOX::Spreadsheet::CT_AreaChart& oVal);
		void WriteCT_PlotArea(OOX::Spreadsheet::CT_PlotArea& oVal);
		void WriteCT_Surface(OOX::Spreadsheet::CT_Surface& oVal);
		void WriteCT_View3D(OOX::Spreadsheet::CT_View3D& oVal);
		void WriteCT_PivotFmt(OOX::Spreadsheet::CT_PivotFmt& oVal);
		void WriteCT_pivotFmts(OOX::Spreadsheet::CT_pivotFmts& oVal);
		void WriteCT_Chart(OOX::Spreadsheet::CT_Chart& oVal);
		void WriteCT_Protection(OOX::Spreadsheet::CT_Protection& oVal);
		void WriteCT_PivotSource(OOX::Spreadsheet::CT_PivotSource& oVal);
		void WriteCT_Style(OOX::Spreadsheet::CT_Style& oVal);
		void WriteCT_Shape (OOX::CSizeAnchor* pVal);
		void WriteCT_FromTo (OOX::CFromTo& oFromTo);
		void WriteCT_Ext (OOX::CExt& oExt);

		void WriteCT_Variation(OOX::Spreadsheet::ChartEx::CVariation & oVal);
		
		void WriteCT_StyleEntry(OOX::Spreadsheet::ChartEx::CStyleEntry & oVal);
		void WriteCT_MarkerLayout(OOX::Spreadsheet::ChartEx::CMarkerLayout & oVal);

		void WriteCT_ChartExChartData(OOX::Spreadsheet::ChartEx::CChartData *pVal);
		void WriteCT_ChartExChart(OOX::Spreadsheet::ChartEx::CChart *pVal);
		void WriteCT_ChartExData(OOX::Spreadsheet::ChartEx::CData * pVal);
		void WriteCT_ChartExExternalData(OOX::Spreadsheet::ChartEx::CExternalData *pVal);
		void WriteCT_ChartExDataDimension(OOX::Spreadsheet::ChartEx::CStrDimension *pVal);
		void WriteCT_ChartExDataDimension(OOX::Spreadsheet::ChartEx::CNumDimension *pVal);
		void WriteCT_ChartExFormula(OOX::Spreadsheet::ChartEx::CFormula *pVal);
		void WriteCT_ChartExStringLevel(OOX::Spreadsheet::ChartEx::CStringLevel *pVal);
		void WriteCT_ChartExNumericLevel(OOX::Spreadsheet::ChartEx::CNumericLevel *pVal);
		void WriteCT_ChartExStringValue(OOX::Spreadsheet::ChartEx::CStringValue *pVal);
		void WriteCT_ChartExNumericValue(OOX::Spreadsheet::ChartEx::CNumericValue *pVal);
		void WriteCT_ChartExPlotArea(OOX::Spreadsheet::ChartEx::CPlotArea*pVal);
		void WriteCT_ChartExTitle(OOX::Spreadsheet::ChartEx::CTitle *pVal);
		void WriteCT_ChartExLegend(OOX::Spreadsheet::ChartEx::CLegend *pVal);
		void WriteCT_ChartExPlotAreaRegion(OOX::Spreadsheet::ChartEx::CPlotAreaRegion *pVal);
		void WriteCT_ChartExAxis(OOX::Spreadsheet::ChartEx::CAxis *pVal);
		void WriteCT_ChartExText(OOX::Spreadsheet::ChartEx::CText *pVal);
		void WriteCT_ChartExTextData(OOX::Spreadsheet::ChartEx::CTextData *pVal);
		void WriteCT_ChartExPlotSurface(OOX::Spreadsheet::ChartEx::CPlotSurface *pVal);
		void WriteCT_ChartExSeries(OOX::Spreadsheet::ChartEx::CSeries *pVal);
		void WriteCT_ChartExDataPoint(OOX::Spreadsheet::ChartEx::CDataPoint *pVal);
		void WriteCT_ChartExDataLabels(OOX::Spreadsheet::ChartEx::CDataLabels *pVal);
		void WriteCT_ChartExDataLabel(OOX::Spreadsheet::ChartEx::CDataLabel *pVal);
		void WriteCT_ChartExDataLabelHidden(OOX::Spreadsheet::ChartEx::CDataLabelHidden *pVal);
		void WriteCT_ChartExSeriesLayoutProperties(OOX::Spreadsheet::ChartEx::CSeriesLayoutProperties *pVal);
		void WriteCT_ChartExNumberFormat(OOX::Spreadsheet::ChartEx::CNumberFormat *pVal);
		void WriteCT_ChartExDataLabelVisibilities(OOX::Spreadsheet::ChartEx::CDataLabelVisibilities *pVal);	
		void WriteCT_ChartExBinning(OOX::Spreadsheet::ChartEx::CBinning *pVal);
		void WriteCT_ChartExStatistics(OOX::Spreadsheet::ChartEx::CStatistics *pVal);
		void WriteCT_ChartExSubtotals(OOX::Spreadsheet::ChartEx::CSubtotals *pVal);
		void WriteCT_ChartExSeriesElementVisibilities(OOX::Spreadsheet::ChartEx::CSeriesElementVisibilities *pVal);
		void WriteCT_ChartExCatScaling(OOX::Spreadsheet::ChartEx::CCatScaling *pVal);
		void WriteCT_ChartExValScaling(OOX::Spreadsheet::ChartEx::CValScaling *pVal);
		void WriteCT_ChartExAxisUnit(OOX::Spreadsheet::ChartEx::CAxisUnit *pVal);
		void WriteCT_ChartExTickMarks(OOX::Spreadsheet::ChartEx::CTickMarks *pVal);
		void WriteCT_ChartExGridlines(OOX::Spreadsheet::ChartEx::CGridlines *pVal);
		void WriteCT_ChartExAxisUnitsLabel(OOX::Spreadsheet::ChartEx::CAxisUnitsLabel *pVal);
		
		void WriteExtensions(OOX::Drawing::COfficeArtExtensionList *pVal);

		void WriteChartFiltering(OOX::Spreadsheet::CSeriesFiltering *pVal);
		void WriteDataLabelsRange(OOX::Spreadsheet::CSeriesDataLabelsRange *pVal);

		void WriteFilteredSeriesTitle(OOX::Spreadsheet::CT_Tx *pVal);
		void WriteFilteredCategoryTitle(OOX::Spreadsheet::CT_AxDataSource *pVal);

		void WriteFilteredLineSeries(OOX::Spreadsheet::CT_LineSer *pVal);
		void WriteFilteredScatterSeries(OOX::Spreadsheet::CT_ScatterSer *pVal);
		void WriteFilteredRadarSeries(OOX::Spreadsheet::CT_RadarSer *pVal);
		void WriteFilteredBarSeries(OOX::Spreadsheet::CT_BarSer *pVal);
		void WriteFilteredAreaSeries(OOX::Spreadsheet::CT_AreaSer *pVal);
		void WriteFilteredBubbleSeries(OOX::Spreadsheet::CT_BubbleSer *pVal);
		void WriteFilteredSurfaceSeries(OOX::Spreadsheet::CT_SurfaceSer *pVal);
		void WriteFilteredPieSeries(OOX::Spreadsheet::CT_PieSer *pVal);

		void WriteFullRef(const std::wstring & ref);
		void WriteLevelRef(const std::wstring & ref);
		void WriteFormulaRef(const std::wstring & ref);

		void WriteCategoryFilterExceptions(OOX::Spreadsheet::CCategoryFilterExceptions *pVal);
		void WriteCategoryFilterException(OOX::Spreadsheet::CCategoryFilterException *pVal);

		void toBin(OOX::Spreadsheet::ItemsChoiceType5 eType, void* pVal);
		void toBin(OOX::Spreadsheet::ItemsChoiceType6 eType, void* pVal);
		void toBin(OOX::Spreadsheet::ItemsChoiceType4 eType, void* pVal);
		void toBin(OOX::Spreadsheet::ItemsChoiceType3 eType, void* pVal);
	};
}
