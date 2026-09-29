/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "SS.h"
#include "GELFRAME.h"
#include "SHAPEPROPS.h"
#include "CRTMLFRT.h"

#include"../Biff_records/DataFormat.h"
#include"../Biff_records/Begin.h"
#include"../Biff_records/Chart3DBarShape.h"
#include"../Biff_records/LineFormat.h"
#include"../Biff_records/AreaFormat.h"
#include"../Biff_records/PieFormat.h"
#include"../Biff_records/SerFmt.h"
#include"../Biff_records/MarkerFormat.h"
#include"../Biff_records/AttachedLabel.h"
#include"../Biff_records/End.h"
#include"../Biff_records/StartObject.h"
#include"../Biff_records/EndObject.h"
#include"../Biff_records/ShapePropsStream.h"

#include"../../../../../OOXML/PPTXFormat/Logic/SpPr.h"

namespace XLS
{

	std::wstring default_series_fill_color[] =
	{
		L"9999FF",  L"993366", 	L"FFFFCC", 	L"CCFFFF", 	L"660066", 	L"FF8080", 	L"0066CC", 	L"CCCCFF", 	L"800000", 	
		L"FF00FF", 	L"FFFF00", 	L"00FFFF", 	L"800080", 	L"800000", 	L"808000", 	L"0000FF", 	L"00CCFF", 	L"CCFFFF", 	
		L"CCFFCC", 	L"FFFF99", 	L"99CCFF", 	L"FF99CC", 	L"CC99FF", 	L"FFCC99", 	L"3366FF", 	L"33CCCC", 	L"99CC00", 	
		L"FFCC00", 	L"FF9900", 	L"FF6600", 	L"666699", 	L"969696", 	L"336600", 	L"339966", 	L"330000", 	L"333300", 	
		L"993300", 	L"993366", 	L"333399", 	L"333333", 	L"000000", 	L"FFFFFF", 	L"FF0000", 	L"00FF00", 	L"0000FF", 	L"FFFF00", 	
		L"FF00FF", 	L"00FFFF", 	L"800000", 	L"800000", 	L"800000", 	L"808000", 	L"800080", 	L"808000", 	L"C0C0C0", 	L"808080"
	};

