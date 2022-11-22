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

void CrtLine::readFields(CFRecord& record)
{
	record >> id;
}

int CrtLine::serialize(std::wostream & _stream)
{
	std::wstring sNode;
	switch (id)
	{
		case 0:	sNode = L"c:dropLines";		break; // m_iChartType == CHART_TYPE_Line , CHART_TYPE_Area, CHART_TYPE_Stock
		case 1: sNode = L"c:hiLowLines";	break; // m_iChartType == CHART_TYPE_Line, CHART_TYPE_Stock
		case 2: sNode = L"c:serLines";		break; // m_iChartType == CHART_TYPE_Bar || m_iChartType == CHART_TYPE_BopPop
		case 3: sNode = L"c:leaderLines";	break; // m_iChartType == CHART_TYPE_Pie
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

		if (m_iChartType == /*CHART_TYPE_Line*/2)
		{
			CP_XML_NODE(L"c:marker") {	CP_XML_ATTR (L"val" , L"1"); }	
		}
	}	
	return 0;

}

} // namespace XLS

