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
#include "ChartSerializer.h"

#include <algorithm>

#include "../../../XlsxFormat/Xlsx.h"
#include "../../../XlsxFormat/Workbook/Workbook.h"
#include "../../../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../../../XlsxFormat/Styles/Styles.h"
#include "../../../XlsxFormat/Worksheets/Worksheet.h"

#define NUMID_START 160
const wchar_t* gc_Cat = L"cat";
const wchar_t* gc_Val = L"val";
const wchar_t* gc_XVal = L"xVal";
const wchar_t* gc_YVal = L"yVal";
const wchar_t* gc_BubbleSize = L"bubbleSize";

#define PARSE_CT_CHART(chart, type) \
{\
	for(size_t i = 0; i < chart->m_ser.size();++i)\
	{\
		type* ser = chart->m_ser[i];\
		PARSE_TITLE(ser, true, NULL);\
		parseAxDataSource(ser->m_cat, true, gc_Cat);\
		parseNumDataSource(ser->m_val, true, gc_Val);\
	}\
}
#define PARSE_CT_CHART_SCATTER(chart, type) \
{\
	for(size_t i = 0; i < chart->m_ser.size();++i)\
	{\
		type* ser = chart->m_ser[i];\
		PARSE_TITLE(ser, true, NULL);\
		parseAxDataSource(ser->m_xVal, true, gc_XVal);\
		parseNumDataSource(ser->m_yVal, true, gc_YVal);\
	}\
}
#define PARSE_TRENDLINE(chart, type) \
{\
	for(size_t i = 0; i < chart->m_ser.size();++i)\
	{\
		type* ser = chart->m_ser[i];\
		for(size_t j = 0; j < ser->m_trendline.size(); ++j)\
		{\
			OOX::Spreadsheet::CT_Trendline* pTrendline = ser->m_trendline[j];\
			if(NULL != pTrendline->m_trendlineLbl)\
			{\
				PARSE_TITLE(pTrendline->m_trendlineLbl, false, NULL);\
			}\
		}\
	}\
}
#define PARSE_ERRBARS(chart, type) \
{\
	for(size_t i = 0; i < chart->m_ser.size();++i)\
	{\
		type* ser = chart->m_ser[i];\
		for(size_t j = 0; j < ser->m_errBars.size(); ++j)\
		{\
			OOX::Spreadsheet::CT_ErrBars* pErrBars = ser->m_errBars[j];\
			parseNumDataSource(pErrBars->m_minus, false, NULL);\
			parseNumDataSource(pErrBars->m_plus, false, NULL);\
		}\
	}\
}
#define PARSE_BUBBLE_SIZE(chart, type) \
{\
	for(size_t i = 0; i < chart->m_ser.size();++i)\
	{\
		type* ser = chart->m_ser[i];\
		parseNumDataSource(ser->m_bubbleSize, true, gc_BubbleSize);\
	}\
}
#define PARSE_TITLE(pTitle, bUpdateRange, cRangeName)\
{\
	if(NULL != pTitle && NULL != pTitle->m_tx)\
	{\
		parseStrRef(pTitle->m_tx->m_strRef, bUpdateRange, cRangeName);\
	}\
}

namespace BinXlsxRW{

