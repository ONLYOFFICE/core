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
	/*
	if(m_LineFormat != nullptr)
	{
		proc.mandatory(*m_LineFormat);
		auto castedLine = static_cast<LineFormat*>(m_LineFormat.get());
		castedLine->setChecksum(ShapePropsStreamChecksumData);
	}
	if(m_AreaFormat != nullptr)
	{
		proc.mandatory(*m_AreaFormat);
		auto castedArea = static_cast<AreaFormat*>(m_AreaFormat.get());
		castedArea->setChecksum(ShapePropsStreamChecksumData);
	}*/
	{
		LineFormat line;
		line.rgb.red = 0xFFFF;
		line.rgb.green = 0xFFFF;
		line.rgb.blue = 0xFFFF;
		line.lns = 0;
		line.we = 0;
		line.icv = 9;
		proc.mandatory(line);
		AreaFormat area;
		area.rgbFore.red = 0xFF;
		area.rgbFore.green = 0x00;
		area.rgbFore.blue = 0x00;
		area.rgbBack.red = 0xFF;
		area.rgbBack.green = 0xFF;
		area.rgbBack.blue = 0xFF;
		area.fls = 1;
		area.fAuto = false;
		area.icvFore = 10;
		area.icvBack = 9;
		proc.mandatory(area);
	}


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
		if(i != nullptr)
		{
			auto castedProp = static_cast<SHAPEPROPS*>(i.get());
			auto propHeader = static_cast<ShapePropsStream*>(castedProp->m_ShapePropsStream.get());
			propHeader->dwChecksum = 0x1E563DFF;
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

			if (ind >= 0 && default_series_line_color->size() > ind &&  m_isAutoLine)
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
	SHAPEPROPS		*SpProps			= nullptr;
	ShapePropsStream *spStream			= nullptr;
	if(!m_arSHAPEPROPS.empty())
	{

		SpProps =dynamic_cast<SHAPEPROPS*>	(m_arSHAPEPROPS.back().get());
		spStream = static_cast<ShapePropsStream*>(SpProps->m_ShapePropsStream.get());
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
		XmlUtils::CXmlLiteReader reader;
		auto spPos = spStream->xml_.find("<a:spPr");
		if (spPos != std::string::npos)
		spStream->xml_ = spStream->xml_.erase(0,spPos);
		std::wstring buf(spStream->xml_.begin(), spStream->xml_.end());
		reader.FromString(buf);
		reader.ReadNextNode();
		PPTX::Logic::SpPr props;
		props.fromXML(reader);
		NSBinPptxRW::CXmlWriter writer;
		writer.m_lDocType = XMLWRITER_DOC_TYPE_CHART;
		props.toXmlWriter(&writer);

		_stream.write(writer.GetXmlString().c_str(), writer.GetXmlString().size());
		bLine = false;
		bArea = false;
		indPt = 0;
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

					CP_XML_NODE(L"a:ln")
					{
						CP_XML_ATTR(L"w", 12700);//single

						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								if(default_series_line_color->size() > ind)
									CP_XML_ATTR(L"val",  default_series_line_color[ind]);
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
			marker_format->serialize(_stream, ind, bMarkerEx ? m_GELFRAME : BaseObjectPtr());
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

