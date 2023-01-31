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

#include "ChartSheetSubstream.h"

#include "Biff_records/WriteProtect.h"
#include "Biff_records/SheetExt.h"
#include "Biff_records/WebPub.h"
#include "Biff_records/HFPicture.h"
#include "Biff_records/PrintSize.h"
#include "Biff_records/Fbi.h"
#include "Biff_records/Fbi2.h"
#include "Biff_records/ClrtClient.h"
#include "Biff_records/Palette.h"
#include "Biff_records/SXViewLink.h"
#include "Biff_records/PivotChartBits.h"
#include "Biff_records/SBaseRef.h"
#include "Biff_records/MsoDrawingGroup.h"
#include "Biff_records/Units.h"
#include "Biff_records/CodeName.h"
#include "Biff_records/EOF.h"
#include "Biff_records/BOF.h"
#include "Biff_records/AreaFormat.h"
#include "Biff_records/SerToCrt.h"
#include "Biff_records/AxisParent.h"
#include "Biff_records/Series.h"
#include "Biff_records/BRAI.h"
#include "Biff_records/SIIndex.h"
#include "Biff_records/DataFormat.h"
#include "Biff_records/Text.h"
#include "Biff_records/Pos.h"
#include "Biff_records/Pie.h"
#include "Biff_records/ShtProps.h"
#include "Biff_records/Chart3d.h"
#include "Biff_records/ChartFormat.h"
#include "Biff_records/Legend.h"
#include "Biff_records/AttachedLabel.h"
#include "Biff_records/DataLabExtContents.h"
#include "Biff_records/CrtLine.h"
#include "Biff_records/Dat.h"
#include "Biff_records/Chart.h"
#include "Biff_records/ExternSheet.h"
#include "Biff_records/FrtFontList.h"
#include "Biff_records/ChartFrtInfo.h"
#include "Biff_records/SerParent.h"

#include "Biff_unions/FONTLIST.h"
#include "Biff_unions/PAGESETUP.h"
#include "Biff_unions/BACKGROUND.h"
#include "Biff_unions/PROTECTION_COMMON.h"
#include "Biff_unions/OBJECTS.h"
#include "Biff_unions/CHARTFOMATS.h"
#include "Biff_unions/SERIESDATA.h"
#include "Biff_unions/WINDOW.h"
#include "Biff_unions/CUSTOMVIEW.h"
#include "Biff_unions/CRTMLFRT.h"
#include "Biff_unions/FRAME.h"
#include "Biff_unions/ATTACHEDLABEL.h"
#include "Biff_unions/SERIESFORMAT.h"
#include "Biff_unions/CRT.h"
#include "Biff_unions/AXISPARENT.h"
#include "Biff_unions/AXES.h"
#include "Biff_unions/SS.h"
#include "Biff_unions/AI.h"
#include "Biff_unions/LD.h"
#include "Biff_unions/DAT.h"
#include "Biff_unions/PIVOTVIEW.h"
#include "Biff_unions/RECORD12.h"
#include "Biff_unions/ATTACHEDLABEL.h"
#include "Biff_unions/DFTTEXT.h"

#include "../../Converter/XlsConverter.h"
#include "../../Converter/xlsx_conversion_context.h"


namespace XLS
{;


ChartSheetSubstream::ChartSheetSubstream(const size_t ws_index) : CommonSubstream(ws_index)
{
}


ChartSheetSubstream::~ChartSheetSubstream()
{
}


BaseObjectPtr ChartSheetSubstream::clone()
{
	return BaseObjectPtr(new ChartSheetSubstream(*this));
}


/*
CHARTSHEET = BOF CHARTSHEETCONTENT

CHARTSHEETCONTENT = [WriteProtect] [SheetExt] [WebPub] *HFPicture PAGESETUP PrintSize [HeaderFooter]
					[BACKGROUND] *Fbi *Fbi2 [ClrtClient] [PROTECTION] [Palette] [SXViewLink] 
					[PivotChartBits] [SBaseRef] [MsoDrawingGroup] OBJECTS Units CHARTFOMATS SERIESDATA 
					*WINDOW *CUSTOMVIEW [CodeName] [CRTMLFRT] EOF
*/
const bool ChartSheetSubstream::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();
	global_info_->current_sheet = ws_index_ + 1; 
	
	int count = 0 ;

	if(!proc.mandatory<BOF>())
	{
		return false;
    }
	
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		//Log::warning(CFRecordType::getStringById(type));

		if (type == rt_NONE || proc.isBOF(type)) break;
		if (type == rt_EOF) 
		{
			proc.mandatory<EOF_T>();
			break;
		}