	std::wstring default_series_line_color[] =
	{	
		L"000080", 	L"FF00FF", 	L"FFFF00", 	L"00FFFF", 	L"800080", 	L"800000", 	L"808000", 	L"0000FF", 	L"00CCFF", 	L"CCFFFF", 	
		L"CCFFCC", 	L"FFFF99", 	L"99CCFF", 	L"FF99CC", 	L"CC99FF", 	L"FFCC99", 	L"3366FF", 	L"33CCCC", 	L"99CC00", 	L"FFCC00", 	
		L"FF9900", 	L"FF6600", 	L"666699", 	L"969696", 	L"336600", 	L"339966", 	L"330000", 	L"333300", 	L"993300", 	L"993366", 	
		L"333399", 	L"000000", 	L"FFFFFF", 	L"FF0000", 	L"00FF00", 	L"0000FF", 	L"FFFF00", 	L"FF00FF", 	L"00FFFF", 	L"800000", 	
		L"800000", 	L"800000", 	L"808000", 	L"800080", 	L"808000", 	L"C0C0C0", 	L"808080", 	L"9999FF", 	L"993366", 	L"FFFFCC", 
		L"CCFFFF", 	L"660066", 	L"FF8080", 	L"0066CC", 	L"CCCCFF",
		//todoooo - check what MS uses further 
		L"000080", 	L"FF00FF", 	L"FFFF00", 	L"00FFFF", 	L"800080", 	L"800000", 	L"808000", 	L"0000FF", 	L"00CCFF", 	L"CCFFFF", 	
		L"CCFFCC", 	L"FFFF99", 	L"99CCFF", 	L"FF99CC", 	L"CC99FF", 	L"FFCC99", 	L"3366FF", 	L"33CCCC", 	L"99CC00", 	L"FFCC00", 	
		L"FF9900", 	L"FF6600", 	L"666699", 	L"969696", 	L"336600", 	L"339966", 	L"330000", 	L"333300", 	L"993300", 	L"993366", 	
		L"333399", 	L"000000", 	L"FFFFFF", 	L"FF0000", 	L"00FF00", 	L"0000FF", 	L"FFFF00", 	L"FF00FF", 	L"00FFFF", 	L"800000", 	
		L"800000", 	L"800000", 	L"808000", 	L"800080", 	L"808000", 	L"C0C0C0", 	L"808080", 	L"9999FF", 	L"993366", 	L"FFFFCC", 
		L"CCFFFF", 	L"660066", 	L"FF8080", 	L"0066CC", 	L"CCCCFF"
	};



SS::SS()
{
	m_is3D		= false;

	m_isAutoFill = true;
	m_isAutoLine = true;
}


SS::~SS()
{
}


BaseObjectPtr SS::clone()
{
	return BaseObjectPtr(new SS(*this));
}


/*
SS = DataFormat Begin [Chart3DBarShape] [LineFormat AreaFormat PieFormat] [SerFmt] [GELFRAME] 
		[MarkerFormat] [AttachedLabel] *2SHAPEPROPS [CRTMLFRT] End
*/
const bool SS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DataFormat>())
	{
		return false;
	}
	m_DataFormat = elements_.back();
	elements_.pop_back();

	bool  bRead = false;
	if (proc.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		bRead = true;
	}
	if (proc.optional<Begin>()) //in biff5 may or may not be present, biff8 - must be present
	{
		bRead = true;
		elements_.pop_back();
	}
	if (bRead)
	{
		while (true)
		{
			CFRecordType::TypeId type = proc.getNextRecordType();
			
			if (type == rt_NONE) break;
			if (type == rt_End) 
			{
				if (proc.mandatory<End>())	elements_.pop_back();
				break;
			}

			switch(type)
			{
				case rt_Chart3DBarShape:
				{
					if (proc.optional<Chart3DBarShape>())
					{
						m_Chart3DBarShape = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_LineFormat:
				{
					if(proc.optional<LineFormat>())
					{
						m_LineFormat = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_AreaFormat:
				{
					if (proc.optional<AreaFormat>())
					{
						m_AreaFormat = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_PieFormat:
				{					
					if (proc.optional<PieFormat>())
					{
						m_PieFormat = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_SerFmt:
				{
					if (proc.optional<SerFmt>())
					{
						m_SerFmt = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_GelFrame:
				{
					if (proc.optional<GELFRAME>())
					{
						m_GELFRAME = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_MarkerFormat:
				{
					if (proc.optional<MarkerFormat>())
					{
						m_MarkerFormat = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_AttachedLabel:
				{
					if (proc.optional<AttachedLabel>())
					{
						m_AttachedLabel = elements_.back();
						elements_.pop_back();
					}
				}break;
				case rt_ShapePropsStream:
				{	
					int count = proc.repeated<SHAPEPROPS>(0, 2);
					while(count > 0)
					{
						m_arSHAPEPROPS.insert(m_arSHAPEPROPS.begin(), elements_.back());
						elements_.pop_back();
						count--;
					}
				}break;
				case rt_CrtMlFrt:
				{
					proc.optional<CRTMLFRT>();
				}break;
				case rt_StartBlock:
				case rt_EndBlock:
				case rt_StartObject:
				{
					if (proc.optional<StartObject>())
					{
						elements_.pop_back();
						if (proc.mandatory<EndObject>())	elements_.pop_back();
					}
				}break;
				default://skip					
				{
					return true;
				}break;
			}
		}
	}

	return true;
}

const bool SS::saveContent(BinProcessor& proc)
{
	if(m_DataFormat == nullptr)
		return false;
	proc.mandatory(*m_DataFormat);
	proc.mandatory<Begin>();
	if(m_Chart3DBarShape != nullptr)
		proc.mandatory(*m_Chart3DBarShape);
	std::vector<BYTE> CheckSumData;
	_UINT32 checkSum = 0;
	auto calcChecksum = [](const std::vector<uint8_t>& data, _UINT32 &checksum)
	{
		for (BYTE byte : data) {
			checksum ^= (static_cast<_UINT32>(byte) << 24);

			for (int i = 0; i < 8; ++i) {
				if (checksum & 0x80000000U) {
					checksum = (checksum << 1) ^ 0x000000AFU;
				} else {
					checksum <<= 1;
				}
			}
		}
	};
	/*if(m_LineFormat != nullptr)
	{
		proc.mandatory(*m_LineFormat);
		auto castedLine = static_cast<LineFormat*>(m_LineFormat.get());
		castedLine->setChecksum(CheckSumData);
	}
	else if(!m_arSHAPEPROPS.empty())
	{
		LineFormat lnFmt;
		lnFmt.setChecksum(CheckSumData);
		proc.mandatory(lnFmt);
	}
	if(m_AreaFormat != nullptr)
	{
		proc.mandatory(*m_AreaFormat);
		auto castedArea = static_cast<AreaFormat*>(m_AreaFormat.get());
		if(!castedArea->fAuto)
			castedArea->setChecksum(CheckSumData);
	}
	else if(!m_arSHAPEPROPS.empty())
	{
		AreaFormat areaFmt;
		proc.mandatory(areaFmt);
	}*/
	{
		LineFormat line;
		line.rgb.red = 0xFFFF;
		line.rgb.green = 0xFFFF;
		line.rgb.blue = 0xFFFF;
		line.lns = 0;
		line.we = 0;
		line.icv = 9;
		line.fAuto = false;
		line.fAutoCo = false;
		line.setChecksum(CheckSumData);
		proc.mandatory(line);
		AreaFormat area;
		area.fls = 1;
		area.fAuto = false;
		area.setChecksum(CheckSumData);
		proc.mandatory(area);
	}
	calcChecksum(CheckSumData, checkSum);
	if(m_PieFormat != nullptr)
		proc.mandatory(*m_PieFormat);
	if(m_SerFmt != nullptr)
		proc.mandatory(*m_SerFmt);
	if(m_GELFRAME != nullptr)
		proc.optional(*m_GELFRAME);
	if(m_MarkerFormat != nullptr)
		proc.mandatory(*m_MarkerFormat);
	if(m_AttachedLabel != nullptr)
		proc.mandatory(*m_AttachedLabel);
	for(auto i : m_arSHAPEPROPS)
	{
		auto castedProp = dynamic_cast<SHAPEPROPS*>(i.get());
		if(castedProp == nullptr)
			continue;
		auto propHeader = dynamic_cast<ShapePropsStream*>(castedProp->m_ShapePropsStream.get());
		if(propHeader == nullptr)
			continue;

		if(propHeader->wObjContext == 0)
			propHeader->dwChecksum = checkSum;
		else if(propHeader->wObjContext == 1 && m_MarkerFormat != nullptr)
		{
			auto castedMarker = static_cast<MarkerFormat*>(m_MarkerFormat.get());
			AreaFormat area;
			area.rgbFore.red = castedMarker->rgbFore.red;
			area.rgbFore.green = castedMarker->rgbFore.green;
			area.rgbFore.blue = castedMarker->rgbFore.blue;
			area.rgbBack.red = castedMarker->rgbBack.red;
			area.rgbBack.green = castedMarker->rgbBack.green;
			area.rgbBack.blue = castedMarker->rgbBack.blue;
			area.fls = 1;
			area.fAuto = false;
			CheckSumData = std::vector<BYTE>();
			_UINT32 MarkerCheckSum = 0;
			area.setChecksum(CheckSumData);
			calcChecksum(CheckSumData, MarkerCheckSum);
			propHeader->dwChecksum = MarkerCheckSum;
		}
		proc.mandatory(*i);
	}

	proc.mandatory<End>();
	return true;
}

void SS::apply_crt_ss (BaseObjectPtr crt_ss)
{
	SS * ss_common = dynamic_cast<SS*>(crt_ss.get());
	if (ss_common == NULL) return;

	if (ss_common->m_PieFormat && !m_PieFormat)			m_PieFormat = ss_common->m_PieFormat;
	if (ss_common->m_MarkerFormat && !m_MarkerFormat)	m_MarkerFormat = ss_common->m_MarkerFormat;
	//.... ???
}

int SS::serialize_default(std::wostream & _stream, int series_type, int ind )
{
	DataFormat	*series_data_format = dynamic_cast<DataFormat *>(m_DataFormat.get());
	SerFmt		*series_format		= dynamic_cast<SerFmt *>	(m_SerFmt.get());

	CP_XML_WRITER(_stream)    
	{
		if (series_data_format == NULL || (!m_isAutoLine && !m_isAutoFill)) continue;

		CP_XML_NODE(L"c:spPr")
		{
			if (m_isAutoFill && series_type != CHART_TYPE_Line && series_type != CHART_TYPE_Scatter && ind < 57) //line & scatter
			{			
				CP_XML_NODE(L"a:solidFill")
				{
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",  default_series_fill_color[ind]);		
					}
				}
			}
			if (series_type != CHART_TYPE_Line && series_type != CHART_TYPE_Scatter)
			{
				ind = 31; //black
				LineFormat* line = dynamic_cast<LineFormat*>(m_LineFormat.get());
				
				if ((line) && (line->lns == (_UINT16)5)) ind = -1;
			}
			const size_t colorsCount = sizeof(default_series_line_color) / sizeof(default_series_line_color[0]);
			if (ind >= 0 && colorsCount > ind &&  m_isAutoLine)
			{
				CP_XML_NODE(L"a:ln")
				{
					CP_XML_ATTR(L"w", 12700);	//single
					CP_XML_NODE(L"a:solidFill")
					{
						CP_XML_NODE(L"a:srgbClr")
						{
							CP_XML_ATTR(L"val",  default_series_line_color[ind]);		
						}
					}
					CP_XML_NODE(L"a:prstDash")
					{
						CP_XML_ATTR(L"val", L"solid");
					}
				}
			}
			else
			{
			}
		}
		//if (m_MarkerFormat && series_type != CHART_TYPE_Bubble)
		//	m_MarkerFormat->serialize(_stream);
		//else
		//{
		//	//generation (series_data_format->iss)
		//	//todooo
		//}
		if (m_PieFormat && (series_type == CHART_TYPE_Doughnut	|| 
							series_type == CHART_TYPE_BopPop	|| 
							series_type == CHART_TYPE_Pie))
		{
			m_PieFormat->serialize(_stream);
		}
		if (m_Chart3DBarShape && series_type == CHART_TYPE_Bar && m_is3D)
		{
			m_Chart3DBarShape->serialize(_stream);
		}
		
		if (series_format)
		{
			if (series_type == CHART_TYPE_Line || series_type == CHART_TYPE_Radar || series_type == CHART_TYPE_Scatter)
			{
				CP_XML_NODE(L"c:smooth")
				{
					CP_XML_ATTR(L"val", series_format->fSmoothedLine);
				}
			}
			if (series_format->f3DBubbles == true)
			{
				CP_XML_NODE(L"c:bubble3D"){	CP_XML_ATTR (L"val" , 1);}
			}
		}
	}
	return 0;
}

int SS::serialize(std::wostream & _stream, int series_type, int indPt)
{
	DataFormat		*series_data_format = dynamic_cast<DataFormat*>		(m_DataFormat.get());
	SerFmt			*series_format		= dynamic_cast<SerFmt*>			(m_SerFmt.get());
	MarkerFormat	*marker_format		= dynamic_cast<MarkerFormat*>	(m_MarkerFormat.get());
	ShapePropsStream *spStream			= nullptr;
	ShapePropsStream *mkPropStream		= nullptr;
	if(!m_arSHAPEPROPS.empty())
	{
		for(auto spProp : m_arSHAPEPROPS)
		{
			auto SpProps = dynamic_cast<SHAPEPROPS*>(spProp.get());
			if(SpProps == nullptr)
				continue;
			auto tempStream = dynamic_cast<ShapePropsStream*>(SpProps->m_ShapePropsStream.get());
			if(tempStream == nullptr)
				continue;

			if (tempStream->wObjContext == 0)
				spStream = tempStream;
			else if(tempStream->wObjContext == 1)
				mkPropStream  = tempStream;
		}
	}

	int ind = series_data_format->iss;

	AreaFormat * area = dynamic_cast<AreaFormat*>(m_AreaFormat.get());
	LineFormat * line = dynamic_cast<LineFormat*>(m_LineFormat.get());

	bool bArea = m_GELFRAME ? true : false;
	if (!bArea && (area) && (area->fAuto == false)) bArea = true;

	bool bLine = false;
	if ((line) && (line->fAuto == false)) bLine = true;

	bool bMarkerEx = false;
	if(spStream)
	{
		std::wstring spPr = spStream->getChartSpPr();
		if (false == spPr.empty())
		{
			_stream.write(spPr.c_str(), spPr.size());
			bLine = false;
			bArea = false;
			indPt = 0;
		}
	}

	CP_XML_WRITER(_stream)    
	{
		if (series_data_format == NULL) continue;

		if ((bLine || bArea ) || (indPt < 0))
		{
			CP_XML_NODE(L"c:spPr")
			{
				if (m_is3D == true || (	series_type != CHART_TYPE_Line		&& 
										series_type != CHART_TYPE_Scatter	&& 
										series_type != CHART_TYPE_Stock)) 
				{		
					if (m_GELFRAME && bArea)			
					{
						m_GELFRAME->serialize(CP_XML_STREAM());
						m_isAutoFill = false;
					}
					else if (m_AreaFormat/* && bArea*/)	
					{
						m_AreaFormat->serialize(CP_XML_STREAM());
						m_isAutoFill = false;
					}
					else
					{
						if ((m_isVaried) && (*m_isVaried == false)) ind = 0;
						if (ind < 57)
						{
							CP_XML_NODE(L"a:solidFill")
							{
								CP_XML_NODE(L"a:srgbClr")
								{
									CP_XML_ATTR(L"val", default_series_fill_color[ind]);
								}
							}
						}
					}
				}
				else 
					bMarkerEx = true;
				
				if (m_LineFormat/* && bLine*/)
				{
					m_LineFormat->serialize(CP_XML_STREAM());
					m_isAutoLine = false;
				}
				else
				{
					//generation (automatic)
					if (series_type == CHART_TYPE_Scatter || series_type == CHART_TYPE_Stock) 
					{
					}	
					else if ( series_type != CHART_TYPE_Line || m_is3D == true ) //line & !3dLine
						ind = 31; //black
					const size_t colorsCount = sizeof(default_series_line_color) / sizeof(default_series_line_color[0]);
					CP_XML_NODE(L"a:ln")
					{
						CP_XML_ATTR(L"w", 12700);//single

						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								if(colorsCount > ind)
									CP_XML_ATTR(L"val",  default_series_line_color[ind]);
								else
									CP_XML_ATTR(L"val", L"000000");
							}
						}
						CP_XML_NODE(L"a:prstDash")
						{
							CP_XML_ATTR(L"val", L"solid");
						}
					}
				}
			}
		}
		if (m_PieFormat && (series_type == CHART_TYPE_Doughnut	|| 
							series_type == CHART_TYPE_BopPop	|| 
							series_type == CHART_TYPE_Pie))
		{
			m_PieFormat->serialize(_stream);
		}

		if (marker_format &&	series_type != CHART_TYPE_Bubble	&& 
								series_type != CHART_TYPE_Bar		&&	
								series_type != CHART_TYPE_BopPop )
		{
			std::wstring MkSpPr;
			if(mkPropStream != nullptr)
				MkSpPr = mkPropStream->getChartSpPr();

			marker_format->serialize(_stream, ind, bMarkerEx ? m_GELFRAME : BaseObjectPtr(), MkSpPr);
		}
		else if (/*series_type == CHART_TYPE_Line ||*/ series_type == CHART_TYPE_Scatter)
		{
			CP_XML_NODE(L"c:marker")
			{
				CP_XML_NODE(L"c:symbol")
				{
					CP_XML_ATTR(L"val", L"none");
				}
			}
		}
		
	}
	return 0;
}

int SS::serialize2(std::wostream & _stream, int series_type)
{
	DataFormat	*series_data_format = dynamic_cast<DataFormat *>(m_DataFormat.get());
	SerFmt		*series_format		= dynamic_cast<SerFmt *>	(m_SerFmt.get());

	int ind = series_data_format->iss;

	CP_XML_WRITER(_stream)    
	{
		if (m_Chart3DBarShape && series_type == CHART_TYPE_Bar && m_is3D)
		{
			m_Chart3DBarShape->serialize(_stream);
		}

		if (series_type == CHART_TYPE_Bubble)
		{
			if ((series_format) && (series_format->f3DBubbles == true))
			{
				CP_XML_NODE(L"c:bubble3D"){	CP_XML_ATTR (L"val" , 1);}
			}
		}
		
		if (m_LineFormat && (series_type == CHART_TYPE_Line || series_type == CHART_TYPE_Radar))
		{
			if (series_format)
			{
				CP_XML_NODE(L"c:smooth")
				{
					CP_XML_ATTR(L"val", series_format->fSmoothedLine);
				}
			}
			else
				CP_XML_NODE(L"c:smooth") {	CP_XML_ATTR(L"val", 0); }
		}
	}
	return 0;
}

} // namespace XLS