	ChartWriter::ChartWriter()
	{
		OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();
		pXfs->m_oBorderId.Init();
		pXfs->m_oBorderId->SetValue(0);
		pXfs->m_oFillId.Init();
		pXfs->m_oFillId->SetValue(0);
		pXfs->m_oFontId.Init();
		pXfs->m_oFontId->SetValue(0);
		pXfs->m_oNumFmtId.Init();
		pXfs->m_oNumFmtId->SetValue(0);
		m_aXfs.push_back(pXfs);

		m_nRow1 = -1;
		m_nCol1 = -1;
		m_nRow2 = -1;
		m_nCol2 = -1;
	}
	ChartWriter::~ChartWriter()
	{
        for (std::map<std::wstring, std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>*>::iterator it = m_mapSheets.begin(); it != m_mapSheets.end(); ++it)
		{
            std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>* rows = it->second;

            for(std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>::iterator itRow = rows->begin(); itRow != rows->end(); itRow++)
			{
				delete itRow->second;
			}
			delete it->second;
		}
	}
	void ChartWriter::toXlsx(OOX::Spreadsheet::CXlsx& oXlsx)
	{
		std::vector<std::wstring> aSharedStrings;
		
		oXlsx.CreateWorkbook();
		oXlsx.m_pWorkbook->m_oSheets.Init();

		int nSheetId = 1;
		OOX::Spreadsheet::CWorksheet* pFirstWorksheet = NULL;

        for (std::map<std::wstring, std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>*>::iterator it = m_mapSheets.begin(); it != m_mapSheets.end(); ++it)
		{
			const std::wstring& sSheetName = it->first;
			
			//find or generate black worksheet
			OOX::Spreadsheet::CWorksheet* pWorksheet = toXlsxGetSheet(oXlsx.m_arWorksheets, oXlsx.m_mapWorksheets, sSheetName);
			//fill data to worksheet
			toXlsxSheetdata(pWorksheet, *it->second, aSharedStrings);
			
			OOX::Spreadsheet::CSheet* pSheet = new OOX::Spreadsheet::CSheet();
			pSheet->m_oName = sSheetName;
			pSheet->m_oSheetId.Init();
			pSheet->m_oSheetId->SetValue(nSheetId++);
			
			smart_ptr<OOX::File> oWorksheetFile = smart_ptr<OOX::File>(pWorksheet);
			const OOX::RId oRId = oXlsx.m_pWorkbook->Add(oWorksheetFile);
			pSheet->m_oRid.Init();
			pSheet->m_oRid->SetValue(oRId.get());
			
			oXlsx.m_pWorkbook->m_oSheets->m_arrItems.push_back(pSheet);

			if(NULL == pFirstWorksheet)
			{
				pFirstWorksheet = pWorksheet;
			}
		}

		//SharedStrings
		oXlsx.CreateSharedStrings();
		for(size_t i = 0; i < aSharedStrings.size(); ++i)
		{
			OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
			pText->m_sText = aSharedStrings[i];
			if(std::wstring::npos != pText->m_sText.find(_T(" ")))
			{
				pText->m_oSpace.Init();
				pText->m_oSpace->SetValue(SimpleTypes::xmlspacePreserve);
			}
			OOX::Spreadsheet::CSi* pSi = new OOX::Spreadsheet::CSi();
			pSi->m_arrItems.push_back(pText);
			oXlsx.m_pSharedStrings->AddSi(pSi);
		}
		oXlsx.m_pSharedStrings->m_oCount.Init();
		oXlsx.m_pSharedStrings->m_oCount->SetValue(oXlsx.m_pSharedStrings->m_nCount);
		oXlsx.m_pSharedStrings->m_oUniqueCount.Init();
		oXlsx.m_pSharedStrings->m_oUniqueCount->SetValue(oXlsx.m_pSharedStrings->m_nCount);
		//Styles
		oXlsx.CreateStyles();
		oXlsx.m_pStyles->m_oCellXfs.Init();
		for(size_t i = 0; i < m_aXfs.size(); ++i)
		{
			oXlsx.m_pStyles->m_oCellXfs->m_arrItems.push_back(m_aXfs[i]);
		}
		oXlsx.m_pStyles->m_oNumFmts.Init();

        for (boost::unordered_map<std::wstring, unsigned int>::iterator it = m_mapFormats.begin(); it != m_mapFormats.end(); ++it)
		{
			OOX::Spreadsheet::CNumFmt* pNumFmt = new OOX::Spreadsheet::CNumFmt();
			pNumFmt->m_oFormatCode = (it->first);
			pNumFmt->m_oNumFmtId.Init();
			pNumFmt->m_oNumFmtId->SetValue(NUMID_START + it->second);
			oXlsx.m_pStyles->m_oNumFmts->m_arrItems.push_back(pNumFmt);
		}
		oXlsx.m_pStyles->m_oNumFmts->m_oCount.Init();
		oXlsx.m_pStyles->m_oNumFmts->m_oCount->SetValue(oXlsx.m_pStyles->m_oNumFmts->m_arrItems.size());
		oXlsx.m_pStyles->m_oCellXfs->m_oCount.Init();
		oXlsx.m_pStyles->m_oCellXfs->m_oCount->SetValue(oXlsx.m_pStyles->m_oCellXfs->m_arrItems.size());
		
		OOX::Spreadsheet::CDxf* pDxf = new OOX::Spreadsheet::CDxf();
		pDxf->m_oBorder.Init();
		pDxf->m_oBorder->m_oStart.Init();
		pDxf->m_oBorder->m_oStart->m_oStyle.Init();
		pDxf->m_oBorder->m_oStart->m_oStyle->SetValue(SimpleTypes::Spreadsheet::borderstyleThin);
		pDxf->m_oBorder->m_oStart->m_oColor.Init();
		pDxf->m_oBorder->m_oStart->m_oColor->m_oIndexed.Init();
		pDxf->m_oBorder->m_oStart->m_oColor->m_oIndexed->SetValue(12);
		pDxf->m_oBorder->m_oEnd.Init();
		pDxf->m_oBorder->m_oEnd->m_oStyle.Init();
		pDxf->m_oBorder->m_oEnd->m_oStyle->SetValue(SimpleTypes::Spreadsheet::borderstyleThin);
		pDxf->m_oBorder->m_oEnd->m_oColor.Init();
		pDxf->m_oBorder->m_oEnd->m_oColor->m_oIndexed.Init();
		pDxf->m_oBorder->m_oEnd->m_oColor->m_oIndexed->SetValue(12);
		pDxf->m_oBorder->m_oTop.Init();
		pDxf->m_oBorder->m_oTop->m_oStyle.Init();
		pDxf->m_oBorder->m_oTop->m_oStyle->SetValue(SimpleTypes::Spreadsheet::borderstyleThin);
		pDxf->m_oBorder->m_oTop->m_oColor.Init();
		pDxf->m_oBorder->m_oTop->m_oColor->m_oIndexed.Init();
		pDxf->m_oBorder->m_oTop->m_oColor->m_oIndexed->SetValue(12);
		pDxf->m_oBorder->m_oBottom.Init();
		pDxf->m_oBorder->m_oBottom->m_oStyle.Init();
		pDxf->m_oBorder->m_oBottom->m_oStyle->SetValue(SimpleTypes::Spreadsheet::borderstyleThin);
		pDxf->m_oBorder->m_oBottom->m_oColor.Init();
		pDxf->m_oBorder->m_oBottom->m_oColor->m_oIndexed.Init();
		pDxf->m_oBorder->m_oBottom->m_oColor->m_oIndexed->SetValue(12);
		
		oXlsx.m_pStyles->m_oDxfs.Init();
		oXlsx.m_pStyles->m_oDxfs->m_arrItems.push_back(pDxf);
		oXlsx.m_pStyles->m_oDxfs->m_oCount.Init();
		oXlsx.m_pStyles->m_oDxfs->m_oCount->SetValue(oXlsx.m_pStyles->m_oDxfs->m_arrItems.size());
		//Table
		//todo table в случае нескольких sheet или если серии разнесены по sheet
		if(m_aTableNames.size() > 0)
		{
			OOX::Spreadsheet::CTableFile* pTable = new OOX::Spreadsheet::CTableFile(NULL);
			pTable->m_oTable.Init();
			pTable->m_oTable->m_oDisplayName = L"Table1";
			pTable->m_oTable->m_oRef.Init();
			pTable->m_oTable->m_oRef->SetValue(OOX::Spreadsheet::CCell::combineRef(m_nRow1 - 1, m_nCol1 - 1) + L":" + OOX::Spreadsheet::CCell::combineRef(m_nRow2 - 1, m_nCol2 - 1));
			pTable->m_oTable->m_oTotalsRowCount.Init();
			pTable->m_oTable->m_oTotalsRowCount->SetValue(0);
			pTable->m_oTable->m_oTableBorderDxfId.Init();
			pTable->m_oTable->m_oTableBorderDxfId->SetValue(0);
			pTable->m_oTable->m_oTableStyleInfo.Init();
			pTable->m_oTable->m_oTableStyleInfo->m_oShowFirstColumn.Init();
			pTable->m_oTable->m_oTableStyleInfo->m_oShowFirstColumn->FromBool(false);
			pTable->m_oTable->m_oTableStyleInfo->m_oShowLastColumn.Init();
			pTable->m_oTable->m_oTableStyleInfo->m_oShowLastColumn->FromBool(false);
			pTable->m_oTable->m_oTableStyleInfo->m_oShowRowStripes.Init();
			pTable->m_oTable->m_oTableStyleInfo->m_oShowRowStripes->FromBool(true);
			pTable->m_oTable->m_oTableStyleInfo->m_oShowColumnStripes.Init();
			pTable->m_oTable->m_oTableStyleInfo->m_oShowColumnStripes->FromBool(false);
			pTable->m_oTable->m_oTableColumns.Init();
			
			for(size_t i = 0; i < m_aTableNames.size(); ++i)
			{
				OOX::Spreadsheet::CTableColumn* pTableColumn = new OOX::Spreadsheet::CTableColumn();

				if (!pTableColumn->m_oId.IsInit())
				{
					pTableColumn->m_oId.Init();
					pTableColumn->m_oId->SetValue((unsigned int)i + 1);
				}
				if (!pTableColumn->m_oName.IsInit())
				{
					pTableColumn->m_oName = m_aTableNames[i];
				}
				pTable->m_oTable->m_oTableColumns->m_arrItems.push_back(pTableColumn);
			}
			pTable->m_oTable->m_oTableColumns->m_oCount.Init();
			pTable->m_oTable->m_oTableColumns->m_oCount->SetValue(pTable->m_oTable->m_oTableColumns->m_arrItems.size());

			//check unique names
			size_t szTableNames = m_aTableNames.size();
			std::sort( m_aTableNames.begin(), m_aTableNames.end() );
			m_aTableNames.erase( std::unique( m_aTableNames.begin(), m_aTableNames.end() ), m_aTableNames.end() );
			size_t szTableNamesUnique = m_aTableNames.size();

			//add to sheet
			if(szTableNames == szTableNamesUnique)
			{
				OOX::Spreadsheet::CTablePart* pTablePart = new OOX::Spreadsheet::CTablePart();
				NSCommon::smart_ptr<OOX::File> pTableFile(pTable);
				
				const OOX::RId oRId = pFirstWorksheet->Add(pTableFile);
				pTablePart->m_oRId.Init();
				pTablePart->m_oRId->SetValue(oRId.get());
				
				pFirstWorksheet->m_oTableParts.Init();
				pFirstWorksheet->m_oTableParts->m_arrItems.push_back(pTablePart);
				pFirstWorksheet->m_oTableParts->m_oCount.Init();
				pFirstWorksheet->m_oTableParts->m_oCount->SetValue(pFirstWorksheet->m_oTableParts->m_arrItems.size());
			}
		}
	}
	void ChartWriter::parseChart(const OOX::Spreadsheet::CT_Chart* pChart)
	{
		if(NULL != pChart)
		{
			PARSE_TITLE(pChart->m_title, false, NULL);
			OOX::Spreadsheet::CT_PlotArea* pPlotArea = pChart->m_plotArea;
			if(NULL != pPlotArea)
			{
				for(size_t i = 0; i < pPlotArea->m_ItemsElementName1.size(); ++i)
				{
					OOX::Spreadsheet::ItemsChoiceType6 eType = *pPlotArea->m_ItemsElementName1[i];
					void* pItem = pPlotArea->m_Items1[i];
					switch(eType)
					{
					case OOX::Spreadsheet::itemschoicetype6CATAX:
					{
						OOX::Spreadsheet::CT_CatAx* pCatax = static_cast<OOX::Spreadsheet::CT_CatAx*>(pItem);
						PARSE_TITLE(pCatax->m_title, false, NULL);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype6DATEAX:
					{
						OOX::Spreadsheet::CT_DateAx* pDateax = static_cast<OOX::Spreadsheet::CT_DateAx*>(pItem);
						PARSE_TITLE(pDateax->m_title, false, NULL);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype6SERAX:
					{
						OOX::Spreadsheet::CT_SerAx* pSerax = static_cast<OOX::Spreadsheet::CT_SerAx*>(pItem);
						PARSE_TITLE(pSerax->m_title, false, NULL);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype6VALAX:
					{
						OOX::Spreadsheet::CT_ValAx* pValax = static_cast<OOX::Spreadsheet::CT_ValAx*>(pItem);
						PARSE_TITLE(pValax->m_title, false, NULL);
						if(NULL != pValax->m_dispUnits)
						{
							PARSE_TITLE(pValax->m_dispUnits->m_dispUnitsLbl, false, NULL);
						}
						break;
					}
					}
				}
				for(size_t i = 0; i < pPlotArea->m_ItemsElementName0.size(); ++i)
				{
					OOX::Spreadsheet::ItemsChoiceType5 eType = *pPlotArea->m_ItemsElementName0[i];
					void* pItem = pPlotArea->m_Items[i];
					switch(eType)
					{
					case OOX::Spreadsheet::itemschoicetype5AREA3DCHART:
					{
						OOX::Spreadsheet::CT_Area3DChart* pChart = static_cast<OOX::Spreadsheet::CT_Area3DChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_AreaSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5AREACHART:
					{
						OOX::Spreadsheet::CT_AreaChart* pChart = static_cast<OOX::Spreadsheet::CT_AreaChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_AreaSer);
						PARSE_TRENDLINE(pChart, OOX::Spreadsheet::CT_AreaSer);
						PARSE_ERRBARS(pChart, OOX::Spreadsheet::CT_AreaSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5BAR3DCHART:
					{
						OOX::Spreadsheet::CT_Bar3DChart* pChart = static_cast<OOX::Spreadsheet::CT_Bar3DChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_BarSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5BARCHART:
					{
						OOX::Spreadsheet::CT_BarChart* pChart = static_cast<OOX::Spreadsheet::CT_BarChart*>(pItem);
						PARSE_TRENDLINE(pChart, OOX::Spreadsheet::CT_BarSer);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_BarSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5BUBBLECHART:
					{
						OOX::Spreadsheet::CT_BubbleChart* pChart = static_cast<OOX::Spreadsheet::CT_BubbleChart*>(pItem);
						PARSE_CT_CHART_SCATTER(pChart, OOX::Spreadsheet::CT_BubbleSer);
						PARSE_TRENDLINE(pChart, OOX::Spreadsheet::CT_BubbleSer);
						PARSE_ERRBARS(pChart, OOX::Spreadsheet::CT_BubbleSer);
						PARSE_BUBBLE_SIZE(pChart, OOX::Spreadsheet::CT_BubbleSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5DOUGHNUTCHART:
					{
						OOX::Spreadsheet::CT_DoughnutChart* pChart = static_cast<OOX::Spreadsheet::CT_DoughnutChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_PieSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5LINE3DCHART:
					{
						OOX::Spreadsheet::CT_Line3DChart* pChart = static_cast<OOX::Spreadsheet::CT_Line3DChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_LineSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5LINECHART:
					{
						OOX::Spreadsheet::CT_LineChart* pChart = static_cast<OOX::Spreadsheet::CT_LineChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_LineSer);
						PARSE_TRENDLINE(pChart, OOX::Spreadsheet::CT_LineSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5OFPIECHART:
					{
						OOX::Spreadsheet::CT_OfPieChart* pChart = static_cast<OOX::Spreadsheet::CT_OfPieChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_PieSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5PIE3DCHART:
					{
						OOX::Spreadsheet::CT_Pie3DChart* pChart = static_cast<OOX::Spreadsheet::CT_Pie3DChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_PieSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5PIECHART:
					{
						OOX::Spreadsheet::CT_PieChart* pChart = static_cast<OOX::Spreadsheet::CT_PieChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_PieSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5RADARCHART:
					{
						OOX::Spreadsheet::CT_RadarChart* pChart = static_cast<OOX::Spreadsheet::CT_RadarChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_RadarSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5SCATTERCHART:
					{
						OOX::Spreadsheet::CT_ScatterChart* pChart = static_cast<OOX::Spreadsheet::CT_ScatterChart*>(pItem);
						PARSE_CT_CHART_SCATTER(pChart, OOX::Spreadsheet::CT_ScatterSer);
						PARSE_TRENDLINE(pChart, OOX::Spreadsheet::CT_ScatterSer);
						PARSE_ERRBARS(pChart, OOX::Spreadsheet::CT_ScatterSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5STOCKCHART:
					{
						OOX::Spreadsheet::CT_StockChart* pChart = static_cast<OOX::Spreadsheet::CT_StockChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_LineSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5SURFACE3DCHART:
					{
						OOX::Spreadsheet::CT_Surface3DChart* pChart = static_cast<OOX::Spreadsheet::CT_Surface3DChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_SurfaceSer);
						break;
					}
					case OOX::Spreadsheet::itemschoicetype5SURFACECHART:
					{
						OOX::Spreadsheet::CT_SurfaceChart* pChart = static_cast<OOX::Spreadsheet::CT_SurfaceChart*>(pItem);
						PARSE_CT_CHART(pChart, OOX::Spreadsheet::CT_SurfaceSer);
						break;
					}
					}
				}
			}
		}
		//проверяем можем ли создать таблицу
		if(m_mapSheets.size() > 0 && m_nRow1 > 0 && m_nRow2 > 0 && m_nCol1 > 0 && m_nCol2 > 0 && m_nRow1 < m_nRow2)
		{
            std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>*            rows = m_mapSheets.begin()->second;
            std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>::iterator   itRows = rows->find(m_nRow1);

            if(itRows != rows->end())
			{
                std::map<int, OOX::Spreadsheet::CCell*>* cells = itRows->second;
				std::vector<int> aIndexesCell;

                for(std::map<int, OOX::Spreadsheet::CCell*>::const_iterator it = cells->begin(); it != cells->end(); ++it)
				{
					aIndexesCell.push_back(it->first);
				}

                std::sort(aIndexesCell.begin(), aIndexesCell.end());

                if(m_nCol2 - m_nCol1 + 1 == aIndexesCell.size() && m_nCol1 == aIndexesCell[0] && m_nCol2 == aIndexesCell[aIndexesCell.size() - 1])
				{
					for(size_t j = 0; j < aIndexesCell.size(); ++j)
					{
						int nIndexCell = aIndexesCell[j];
						OOX::Spreadsheet::CCell* pCell = cells->at(nIndexCell);
						//не должно быть ячеек без m_oValue
						m_aTableNames.push_back(pCell->m_oValue->m_sText);
					}
				}
			}
		}
	}
	OOX::Spreadsheet::CWorksheet* ChartWriter::toXlsxGetSheet(std::vector<OOX::Spreadsheet::CWorksheet*>& arWorksheets, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets, const std::wstring& sName)
	{
		OOX::Spreadsheet::CWorksheet* pWorksheet = NULL;
		std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>::const_iterator pFind = mapWorksheets.find(sName);

        if (pFind == mapWorksheets.end())
		{
			pWorksheet = new OOX::Spreadsheet::CWorksheet(NULL);
			pWorksheet->m_oSheetFormatPr.Init();
			pWorksheet->m_oSheetFormatPr->m_oDefaultRowHeight = 15.;
			pWorksheet->m_oPageMargins.Init();
			pWorksheet->m_oPageMargins.Init();
			pWorksheet->m_oPageMargins->m_oLeft.Init();
			pWorksheet->m_oPageMargins->m_oLeft->FromInches(0.7);
			pWorksheet->m_oPageMargins->m_oRight.Init();
			pWorksheet->m_oPageMargins->m_oRight->FromInches(0.7);
			pWorksheet->m_oPageMargins->m_oTop.Init();
			pWorksheet->m_oPageMargins->m_oTop->FromInches(0.75);
			pWorksheet->m_oPageMargins->m_oBottom.Init();
			pWorksheet->m_oPageMargins->m_oBottom->FromInches(0.75);
			pWorksheet->m_oPageMargins->m_oHeader.Init();
			pWorksheet->m_oPageMargins->m_oHeader->FromInches(0.3);
			pWorksheet->m_oPageMargins->m_oFooter.Init();
			pWorksheet->m_oPageMargins->m_oFooter->FromInches(0.3);

			mapWorksheets[sName] = pWorksheet;
			arWorksheets.push_back(pWorksheet);
		}
		else
		{
			pWorksheet = pFind->second;
		}
		return pWorksheet;
	}
    void ChartWriter::toXlsxSheetdata(OOX::Spreadsheet::CWorksheet* pWorksheet, const std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>& rows, std::vector<std::wstring>& aSharedStrings)
	{
		pWorksheet->m_oSheetData.Init();
		std::vector<int> aIndexesRow;

        for(std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>::const_iterator it = rows.begin(); it != rows.end(); ++it)
		{
			aIndexesRow.push_back(it->first);
		}

        std::sort(aIndexesRow.begin(), aIndexesRow.end());

        for(size_t i = 0; i < aIndexesRow.size(); ++i)
		{
			int nIndexRow = aIndexesRow[i];

            OOX::Spreadsheet::CRow* pRow = new OOX::Spreadsheet::CRow();
			pRow->m_oR.Init();
			pRow->m_oR->SetValue(nIndexRow);

            const std::map<int, OOX::Spreadsheet::CCell*>& cells = *rows.at(nIndexRow);
			std::vector<int> aIndexesCell;

            for(std::map<int, OOX::Spreadsheet::CCell*>::const_iterator it = cells.begin(); it != cells.end(); ++it)
			{
				aIndexesCell.push_back(it->first);
			}
			std::sort(aIndexesCell.begin(), aIndexesCell.end());

            for(size_t j = 0; j < aIndexesCell.size(); ++j)
			{
				int nIndexCell = aIndexesCell[j];

                OOX::Spreadsheet::CCell* pCell = cells.at(nIndexCell);
				//SharedStrings
				if(pCell->m_oValue.IsInit())
				{
					if (pCell->m_oType.IsInit())
					{
						if (pCell->m_oType->GetValue() == SimpleTypes::Spreadsheet::celltypeSharedString)
						{
							aSharedStrings.push_back(pCell->m_oValue->m_sText);
							pCell->m_oValue->m_sText = std::to_wstring(aSharedStrings.size() - 1);
						}
					}
					else
					{
						pCell->m_oType.Init();
						const std::wstring& val = pCell->m_oValue->m_sText;
						if(L"TRUE" == val || L"FALSE" == val)
						{
							pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeBool);
							if(L"TRUE" == val)
							{
								pCell->m_oValue->m_sText = L"1";
							}
							else
							{
								pCell->m_oValue->m_sText = L"0";
							}
						}
						else if(L"#NULL!" == val || L"#DIV/0!" == val || L"#VALUE!" == val || L"#REF!" == val || L"#NAME?" == val || L"#NUM!" == val || L"#N/A" == val)
						{
							pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeError);
							pCell->m_oValue->m_sText = val;
						}
						else
						{
							//как в CsvReader - подозрительный код
							WCHAR *pEndPtr;
							wcstod(val.c_str(), &pEndPtr);
							if (0 != *pEndPtr)
							{
								// Не число
								aSharedStrings.push_back(val);
								pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
								pCell->m_oValue->m_sText = std::to_wstring((int)aSharedStrings.size() - 1);
							}
							else
							{
								pCell->m_oValue->m_sText = val;
							}
						}
					}
				}
				pRow->m_arrItems.push_back(pCell);
			}
			pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
		}
	}
	void ChartWriter::parseCell(const std::wstring& sheet, const int& nRow, const int& nCol, const std::wstring& val, std::wstring* format = NULL, bool bAlwaysSharedString)
	{
        std::map<std::wstring, std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>*>::const_iterator itSheets = m_mapSheets.find(sheet);
        std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>* rows = NULL;

        if(itSheets == m_mapSheets.end())
		{
            rows = new std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>();
			m_mapSheets.insert(std::make_pair(sheet, rows));
		}
		else
		{
			rows = itSheets->second;
		}
        std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>::const_iterator itRows = rows->find(nRow);
        std::map<int, OOX::Spreadsheet::CCell*>* cells = NULL;

        if(itRows == rows->end())
		{
            cells = new std::map<int, OOX::Spreadsheet::CCell*>();
			rows->insert(std::make_pair(nRow, cells));
		}
		else
		{
			cells = itRows->second;
		}
		if(cells->find(nCol) == cells->end())
		{
			OOX::Spreadsheet::CCell* pNewCell = parseCreateCell(nRow, nCol, val, format);

			if (bAlwaysSharedString)
			{
				pNewCell->m_oType.Init();
				pNewCell->m_oType->SetValue(SimpleTypes::Spreadsheet::ECellTypeType::celltypeSharedString);
			}
			cells->insert(std::make_pair(nCol, pNewCell));
		}
	}
	OOX::Spreadsheet::CCell* ChartWriter::parseCreateCell(const int& nRow, const int& nCol, const std::wstring& val, std::wstring* format = NULL)
	{
		OOX::Spreadsheet::CCell* pNewCell = new OOX::Spreadsheet::CCell();
		//пока добавляем как есть, shared string после записи таблицы
		pNewCell->m_oValue.Init();
		pNewCell->m_oValue->m_sText = val;

        if(NULL != format)
		{
			unsigned int nXfsIndex = m_aXfs.size();

            boost::unordered_map<std::wstring, unsigned int>::const_iterator itFormat = m_mapFormats.find(*format);
			if(itFormat == m_mapFormats.end())
			{
				m_mapFormats[*format] = nXfsIndex;
			}
			else
			{
				nXfsIndex = itFormat->second;
			}

			OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();
			pXfs->m_oBorderId.Init();
			pXfs->m_oBorderId->SetValue(0);
			pXfs->m_oFillId.Init();
			pXfs->m_oFillId->SetValue(0);
			pXfs->m_oFontId.Init();
			pXfs->m_oFontId->SetValue(0);
			pXfs->m_oNumFmtId.Init();
			pXfs->m_oNumFmtId->SetValue(NUMID_START + nXfsIndex);
			m_aXfs.push_back(pXfs);
			
			pNewCell->m_oStyle = nXfsIndex;
		}
		pNewCell->setRowCol(nRow - 1, nCol - 1);
		return pNewCell;
	}
	void ChartWriter::parseStrRef(const OOX::Spreadsheet::CT_StrRef* pStrRef, bool bUpdateRange, const wchar_t* cRangeName)
	{
		if(NULL != pStrRef && NULL != pStrRef->m_f)
		{
			std::wstring wb, sheetFrom, sheetTo;
			int nRow1, nCol1, nRow2, nCol2;
			if(OOX::Spreadsheet::CCell::parse3DRef(*pStrRef->m_f, wb, sheetFrom, sheetTo, nRow1, nCol1, nRow2, nCol2) &&
					sheetFrom.length() > 0 && 0 == sheetTo.length() && NULL != pStrRef->m_strCache)
			{
				bool bRow = nRow1 == nRow2;
				if(bUpdateRange)
				{
					if(NULL != cRangeName)
					{
						if(bRow)
						{
							if(nCol1 > 1)
							{
								parseCell(sheetFrom, nRow1, nCol1 - 1, std::wstring(cRangeName), NULL, true);
							}
						}
						else
						{
							if(nRow1 > 1)
							{
								parseCell(sheetFrom, nRow1 - 1, nCol1, std::wstring(cRangeName), NULL, true);
							}
						}
					}
					if(-1 == m_nRow1 || m_nRow1 > nRow1)
					{
						m_nRow1 = nRow1;
					}
					if(-1 == m_nCol1 || m_nCol1 > nCol1)
					{
						m_nCol1 = nCol1;
					}
					if(-1 == m_nRow2 || m_nRow2 < nRow2)
					{
						m_nRow2 = nRow2;
					}
					if(-1 == m_nCol2 || m_nCol2 < nCol2)
					{
						m_nCol2 = nCol2;
					}
				}
				for(size_t i = 0; i < pStrRef->m_strCache->m_pt.size(); ++i)
				{
					OOX::Spreadsheet::CT_StrVal* val = pStrRef->m_strCache->m_pt[i];
					if(NULL != val->m_idx && NULL != val->m_v)
					{
						int nRow, nCol;
						if(bRow)
						{
							nRow = nRow1;
							nCol = nCol1 + *val->m_idx;
						}
						else
						{
							nRow = nRow1 + *val->m_idx;
							nCol = nCol1;
						}
						parseCell(sheetFrom, nRow, nCol, *val->m_v, NULL, true);
					}
				}
			}
		}
	}
	void ChartWriter::parseNumRef(const OOX::Spreadsheet::CT_NumRef* pNumRef, bool bUpdateRange, const wchar_t* cRangeName)
	{
		if(NULL != pNumRef && NULL != pNumRef->m_f)
		{
			std::wstring wb, sheetFrom, sheetTo;
			int nRow1, nCol1, nRow2, nCol2;
			if(OOX::Spreadsheet::CCell::parse3DRef(*pNumRef->m_f, wb, sheetFrom, sheetTo, nRow1, nCol1, nRow2, nCol2) &&
					sheetFrom.length() > 0 && 0 == sheetTo.length() && NULL != pNumRef->m_numCache)
			{
				bool bRow = nRow1 == nRow2;
				if(bUpdateRange)
				{
					if(NULL != cRangeName)
					{
						if(bRow)
						{
							if(nCol1 > 1)
							{
								parseCell(sheetFrom, nRow1, nCol1 - 1, std::wstring(cRangeName), NULL);
							}
						}
						else
						{
							if(nRow1 > 1)
							{
								parseCell(sheetFrom, nRow1 - 1, nCol1, std::wstring(cRangeName), NULL);
							}
						}
					}
					if(-1 == m_nRow1 || m_nRow1 > nRow1)
					{
						m_nRow1 = nRow1;
					}
					if(-1 == m_nCol1 || m_nCol1 > nCol1)
					{
						m_nCol1 = nCol1;
					}
					if(-1 == m_nRow2 || m_nRow2 < nRow2)
					{
						m_nRow2 = nRow2;
					}
					if(-1 == m_nCol2 || m_nCol2 < nCol2)
					{
						m_nCol2 = nCol2;
					}
				}
				std::wstring* formatCodeSer = pNumRef->m_numCache->m_formatCode;
				for(size_t i = 0; i < pNumRef->m_numCache->m_pt.size(); ++i)
				{
					OOX::Spreadsheet::CT_NumVal* val = pNumRef->m_numCache->m_pt[i];
					if(NULL != val->m_idx && NULL != val->m_v)
					{
						std::wstring* formatCode = NULL != val->m_formatCode ? val->m_formatCode : formatCodeSer;
						int nRow, nCol;
						if(bRow)
						{
							nRow = nRow1;
							nCol = nCol1 + *val->m_idx;
						}
						else
						{
							nRow = nRow1 + *val->m_idx;
							nCol = nCol1;
						}
						parseCell(sheetFrom, nRow, nCol, *val->m_v, formatCode);
					}
				}
			}
            else
            {
                //range is defined name
            }
		}
	}
	void ChartWriter::parseMultiLvlStrRef(const OOX::Spreadsheet::CT_MultiLvlStrRef* pMultiLvlStrRef, bool bUpdateRange, const wchar_t* cRangeName)
	{
		//todo нужен пример
	}
	void ChartWriter::parseAxDataSource(const OOX::Spreadsheet::CT_AxDataSource* pAxDataSource, bool bUpdateRange, const wchar_t* cRangeName)
	{
		if(NULL != pAxDataSource)
		{
			parseStrRef(pAxDataSource->m_strRef, bUpdateRange, cRangeName);
			parseNumRef(pAxDataSource->m_numRef, bUpdateRange, cRangeName);
			parseMultiLvlStrRef(pAxDataSource->m_multiLvlStrRef, bUpdateRange, cRangeName);
		}
	}
	void ChartWriter::parseNumDataSource(const OOX::Spreadsheet::CT_NumDataSource* pNumDataSource, bool bUpdateRange, const wchar_t* cRangeName)
	{
		if(NULL != pNumDataSource)
		{
			parseNumRef(pNumDataSource->m_numRef, bUpdateRange, cRangeName);
		}
	}
};