		switch(type)
		{
			case rt_WriteProtect:		
			{
				if (proc.optional<WriteProtect>())
				{
					m_WriteProtect = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_SheetExt:			
			{
				if (proc.optional<SheetExt>())
				{
					m_SheetExt = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_WebPub:				proc.optional<WebPub>();		break;
			case rt_HFPicture:
			{
				count = proc.repeated<HFPicture>(0, 0);	
				while(count > 0)
				{
					m_arHFPicture.insert(m_arHFPicture.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;		
			case rt_Header:
			case rt_Footer:		
			case rt_BottomMargin:
			case rt_TopMargin:
			case rt_LeftMargin:
			case rt_RightMargin:
			{
				if (proc.mandatory<PAGESETUP>())
				{
					m_PAGESETUP = elements_.back();
					elements_.pop_back();
				}
			}break;			
			case rt_PrintSize:
			{
				if (proc.mandatory<PrintSize>())
				{
					m_PrintSize = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_HeaderFooter:		
			{
				count = proc.repeated<RECORD12>	(0, 0);		
				while(count > 0)
				{
					m_arRECORD12.insert(m_arRECORD12.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;		
			case rt_BkHim:
			{
				if (proc.optional<BACKGROUND>())
				{
					m_BACKGROUND = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_Fbi:
			{
				count = proc.repeated<Fbi>(0, 0);
				while(count > 0)
				{
					m_arFbi.insert(m_arFbi.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_Fbi2:
			{
				count = proc.repeated<Fbi2>(0, 0);
				while(count > 0 && m_arFbi.empty())//??? разделить
				{
					m_arFbi.insert(m_arFbi.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}

			case rt_ClrtClient:			proc.optional<ClrtClient>();		break;
			
			case rt_Protect:	
			case rt_ScenarioProtect:
			case rt_ObjProtect:
			case rt_Password:										
			{
				if (proc.optional<PROTECTION_COMMON>())
				{
					m_PROTECTION = elements_.back();
					elements_.pop_back();
				}
			}break;			
			case rt_Palette:
			{
				if (proc.optional<Palette>())
				{
					m_Palette = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_SXViewLink:
			{
				if (proc.optional<SXViewLink>())
				{
					m_SXViewLink = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_PivotChartBits:		
			{
				if (proc.optional<PivotChartBits>())
				{
					m_PivotChartBits = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_SBaseRef:
			{				
				if (proc.optional<SBaseRef>())
				{
					m_SBaseRef = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_Obj:
			case rt_MsoDrawing:
			{
				MsoDrawingGroup mso_drawing_group(true);
				proc.optional(mso_drawing_group);

				OBJECTS objects(true);
				if (proc.mandatory(objects))
				{
					m_OBJECTS = elements_.back(); 
					elements_.pop_back();
				}
			}break;
			
			case rt_ExternSheet:
			{
				if (proc.optional<ExternSheet>())
				{
					m_ExternSheet = elements_.back(); 
					elements_.pop_back();
				}
			}break;
			case rt_Units:
			{
				if (proc.mandatory<Units>())
				{
					m_Units = elements_.back(); 
					elements_.pop_back();
				}	
			}break;
			
			case rt_Chart:
			{
				if ( proc.mandatory<CHARTFORMATS>() )
				{
					m_CHARTFORMATS = elements_.back();
					elements_.pop_back();

					recalc((CHARTFORMATS*)m_CHARTFORMATS.get());
				}
			}break;
			case rt_Dimensions:
			case rt_SIIndex:
			{
				if ( proc.mandatory<SERIESDATA>() )
				{
					m_SERIESDATA = elements_.back();
					elements_.pop_back();
					
					recalc((SERIESDATA*)m_SERIESDATA.get());
				}
			}break;

			case rt_Window2:
			{
				count = proc.repeated<WINDOW>(0, 0);
				while(count > 0)
				{
					m_arWINDOW.insert(m_arWINDOW.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;

			case rt_UserSViewBegin:
			{
				count = proc.repeated<CUSTOMVIEW>(0, 0);
				while(count > 0)
				{
					m_arCUSTOMVIEW.insert(m_arCUSTOMVIEW.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			
			case rt_CodeName:
			{
				if (proc.optional<CodeName>())
				{
					m_CodeName = elements_.back(); 
					elements_.pop_back();
				}
			}break;
			case rt_CrtMlFrt:
			{
				if (proc.optional<CRTMLFRT>())
				{
					m_CRTMLFRT = elements_.back(); 
					elements_.pop_back();
				}
			}break;
			case rt_FrtFontList: //error in stream !!
			{
				count = proc.repeated<FONTLIST>(0, 0);
				while(count > 0)
				{
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_ChartFrtInfo: 
			{
				//skipped record 
				count = proc.repeated<ChartFrtInfo>(0, 0);
			}break;
			default://unknown .... skip					
			{
				proc.SkipRecord();	
			}break;
		}
	}
	LoadHFPicture();

	return true;
}

void ChartSheetSubstream::recalc(CHARTFORMATS* charts)
{
	if (charts == NULL) return;
	if (charts->m_arAXISPARENT.empty()) return;

	int ind_AXIS = 0;
	AXISPARENT* parent0 = dynamic_cast<AXISPARENT*>(charts->m_arAXISPARENT[ind_AXIS].get());

	int iCrt = -1;
	for (size_t i = 0; i < parent0->m_arCRT.size(); i++)
	{
		CRT* crt= dynamic_cast<CRT*>(parent0->m_arCRT[i].get());
		if (crt)
		{
			parent0->m_mapCRTIndex.insert(std::make_pair(crt->m_indexCrt, i));
		}
	}
	for (size_t i = 0 ; i < charts->m_arSERIESFORMAT.size(); i++)
	{
		SERIESFORMAT * series = dynamic_cast<SERIESFORMAT *>(charts->m_arSERIESFORMAT[i].get());
		if (series == NULL) continue;

		SerParent *parent = dynamic_cast<SerParent*>(series->m_SerParent.get());
		if ( parent )
		{
			SERIESFORMAT *series_owner = dynamic_cast<SERIESFORMAT *>(charts->m_arSERIESFORMAT[parent->series - 1].get());
			if (series_owner)
			{
				series_owner->m_arSERIESFORMAT_ext.push_back(charts->m_arSERIESFORMAT[i]);	
				charts->m_arSERIESFORMAT[i] = BaseObjectPtr();
			}
		}
	}
	for (size_t i = 0 ; i < charts->m_arSERIESFORMAT.size(); i++)
	{
		SERIESFORMAT * series = dynamic_cast<SERIESFORMAT *>(charts->m_arSERIESFORMAT[i].get());
		if (series == NULL) continue;

		SerToCrt * serCrt = dynamic_cast<SerToCrt *>(series->m_SerToCrt.get());
		if ( serCrt == NULL) continue;

		iCrt = serCrt->id;

		while ((parent0->m_arCRT.size() <= iCrt) && (ind_AXIS < charts->m_arAXISPARENT.size()) && (charts->m_arAXISPARENT.size() > 1))
		{
			parent0 = dynamic_cast<AXISPARENT*>(charts->m_arAXISPARENT[++ind_AXIS].get());

		}

		if (iCrt >= parent0->m_arCRT.size() || iCrt < 0)
			continue;

		CRT * crt = dynamic_cast<CRT*>(parent0->m_arCRT[iCrt].get());
		
		std::unordered_map<int,std::vector<int>>::iterator it = m_mapTypeChart.find(iCrt);
		if (it == m_mapTypeChart.end())
		{
			std::vector<int> ser;
			ser.push_back(i);
			m_mapTypeChart.insert(std::make_pair(iCrt, ser));
		}
		else
		{
			it->second.push_back(i);
		}
	}
	for (size_t i = 0; i < parent0->m_arCRT.size(); i++)
	{
		std::unordered_map<int,std::vector<int>>::iterator it = m_mapTypeChart.find(i);
		if (it == m_mapTypeChart.end())
		{
			std::vector<int> ser;
			m_mapTypeChart.insert(std::make_pair(i, ser));
		}
	}
}

void ChartSheetSubstream::recalc(SERIESDATA* data)
{
}

int ChartSheetSubstream::serialize(std::wostream & _stream)
{
	CHARTFORMATS	*chart_formats		= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	if (!chart_formats) return 0;

	AreaFormat		*chart_area_format	= NULL;
	FRAME			*chart_frame		= dynamic_cast<FRAME*>(chart_formats->m_FRAME.get());
	if (chart_frame) chart_area_format	= dynamic_cast<AreaFormat*>(chart_frame->m_AreaFormat.get());

	ShtProps		*sht_props			= dynamic_cast<ShtProps*>(chart_formats->m_ShtProps.get());
	Chart			*chart_rect			= dynamic_cast<Chart*>(chart_formats->m_ChartRect.get());
	
	DFTTEXT *default_text = (false == chart_formats->m_arDFTTEXT.empty()) ? dynamic_cast<DFTTEXT*>(chart_formats->m_arDFTTEXT[0].get()) : NULL; 

	std::wstring namePivotTable;
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:lang")
		{
			CP_XML_ATTR(L"val",L"en-US");
		}

		CP_XML_NODE(L"c:roundedCorners") 
		{
			if ((chart_area_format) && (chart_area_format->fInvertNeg))	CP_XML_ATTR(L"val", 1); //????
			else														CP_XML_ATTR(L"val", 0); 
		}
		if (m_SXViewLink)
		{
			SXViewLink *link = dynamic_cast<SXViewLink*>(m_SXViewLink.get());

			CP_XML_NODE(L"c:pivotSource")
			{
				CP_XML_NODE(L"c:name")
				{
					std::wstring name = link->stPivotTable.value();
					
					std::wstring::size_type pos = name.find(L"]");
					std::wstring::size_type pos1 = name.find(L"!", pos);
					
					if (std::wstring::npos != pos1)
						namePivotTable = name.substr(pos1 + 1);

					if (std::wstring::npos != pos)
						name = L"[]" + name.substr(pos + 1);

					CP_XML_STREAM() << name;
				}
				CP_XML_NODE(L"c:fmtId")
				{
					CP_XML_ATTR(L"val", 0); 
				}
			}
		}
		CP_XML_NODE(L"c:chart")
		{
			if (false == namePivotTable.empty())
			{
				std::map<std::wstring, BaseObjectPtr>::iterator pFindTable = global_info_->mapPivotTableViews.find(namePivotTable);
				if (pFindTable != global_info_->mapPivotTableViews.end())
				{
				}
				//CP_XML_NODE(L"c:pivotFmts")
				//{
				//	CP_XML_NODE(L"c:pivotFmt")
				//	{
				//		CP_XML_NODE(L"c:idx")
				//		{
				//			CP_XML_ATTR(L"val", 0); 
				//			//<c:marker>
				//		}
				//	}
				//}
			
			}
			serialize_title		(CP_XML_STREAM());		
			serialize_3D		(CP_XML_STREAM());
			serialize_plot_area (CP_XML_STREAM());

			if (sht_props)
			{
				if (sht_props->fPlotVisOnly == true)
				{
					CP_XML_NODE(L"c:plotVisOnly") {	CP_XML_ATTR(L"val", sht_props->fPlotVisOnly); }
				}	
				CP_XML_NODE(L"c:dispBlanksAs")
				{
					switch(sht_props->mdBlank)
					{
					case 0:	CP_XML_ATTR(L"val", L"gap");	break;
					case 1:	CP_XML_ATTR(L"val", L"zero");	break;
					case 2:	CP_XML_ATTR(L"val", L"span");	break;
					}
				}
			}
		}
		if (chart_frame)
			chart_frame->serialize(_stream);
		else
		{
			CP_XML_NODE(L"c:spPr")
			{
				CP_XML_NODE(L"a:noFill");
				CP_XML_NODE(L"a:ln")
				{
					CP_XML_NODE(L"a:noFill");
				}
			}
		}
		if ((default_text) && (default_text->m_ATTACHEDLABEL))
		{
			ATTACHEDLABEL* attach_lablel = dynamic_cast<ATTACHEDLABEL*>(default_text->m_ATTACHEDLABEL.get());
			
			attach_lablel->serialize_txPr(_stream);
		}
		if (m_SXViewLink)
		{
			CP_XML_NODE(L"c:extLst")
			{
				CP_XML_NODE(L"c:ext")
				{
					CP_XML_ATTR(L"uri", L"{781A3756-C4B2-4CAC-9D66-4F8BD8637D16}");
					CP_XML_ATTR(L"xmlns:c14", L"http://schemas.microsoft.com/office/drawing/2007/8/2/chart");
					CP_XML_NODE(L"c14:pivotOptions")
					{
						CP_XML_NODE(L"c14:dropZoneFilter")
						{
							CP_XML_ATTR(L"val", 1);
						}		
						CP_XML_NODE(L"c14:dropZoneCategories")
						{
							CP_XML_ATTR(L"val", 1);
						}
						CP_XML_NODE(L"c14:dropZoneData")
						{
							CP_XML_ATTR(L"val", 1);
						}
						CP_XML_NODE(L"c14:dropZoneSeries")
						{
							CP_XML_ATTR(L"val", 1);
						}
						CP_XML_NODE(L"c14:dropZonesVisible")
						{
							CP_XML_ATTR(L"val", 1);
						}
					}
				}
			}
		}
	}

	if (chart_rect)
	{
		global_info_->xls_converter->xlsx_context->get_drawing_context().set_absolute_anchor(
			0, 0, chart_rect->dx.dVal, chart_rect->dy.dVal);
		
	}
	return 0;
}
int ChartSheetSubstream::serialize_3D (std::wostream & _stream)
{
	CHARTFORMATS	*chart_formats		= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	if (!chart_formats) return 0;

	Chart3d *chart3D = NULL;

	BaseObjectPtr	wallSpPr; 
	BaseObjectPtr	floorSpPr;

	for (size_t i = 0; i < chart_formats->m_arAXISPARENT.size(); i++)
	{
		AXISPARENT* parent		= dynamic_cast<AXISPARENT*>	(chart_formats->m_arAXISPARENT[i].get());
		AxisParent* ax_parent	= dynamic_cast<AxisParent*>	(parent->m_AxisParent.get());

		if (ax_parent->iax == 0) //primary axes
		{
			for (size_t i = 0 ; i < parent->m_arCRT.size() ; i++)
			{
				CRT* crt = dynamic_cast<CRT*>(parent->m_arCRT[i].get());
				if((crt) && (crt->m_Chart3d))
				{
					chart3D = dynamic_cast<Chart3d*>(crt->m_Chart3d.get());
					break;
				}
			}

			AXES * axes = dynamic_cast<AXES*> (parent->m_AXES.get());
			if (axes)
			{
				wallSpPr	= axes->m_Wall_FRAME;
				floorSpPr	= axes->m_Floor_FRAME;
			}
		}
	}
	if (!chart3D) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:view3D")
		{
			if (chart3D->anElev != 0)
			{
				CP_XML_NODE(L"c:rotX")
				{
					CP_XML_ATTR (L"val" , chart3D->anElev);
				}
			}
			if (chart3D->pcHeight3D != 100 && chart3D->pcHeight3D >= 5 && chart3D->pcHeight3D <= 500)
			{
				CP_XML_NODE(L"c:hPercent")
				{
					CP_XML_ATTR (L"val" , chart3D->pcHeight3D);
				}
			}
			if (chart3D->anRot != 0)
			{
				CP_XML_NODE(L"c:rotY")
				{
					CP_XML_ATTR (L"val" , chart3D->anRot);
				}
			}
			CP_XML_NODE(L"c:depthPercent")
			{
				CP_XML_ATTR (L"val" , chart3D->pcDepth);
			}
			if (chart3D->fPerspective == false)
			{
				CP_XML_NODE(L"c:rAngAx") {  CP_XML_ATTR (L"val" , L"1"); }
			}
			else
			{
				CP_XML_NODE(L"c:rAngAx") 
				{  
					CP_XML_ATTR (L"val" , L"0"); 
				}
				CP_XML_NODE(L"c:perspective") 
				{
					CP_XML_ATTR (L"val" , chart3D->pcDist); 
				}
			}
		}
		
		CP_XML_NODE(L"c:floor")
		{
			if (floorSpPr)
			{
				CP_XML_NODE(L"c:spPr")
				{
					floorSpPr->serialize(CP_XML_STREAM());
				}
			}
		}
		CP_XML_NODE(L"c:sideWall")
		{
			if (wallSpPr)
			{
				CP_XML_NODE(L"c:spPr")
				{
					wallSpPr->serialize(CP_XML_STREAM());
				}
			}
		}
		CP_XML_NODE(L"c:backWall")
		{
			if (wallSpPr)
			{
				CP_XML_NODE(L"c:spPr")
				{
					wallSpPr->serialize(CP_XML_STREAM());
				}
			}
		}
	}
	return 0;
}
int ChartSheetSubstream::serialize_title (std::wostream & _stream)
{
	CHARTFORMATS	*chart_formats		= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	if (!chart_formats) return 0;

    BaseObjectPtr   attached_label	= chart_formats->find_label(1, 0xffff);
    //BaseObjectPtr   dft_text		= chart_formats->find_default_text(2);

	ATTACHEDLABEL * title_label = dynamic_cast<ATTACHEDLABEL *>(attached_label.get());
	if (title_label == NULL) return 0;	
	
	AI* title_text = dynamic_cast<AI *>(title_label->m_AI.get());
	
	if (title_text == NULL) return 0;			
	if (title_text->empty()) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:title")
		{
			attached_label->serialize(CP_XML_STREAM()); 
		}
		if (!title_text->m_SeriesText)
		{
			CP_XML_NODE(L"c:autoTitleDeleted")
			{
				CP_XML_ATTR (L"val" , 0); 
			}
		}
	}
	return 0;
}
int ChartSheetSubstream::serialize_legend (std::wostream & _stream, const std::wstring & legend_entries)
{
	CHARTFORMATS	*chart_formats		= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	if (!chart_formats) return 0;

	AXISPARENT* parent0 = dynamic_cast<AXISPARENT*>(chart_formats->m_arAXISPARENT[0].get());
	if (parent0 == NULL) return 0;

	if (parent0->m_arCRT.size() < 1) return 0;

	//DAT* DAT_ = dynamic_cast<DAT*>(chart_formats->m_DAT.get());
	//Dat *dat_legend = NULL;
	//LD	*dat_ld		= NULL;
	//if (DAT_)
	//{
	//	dat_legend	= dynamic_cast<Dat*>(DAT_->m_Dat.get());
	//	dat_ld		= dynamic_cast<LD*>(DAT_->m_LD.get());
	//}
	
	//todooo разобраться с разными типами в одном чарте .. считать количество серий?? 
	std::unordered_map< int, std::vector<int>>::iterator it = m_mapTypeChart.begin();
	while (it != m_mapTypeChart.end())
	{
		CRT * crt = dynamic_cast<CRT*>(parent0->m_arCRT[it->first].get());

		LD * ld = dynamic_cast<LD*>(crt->m_LD.get());
		if (ld == NULL)
		{
			++it;
			continue;
		}

		CP_XML_WRITER(_stream)    
		{
			CP_XML_NODE(L"c:legend")
			{
				ld->m_countSeries = it->second.size();
				ld->serialize(CP_XML_STREAM(), legend_entries);				
			}
		}
		break;
	}
	return 0;
}
int ChartSheetSubstream::serialize_plot_area (std::wostream & _stream)
{
	CHARTFORMATS	*chart_formats		= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	if (!chart_formats) return 0;

	if (chart_formats->m_arAXISPARENT.empty())
		return 0;

	AXISPARENT* parent0 = dynamic_cast<AXISPARENT*>(chart_formats->m_arAXISPARENT[0].get());
	if (parent0 == NULL) return 0;

	FRAME	*PlotAreaFRAME	= NULL;
	Pos		*PlotAreaPos	= NULL;

	AxisParent* ax_parent	= dynamic_cast<AxisParent*>	(parent0->m_AxisParent.get());
	AXES*		axes		= dynamic_cast<AXES*>		(parent0->m_AXES.get());

	if ((ax_parent->iax == 0) && axes) //primary axes
	{
		PlotAreaFRAME	= dynamic_cast<FRAME*>	(axes->m_PlotArea_FRAME.get());
		PlotAreaPos		= dynamic_cast<Pos*>	(parent0->m_Pos.get()); //outer plot area

		if (PlotAreaPos && !parent0->m_arCRT.empty())
		{
			CRT * crt = dynamic_cast<CRT*>(parent0->m_arCRT[0].get());
			if ((crt) && (	crt->m_iChartType == CHART_TYPE_Radar || 
							crt->m_iChartType == CHART_TYPE_RadarArea ||
							crt->m_iChartType == CHART_TYPE_Scatter))//еще?
			{
				PlotAreaPos->m_iLayoutTarget = 2; //inner
			}
		}
		
		if (PlotAreaPos && PlotAreaFRAME)
		{
			//FRAME *FRAME_ = dynamic_cast<FRAME*>	(chart_formats->m_FRAME.get());
			
			PlotAreaPos->m_Frame = PlotAreaFRAME->m_Frame;//FRAME_->m_Frame;
		}
	}

	ShtProps *sht_props = dynamic_cast<ShtProps*>(chart_formats->m_ShtProps.get());
	
	std::wstringstream stream_legend_entries;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:plotArea")
		{
			if ((sht_props) && (sht_props->fAlwaysAutoPlotArea != false))
			{
				if (chart_formats->m_CrtLayout12A)
				{
					chart_formats->m_CrtLayout12A->serialize(CP_XML_STREAM());
				}
				else if (PlotAreaPos)
				{
					PlotAreaPos->serialize(CP_XML_STREAM());
				}
			}

			int series_order = 0;

			for (std::unordered_map<int, std::vector<int>>::iterator it = m_mapTypeChart.begin(); it != m_mapTypeChart.end(); ++it)
			{
				std::map<int, int>::iterator pFindIndexCrt = parent0->m_mapCRTIndex.find(it->first);
				
				if (pFindIndexCrt == parent0->m_mapCRTIndex.end())
					continue;

				CRT * crt = dynamic_cast<CRT*>(parent0->m_arCRT[pFindIndexCrt->second].get());

				ChartFormat	*format	= dynamic_cast<ChartFormat*>(crt->m_ChartFormat.get());
				AXISPARENT	*parent	= dynamic_cast<AXISPARENT*>(chart_formats->m_arAXISPARENT[crt->m_indAXISPARENT].get());

				CP_XML_NODE(crt->getOoxChartType())
				{	
					crt->m_ChartType->serialize_attribute( CP_GET_XML_NODE());
					
					if (crt->m_iChartType == CHART_TYPE_Scatter)
					{
						serialize_scatter_style(CP_XML_STREAM(), crt);
					}
					else if (crt->m_iChartType != CHART_TYPE_Stock)
					{
						crt->m_ChartType->serialize(CP_XML_STREAM());
					}
					
					format->serialize(CP_XML_STREAM());
					for (size_t i = 0 ; i < it->second.size(); i++)
					{
						SERIESFORMAT * series = dynamic_cast<SERIESFORMAT *>(chart_formats->m_arSERIESFORMAT[it->second[i]].get());
						if (series == NULL)	continue;

						SS * series_ss = dynamic_cast<SS *>(series->m_SS.get());
						if (series_ss == NULL) continue;

						series_ss->apply_crt_ss(crt->m_SS);						
						series_ss->m_is3D = crt->m_bIs3D;

						if (crt->m_iChartType == CHART_TYPE_Pie			|| 
							crt->m_iChartType == CHART_TYPE_BopPop		||
							crt->m_iChartType == CHART_TYPE_Doughnut	|| 
							crt->m_iChartType == CHART_TYPE_Surf )
								series_ss->m_isVaried	= format->fVaried;

						DataFormat * series_data_format = dynamic_cast<DataFormat *>(series_ss->m_DataFormat.get());
						int series_id = series_data_format->iss;

						Series * ser = dynamic_cast<Series *>(series->m_Series.get());
					
						CP_XML_NODE(L"c:ser")
						{				
							CP_XML_NODE(L"c:idx")	{ CP_XML_ATTR (L"val" , series_id); }
							CP_XML_NODE(L"c:order")	{ CP_XML_ATTR (L"val" , series_order++); }
							
							series->m_arAI[0]->serialize(CP_XML_STREAM());
							
							series_ss->serialize(CP_XML_STREAM(), crt->m_iChartType, -1);

							serialize_dPt(CP_XML_STREAM(), it->second[i], crt, (std::max)(ser->cValx, ser->cValy));//+bubbles
							
/*							if (arPivotData.empty() == false)
							{
								series->set_ref(arPivotData,  i * 2);
							}*/				
							
							if (crt->m_iChartType == CHART_TYPE_Scatter || 
								crt->m_iChartType == CHART_TYPE_Bubble)
							{
								serialize_ser(L"c:xVal", CP_XML_STREAM(), series_id, series->m_arAI[2], ser->sdtX, ser->cValx);
								serialize_ser(L"c:yVal", CP_XML_STREAM(), series_id, series->m_arAI[1], ser->sdtY, ser->cValy);

								if (crt->m_iChartType == CHART_TYPE_Bubble)
									serialize_ser(L"c:bubbleSize", CP_XML_STREAM(), series_id, series->m_arAI[3], ser->sdtBSize, ser->cValBSize);
							}
							else
							{

								serialize_ser(L"c:cat", CP_XML_STREAM(), series_id, series->m_arAI[2], ser->sdtX, ser->cValx);
								serialize_ser(L"c:val", CP_XML_STREAM(), series_id, series->m_arAI[1], ser->sdtY, ser->cValy);
							}							
//-----------------------------------------------------------------------------------------------------------------------------------------
							series->serialize_parent(CP_XML_STREAM(), chart_formats);
//-----------------------------------------------------------------------------------------------------------------------------------------
							std::wstringstream stream_dLbls;
							serialize_dLbls(stream_dLbls, it->second[i], crt);
		
							if (stream_dLbls.str().empty() == false)
							{
								CP_XML_NODE(L"c:dLbls")
								{	
									CP_XML_STREAM() << stream_dLbls.str();
								}
							}
							series_ss->serialize2(CP_XML_STREAM(), crt->m_iChartType); //особенности xlsx (
						}	
							
						series->serialize_legend(stream_legend_entries, it->second[i]); 
					}
					for (size_t i = 0 ; i < crt->m_arCrtLine.size(); i++)
					{
						CrtLine* crtLine = dynamic_cast<CrtLine*>(crt->m_arCrtLine[i].get());
						if (crtLine == NULL) continue;
						if (crtLine->id == 3) continue; // leaderLines in dLbls

						crtLine->m_iChartType = crt->m_iChartType;
						crtLine->serialize(CP_XML_STREAM());
					}	

					if (crt->m_DROPBAR[0] || crt->m_DROPBAR[1])
					{
						CP_XML_NODE(L"c:upDownBars")
						{
							if (crt->m_DROPBAR[0])
							{
								CP_XML_NODE(L"c:upBars")
								{
									crt->m_DROPBAR[0]->serialize(CP_XML_STREAM());
								}
							}
							if (crt->m_DROPBAR[1])
							{
								CP_XML_NODE(L"c:downBars")
								{
									crt->m_DROPBAR[1]->serialize(CP_XML_STREAM());		
								}
							}
						}
					}

					AXES * axes = dynamic_cast<AXES*>(parent->m_AXES.get());
					if (axes)
					{
						for (size_t a = 0 ; a < axes->m_arAxesId.size(); a++)
						{
							CP_XML_NODE(L"c:axId")
							{
								CP_XML_ATTR(L"val", axes->m_arAxesId[a].first);
							}
						}
					}
					//else error complex_29s.xls
				}
			}

			for (size_t i = 0; i < chart_formats->m_arAXISPARENT.size(); i++)
			{
				AXISPARENT* parent		= dynamic_cast<AXISPARENT*>	(chart_formats->m_arAXISPARENT[i].get());
				AxisParent* ax_parent	= dynamic_cast<AxisParent*>	(parent->m_AxisParent.get());
				AXES*		axes		= dynamic_cast<AXES*>		(parent->m_AXES.get());

				if (axes)
				{
					axes->serialize(CP_XML_STREAM(), (ax_parent->iax!=0) ); //secondary
				}
				//else error complex_29s.xls
			}

			if (PlotAreaFRAME)
				PlotAreaFRAME->serialize(CP_XML_STREAM());		
		}	

		serialize_legend (_stream, stream_legend_entries.str());

	}
	return 0;
}
int ChartSheetSubstream::serialize_scatter_style(std::wostream & _stream, CRT *crt)
{
	bool bMarker = true, bSmooth = false, bLine = true;//todooo  - так как есть отдельные настройки

	CP_XML_WRITER(_stream)
	{
		CP_XML_NODE(L"c:scatterStyle")
		{
			if (bLine)
			{
				if (bSmooth && bMarker)	CP_XML_ATTR (L"val" , L"smoothMarker");
				else if (bSmooth)		CP_XML_ATTR (L"val" , L"smooth");		
				else if (bMarker)		CP_XML_ATTR (L"val" , L"lineMarker");	
				else					CP_XML_ATTR (L"val" , L"line");	
			}
			else
			{
				if (bMarker)			CP_XML_ATTR (L"val" , L"marker");
				else					CP_XML_ATTR (L"val" , L"none");
			}
		}
	}
	return 0;
}
int ChartSheetSubstream::serialize_dPt(std::wostream & _stream, int id, CRT *crt, int count_point)
{
	if (crt == NULL) return 0;//1 (318).xls

	CHARTFORMATS	*chart_formats	= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	SERIESFORMAT	*series			= dynamic_cast<SERIESFORMAT *>(chart_formats->m_arSERIESFORMAT[id].get());

	ChartFormat		*format			= dynamic_cast<ChartFormat*>(crt->m_ChartFormat.get());

	std::map<int, bool> present_idx;

	CP_XML_WRITER(_stream)
	{
		for (size_t i = 0 ; i < series->m_arPtSS.size(); i++)
		{
			CP_XML_NODE(L"c:dPt")
			{
				SS			* series_ss				= dynamic_cast<SS *>(series->m_arPtSS[i].get());
				DataFormat	* series_data_format	= dynamic_cast<DataFormat *>(series_ss->m_DataFormat.get());

				series_ss->m_is3D		= crt->m_bIs3D;
				
				if (crt->m_iChartType == CHART_TYPE_Pie		|| 
					crt->m_iChartType == CHART_TYPE_BopPop	|| 
					crt->m_iChartType == CHART_TYPE_Doughnut || 
					crt->m_iChartType == CHART_TYPE_Surf )
						series_ss->m_isVaried	= format->fVaried;

				CP_XML_NODE(L"c:idx")
				{
					CP_XML_ATTR(L"val", series_data_format->xi);
					
					if (format->fVaried)
						present_idx.insert(std::make_pair(series_data_format->xi, true));
				}
				series_ss->serialize	(CP_XML_STREAM(), crt->m_iChartType, series_data_format->xi);
				//series_ss->serialize2	(CP_XML_STREAM(), crt->m_iChartType); // 3d shape box не нужен - Book 78 3D.xls
			}
		}

		bool varied = false;
		if (crt->m_iChartType == CHART_TYPE_Pie		|| 
			crt->m_iChartType == CHART_TYPE_BopPop	|| 
			crt->m_iChartType == CHART_TYPE_Doughnut || 
			crt->m_iChartType == CHART_TYPE_Surf )
				varied	= format->fVaried;	

		if (varied && present_idx.size() < count_point)
		{
			SS	* series_ss	= dynamic_cast<SS *>(series->m_SS.get());

			int vari_color = 0;
			for (int i = 0 ; i < count_point; i++)
			{
				std::map<int, bool>::iterator it = present_idx.find(i);
				
				if (it == present_idx.end())
				{
					CP_XML_NODE(L"c:dPt")
					{
						CP_XML_NODE(L"c:idx")
						{
							CP_XML_ATTR(L"val", i );
						}
						series_ss->serialize_default(CP_XML_STREAM(), crt->m_iChartType, i/*vari_color++*/);
					}
				}
			}

		}
	}
	return 0;
}

int ChartSheetSubstream::serialize_dLbls (std::wostream & _stream, int id, CRT *crt)
{
	CHARTFORMATS	*chart_formats	= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	SERIESFORMAT	*series			= dynamic_cast<SERIESFORMAT *>(chart_formats->m_arSERIESFORMAT[id].get());
	SS				*series_ss		= dynamic_cast<SS *>(series->m_SS.get());	
	DataFormat		*series_df		= dynamic_cast<DataFormat *>(series_ss->m_DataFormat.get());

	bool is_area = (crt->m_iChartType == CHART_TYPE_Area || crt->m_iChartType == CHART_TYPE_RadarArea);
	int series_id = series_df->iss;
	
	std::vector<std::pair<int, BaseObjectPtr>>	labels = chart_formats->find_labels ( 4, id);
 
	bool add_labels			= false;
	bool need_add_labels	= false;

	CP_XML_WRITER(_stream)
	{
		Text			* text		= NULL;
		AttachedLabel	* att_label = dynamic_cast<AttachedLabel*>(series_ss->m_AttachedLabel.get());
		ATTACHEDLABEL	* AT_LABEL	= dynamic_cast<ATTACHEDLABEL*>(chart_formats->find_label(4, id).get());
		
		if (AT_LABEL)	text = dynamic_cast<Text*> (AT_LABEL->m_TextProperties.get());
		
		if (att_label)
		{
			att_label->is_area = is_area;
			series_ss->m_AttachedLabel->serialize(_stream); 
			add_labels = true;
		}
		else 
		{				
			if ((AT_LABEL) && (AT_LABEL->m_FRAME))
			{
				AT_LABEL->m_FRAME->serialize(_stream);
				add_labels = true;
			}
			DataLabExtContents * data_ext_cont = NULL; 

			if (!data_ext_cont && AT_LABEL)
				data_ext_cont = dynamic_cast<DataLabExtContents *>(AT_LABEL->m_DataLabExtContents.get());
			
			if (!data_ext_cont && crt)
				data_ext_cont = dynamic_cast<DataLabExtContents *>(crt->m_DataLabExtContents.get());

			if (data_ext_cont)		
			{
				data_ext_cont->is_area = is_area;
				data_ext_cont->serialize(_stream);
				add_labels = true;
			}
			else if (text)
			{
				text->is_area = is_area;
				text->serialize(_stream);
				add_labels = true;
			}
			else if (!labels.empty())
			{//есть лэйблы отдельных точек, а вот общие никак не обозначены
				need_add_labels = true;
			}
		}
		Pie *pie = dynamic_cast<Pie *>(crt->m_ChartType.get());
		
		if (text)
		{
			if (text->dlp > 0 && text->dlp < 9)
			{
				CP_XML_NODE(L"c:dLblPos")
				{
					switch(text->dlp)
					{
					case 1: CP_XML_ATTR(L"val", L"outEnd"); break;
					case 2: CP_XML_ATTR(L"val", L"inEnd"); break;
					case 3: CP_XML_ATTR(L"val", L"ctr"); break;
					case 4: CP_XML_ATTR(L"val", L"inBase"); break;
					case 5: CP_XML_ATTR(L"val", L"t"); break;
					case 6: CP_XML_ATTR(L"val", L"b"); break;
					case 7: CP_XML_ATTR(L"val", L"l"); break;
					case 8: CP_XML_ATTR(L"val", L"r"); break;
					}
				}
			}
			CP_XML_NODE(L"c:showLegendKey") {CP_XML_ATTR(L"val", text->fShowKey);}
			need_add_labels = true;
		}

		if ( (pie) && (pie->fShowLdrLines) ) 
		{
			CP_XML_NODE(L"c:showLeaderLines") {CP_XML_ATTR(L"val", pie->fShowLdrLines);}
			need_add_labels = true;

		}
		if (!add_labels && need_add_labels)
		{
			CP_XML_NODE(L"c:showLegendKey")	{ CP_XML_ATTR (L"val" , 0); }
			CP_XML_NODE(L"c:showVal")		{ CP_XML_ATTR (L"val" , 0); }
			CP_XML_NODE(L"c:showPercent")	{ CP_XML_ATTR (L"val" , 0); }
			CP_XML_NODE(L"c:showBubbleSize"){ CP_XML_ATTR (L"val" , 0); }
			CP_XML_NODE(L"c:showCatName")	{ CP_XML_ATTR (L"val" , 0); }	
			CP_XML_NODE(L"c:showSerName")	{ CP_XML_ATTR (L"val" , 0); }	
		}
	//подписи к точкам (отдельные)
		for (size_t i = 0; i < labels.size(); i++)
		{
			CP_XML_NODE(L"c:dLbl")
			{
				CP_XML_NODE(L"c:idx") {CP_XML_ATTR(L"val", labels[i].first);}
				
				Text * text = NULL;
				ATTACHEDLABEL* AT_LABEL = dynamic_cast<ATTACHEDLABEL*>(labels[i].second.get());
				if (AT_LABEL) 
				{
					//if (AT_LABEL->m_CrtLayout12)	AT_LABEL->m_CrtLayout12->serialize(_stream);
					//else
					//if (AT_LABEL->m_Pos) AT_LABEL->m_Pos->serialize(_stream);
					
					text = dynamic_cast<Text*> (AT_LABEL->m_TextProperties.get());
					
					if (AT_LABEL->m_FRAME)
						AT_LABEL->m_FRAME->serialize(_stream);

					AT_LABEL->serialize_txPr(CP_XML_STREAM());
					if (text)
					{
						CP_XML_NODE(L"c:showLegendKey") {CP_XML_ATTR(L"val", text->fShowKey);}
					}
					else
						CP_XML_NODE(L"c:showLegendKey") {CP_XML_ATTR(L"val", 0);}

				}							
				DataLabExtContents * data_ext_cont = dynamic_cast<DataLabExtContents *>(crt->m_DataLabExtContents.get());

				if (!data_ext_cont && AT_LABEL)
					data_ext_cont = dynamic_cast<DataLabExtContents *>(AT_LABEL->m_DataLabExtContents.get());
				
				if (data_ext_cont)		
				{
					data_ext_cont->is_area = is_area;
					data_ext_cont->serialize(_stream);
				}
				else if (text)
				{
					text->is_area = is_area;
					text->serialize(_stream);
				}
			}
		}

		for (size_t i = 0 ; i < crt->m_arCrtLine.size(); i++)
		{
			CrtLine* crtLine = dynamic_cast<CrtLine*>(crt->m_arCrtLine[i].get());
			if (crtLine == NULL) continue;
			if (crtLine->id != 3) continue; // only leaderLines in dLbls

			crtLine->m_iChartType = crt->m_iChartType;
			crtLine->serialize(_stream);
		}
	}
	return 0;
}

int ChartSheetSubstream::serialize_ser (std::wstring sNodeSer, std::wostream & _stream, int series_id, const BaseObjectPtr & ai_, int type, int count)
{
	int result = 0;

	AI		* ai	= dynamic_cast<AI *>(ai_.get());
	BRAI	* brai	= dynamic_cast<BRAI *>(ai->m_BRAI.get());
	
	std::wstring formula = brai->formula.getAssembledFormula(true);
	
	int rt = brai->rt;
	bool b = brai->fUnlinkedIfmt;
	int fmt = brai->ifmt;

	//id
// 0 - name
// 1 - val || yVal
// 2 - cat || xVal
// 3 - bubble size
	SERIESDATA * series_data = dynamic_cast<SERIESDATA *>(m_SERIESDATA.get());
	SIIndex		* series_cash = NULL;

	int type_val = 0;
	for (size_t i = 0; (series_data) && (i < series_data->m_arSIIndex.size()); i++)
	{
		SIIndex * si_in = dynamic_cast<SIIndex *>(series_data->m_arSIIndex[i].get());

		int a1 = si_in->numIndex;
		int a2 = brai->id;
		if (a1 == a2)
		{
			if (si_in->m_arData.empty() == false)
			{
				switch(si_in->m_arData[0]->get_type())
				{
				case typeNumber:	type_val = 1; break;
				case typeBoolErr:	type_val = 2; break;
				case typeBlank:		type_val = 3; break;
				case typeLabel:		type_val = 4; break;
				default:
					break;
				}

				series_cash = si_in;
			}
			break;
		}
	}
	std::wstringstream _stream_cash;
	int count_found = 0;
	
	if ((series_cash) && (!series_cash->m_arData.empty()))
	{
		for (int i =0; i < count; i++)
		{
			CellRef ref( i, series_id , true, true);
			if (series_cash->serialize(_stream_cash, i, ref.toString()) == 1) count_found++;
		}
	}

	if ((formula.empty()) && count_found > 0)
	{
		std::wstring sNodeLit;
		if (type == 1 ) sNodeLit = L"c:numLit";
		if (type == 3 ) sNodeLit = L"c:strLit";

		CP_XML_WRITER(_stream)    
		{	
			CP_XML_NODE(sNodeSer) 
			{
				CP_XML_NODE(sNodeLit) 
				{
					//CP_XML_NODE(L"c:formatCode") {	CP_XML_STREAM() << L"General";	}		

					CP_XML_NODE(L"c:ptCount")
					{
						CP_XML_ATTR(L"val", count); // count_found использовать нельзя - тут должно быть максимальное всех точек отчета
					}
					CP_XML_STREAM() << _stream_cash.str();
				
					result = 1;
				}
			}
		}
	}
	else if (!formula.empty())
	{
		std::wstring sNodeRef;
		if (type == 1)	sNodeRef = L"c:numRef";
		if (type == 3)	sNodeRef = L"c:strRef";

		CP_XML_WRITER(_stream)    
		{	
			CP_XML_NODE(sNodeSer) 
			{
				CP_XML_NODE(sNodeRef) 
				{
					//CP_XML_NODE(L"c:formatCode") {	CP_XML_STREAM() << L"General";	}		
					
					CP_XML_NODE(L"c:f")
					{
						CP_XML_STREAM() << formula;
					}
					if (count_found > 0)
					{
						if (type == 1)	sNodeRef = L"c:numCache";
						if (type == 3)	sNodeRef = L"c:strCache";
						CP_XML_NODE(sNodeRef) 
						{
							CP_XML_NODE(L"c:ptCount")
							{
								CP_XML_ATTR(L"val", count); // count_found использовать нельзя - тут должно быть максимальное всех точек отчета
							}
							CP_XML_STREAM() << _stream_cash.str();
						}
					}
					result = 1;
				}
			}
		}
	}

	return result;
}

} // namespace XLS

