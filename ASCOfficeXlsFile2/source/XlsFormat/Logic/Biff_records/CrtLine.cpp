
#include "CrtLine.h"

namespace XLS
{

CrtLine::CrtLine()
{
	m_iChartType = -1;
}


CrtLine::~CrtLine()
{
}


BaseObjectPtr CrtLine::clone()
{
	return BaseObjectPtr(new CrtLine(*this));
}


void CrtLine::writeFields(CFRecord& record)
{
	record << id;
}


void CrtLine::readFields(CFRecord& record)
{
	record >> id;
}

int CrtLine::serialize(std::wostream & _stream)
{
	std::wstring sNode;
	switch (id)
	{
		case 0:	sNode = L"c:dropLines";		break; //m_iChartType == 2 || m_iChartType == 3 (line & area & stock)
		case 1: sNode = L"c:hiLowLines";	break; // m_iChartType == 2, 11 (line & stock)
		case 2: sNode = L"c:serLines";		break; // m_iChartType == 1 || m_iChartType == 8 (bar & ofPie)
		case 3: sNode = L"c:leaderLines";	break; // m_iChartType == 3 (pie)
	}
	if (sNode.empty())
		return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(sNode)
		{
			CP_XML_NODE(L"c:spPr")
			{
				if (m_LineFormat)
					m_LineFormat->serialize(CP_XML_STREAM());
				else
				{
					CP_XML_NODE(L"a:ln")
					{
						CP_XML_ATTR(L"w", 3175);	
						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR (L"val" , L"000000"); 
							}
						}
						CP_XML_NODE(L"a:prstDash")
						{
							CP_XML_ATTR (L"val" , L"solid"); 
						}		
					}
				}
			}
		}

		if (m_iChartType == 2)
		{
			CP_XML_NODE(L"c:marker") {	CP_XML_ATTR (L"val" , L"1"); }	
		}
	}	
	return 0;

}

} // namespace XLS

