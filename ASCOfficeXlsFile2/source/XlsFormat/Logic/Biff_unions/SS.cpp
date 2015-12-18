
#include "SS.h"
#include <Logic/Biff_records/DataFormat.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Chart3DBarShape.h>
#include <Logic/Biff_records/LineFormat.h>
#include <Logic/Biff_records/AreaFormat.h>
#include <Logic/Biff_records/PieFormat.h>
#include <Logic/Biff_records/SerFmt.h>
#include <Logic/Biff_records/MarkerFormat.h>
#include <Logic/Biff_records/AttachedLabel.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_unions/GELFRAME.h>
#include <Logic/Biff_unions/SHAPEPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>

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
		L"800000", 	L"FF00FF", 	L"FFFF00", 	L"00FFFF", 	L"800080", 	L"800000", 	L"808000", 	L"0000FF", 	L"00CCFF", 	L"CCFFFF", 	
		L"CCFFCC", 	L"FFFF99", 	L"99CCFF", 	L"FF99CC", 	L"CC99FF", 	L"FFCC99", 	L"3366FF", 	L"33CCCC", 	L"99CC00", 	L"FFCC00", 	
		L"FF9900", 	L"FF6600", 	L"666699", 	L"969696", 	L"336600", 	L"339966", 	L"330000", 	L"333300", 	L"993300", 	L"993366", 	
		L"333399", 	L"000000", 	L"FFFFFF", 	L"FF0000", 	L"00FF00", 	L"0000FF", 	L"FFFF00", 	L"FF00FF", 	L"00FFFF", 	L"800000", 	
		L"800000", 	L"800000", 	L"808000", 	L"800080", 	L"808000", 	L"C0C0C0", 	L"808080", 	L"9999FF", 	L"993366", 	L"FFFFCC", 
		L"CCFFFF", 	L"660066", 	L"FF8080", 	L"0066CC", 	L"CCCCFF",
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

	proc.mandatory<Begin>();						elements_.pop_back();
	if (proc.optional<Chart3DBarShape>())
	{
		m_Chart3DBarShape = elements_.back();
		elements_.pop_back();
	}
	if(proc.optional<LineFormat>())
	{
		m_LineFormat = elements_.back();
		elements_.pop_back();
	
		if (proc.optional<AreaFormat>())
		{
			m_AreaFormat = elements_.back();
			elements_.pop_back();
		}
		
		if (proc.optional<PieFormat>())
		{
			m_PieFormat = elements_.back();
			elements_.pop_back();
		}
	}
	if (proc.optional<SerFmt>())
	{
		m_SerFmt = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<GELFRAME>())
	{
		m_GELFRAME = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<MarkerFormat>())
	{
		m_MarkerFormat = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<AttachedLabel>())
	{
		m_AttachedLabel = elements_.back();
		elements_.pop_back();
	}
	
	int count = proc.repeated<SHAPEPROPS>(0, 2);
	while(count > 0)
	{
		m_arSHAPEPROPS.insert(m_arSHAPEPROPS.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();								elements_.pop_back();

	return true;
}

void SS::apply_crt_ss (BaseObjectPtr crt_ss)
{
	SS * ss_common = dynamic_cast<SS*>(crt_ss.get());
	if (ss_common == NULL) return;

	if (ss_common->m_PieFormat && !m_PieFormat)	m_PieFormat = ss_common->m_PieFormat;
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
			if (m_isAutoFill && series_type != 2 && series_type != 9) //line & scatter
			{			
				CP_XML_NODE(L"a:solidFill")
				{
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",  default_series_fill_color[ind]);		
					}
				}
			}
			if (series_type != 2 && series_type != 9)
			{
				ind = 31; //black
				LineFormat* line = dynamic_cast<LineFormat*>(m_LineFormat.get());
				
				if ((line) && (line->lns == (_UINT16)5)) ind = -1;
			}

			if (ind >= 0 && m_isAutoLine)
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
		//if (m_MarkerFormat && series_type != 10)
		//	m_MarkerFormat->serialize(_stream);
		//else
		//{
		//	//генерация (series_data_format->iss)
		//	//todooo
		//}
		if (m_PieFormat && (series_type == 12 || series_type == 8 || series_type == 3))
		{
			m_PieFormat->serialize(_stream);
		}
		if (m_Chart3DBarShape && series_type == 1 && m_is3D)
		{
			m_Chart3DBarShape->serialize(_stream);
		}
		
		if (series_format)
		{
			if (series_type == 2 || series_type == 6 || series_type == 9)
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

	DataFormat	*series_data_format = dynamic_cast<DataFormat *>(m_DataFormat.get());
	SerFmt		*series_format		= dynamic_cast<SerFmt *>	(m_SerFmt.get());

	int ind = series_data_format->iss;

	AreaFormat * area = dynamic_cast<AreaFormat*>(m_AreaFormat.get());
	LineFormat * line = dynamic_cast<LineFormat*>(m_LineFormat.get());

	bool bArea = m_GELFRAME ? true : false;
	if (!bArea && (area) && (area->fAuto == false)) bArea = true;

	bool bLine = false;
	if ((line) && (line->fAuto == false)) bLine = true;

	CP_XML_WRITER(_stream)    
	{
		if (series_data_format == NULL) continue;

		if ((bLine || bArea ) || (indPt < 0))
		{
			CP_XML_NODE(L"c:spPr")
			{
				if ((series_type != 2 && series_type != 9 && series_type != 11) || m_is3D == true ) //line & scatter & stock
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
						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val",  default_series_fill_color[ind]);		
							}
						}
					}
				}
				if (m_LineFormat/* && bLine*/)
				{
					m_LineFormat->serialize(CP_XML_STREAM());
					m_isAutoLine = false;
				}
				else
				{
					//генерация (automatic)
					if ( series_type != 2 || m_is3D == true ) //line & !3dLine
						ind = 31; //black
					CP_XML_NODE(L"a:ln")
					{
						CP_XML_ATTR(L"w", 12700);//single

						if (series_type == 9 || series_type == 11) //points only - todooo сделать дефолтовые точки ala 95 стиль & stork
						{
							CP_XML_NODE(L"a:noFill");
						}
						else
						{
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
				}
			}
		}
		if (m_PieFormat && (series_type == 12 || series_type == 8 || series_type == 3))
			m_PieFormat->serialize(_stream);

		if (m_MarkerFormat && series_type != 10 && series_type != 1)
			m_MarkerFormat->serialize(_stream);
		else if (/*series_type == 2 ||*/ series_type == 9)
		{
			CP_XML_NODE(L"c:marker");
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
		if (m_Chart3DBarShape && series_type == 1 && m_is3D)
		{
			m_Chart3DBarShape->serialize(_stream);
		}

		if (series_type == 10)
		{
			if ((series_format) && (series_format->f3DBubbles == true))
			{
				CP_XML_NODE(L"c:bubble3D"){	CP_XML_ATTR (L"val" , 1);}
			}
		}
		
		if ((series_type == 2 || series_type == 6 || series_type == 9) && m_isAutoLine == false)
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

