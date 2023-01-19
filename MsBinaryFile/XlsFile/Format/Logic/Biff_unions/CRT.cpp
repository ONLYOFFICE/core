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

#include "CRT.h"
#include "LD.h"
#include "DROPBAR.h"
#include "SS.h"
#include "SHAPEPROPS.h"
#include "DFTTEXT.h"

#include "../Biff_records/ChartFormat.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/Bar.h"
#include "../Biff_records/Line.h"
#include "../Biff_records/BopPop.h"
#include "../Biff_records/BopPopCustom.h"
#include "../Biff_records/Pie.h"
#include "../Biff_records/Area.h"
#include "../Biff_records/Scatter.h"
#include "../Biff_records/Radar.h"
#include "../Biff_records/RadarArea.h"
#include "../Biff_records/Surf.h"
#include "../Biff_records/CrtLink.h"
#include "../Biff_records/SeriesList.h"
#include "../Biff_records/Chart3d.h"
#include "../Biff_records/CrtLine.h"
#include "../Biff_records/LineFormat.h"
#include "../Biff_records/DataLabExtContents.h"
#include "../Biff_records/End.h"
#include "../Biff_records/ValueRange.h"
#include "../Biff_records/Tick.h"
#include "../Biff_records/FontX.h"
#include "../Biff_records/AxisLine.h"
#include "../Biff_records/LineFormat.h"
#include "../Biff_records/ShapePropsStream.h"
#include "../Biff_records/TextPropsStream.h"
#include "../Biff_records/PlotArea.h"
#include "../Biff_records/DropBar.h"
#include "../Biff_records/CrtMlFrt.h"
#include "../Biff_records/DataFormat.h"

#include "../Biff_records/StartObject.h"
#include "../Biff_records/EndObject.h"
namespace XLS
{


CRT::CRT()
{
	m_indAXISPARENT = 0; 
}


CRT::~CRT()
{
}

//  (BopPop [BopPopCustom])
class Parenthesis_CRT_3: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CRT_3)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CRT_3(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<BopPop>())
		{
			return false;
		}
		proc.optional<BopPopCustom>();
		return true;
	};
};



//  (Bar / Line / (BopPop [BopPopCustom]) / Pie / Area / Scatter / Radar / RadarArea / Surf)
class Parenthesis_CRT_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CRT_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CRT_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return	proc.optional<Bar>() ||
				proc.optional<Line>() ||				
				proc.optional<Parenthesis_CRT_3>() ||
				proc.optional<Pie>() ||
				proc.optional<Area>() ||
				proc.optional<Scatter>() ||
				proc.optional<Radar>() ||
				proc.optional<RadarArea>() ||
				proc.optional<Surf>();
	};
};



//  (CrtLine LineFormat)
class Parenthesis_CRT_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CRT_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CRT_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<CrtLine>())
		{
			return false;
		}
		proc.mandatory<LineFormat>();
		return true;
	};
};



BaseObjectPtr CRT::clone()
{
	return BaseObjectPtr(new CRT(*this));
}


/*
CRT = ChartFormat Begin 
	(Bar / Line / (BopPop [BopPopCustom]) / Pie / Area / Scatter / Radar / RadarArea / Surf)
	CrtLink [SeriesList] [Chart3d] [LD] [2DROPBAR] *4(CrtLine LineFormat) *2DFTTEXT 
	[DataLabExtContents] [SS] *4SHAPEPROPS End
*/
const bool CRT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<ChartFormat>())
	{
		return false;
	}	

	m_ChartFormat = elements_.back();
	elements_.pop_back(); 

	ChartFormat* fmt = dynamic_cast<ChartFormat*>(m_ChartFormat.get());
	m_indexCrt = fmt->icrt;

	if (proc.optional<Begin>())	elements_.pop_back();

	proc.mandatory<Parenthesis_CRT_1>();

	m_ChartType = elements_.front();
	elements_.pop_front();

	if (elements_.size() > 0 && "BopPop" == m_ChartType->getClassName())
	{
		BopPop *bp = dynamic_cast<BopPop*>(m_ChartType.get());
		bp->m_Custom = elements_.front();
		elements_.pop_front();
	}
	
	if (proc.optional<CrtLink>())
	{
		m_CrtLink = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<StartObject>())
	{
		elements_.pop_back();
		if (proc.optional<EndObject>())
			elements_.pop_back();
	}
	if (proc.optional<SeriesList>())
	{
		m_SeriesList = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<Chart3d>())
	{
		m_Chart3d = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<LD>())
	{
		m_LD = elements_.back();
		elements_.pop_back();
	}
	
	if(proc.optional<DROPBAR>())
	{
		m_DROPBAR[0] = elements_.back();
		elements_.pop_back();

		proc.mandatory<DROPBAR>();
		m_DROPBAR[1] = elements_.back();
		elements_.pop_back();
	}
	
	int count = proc.repeated<Parenthesis_CRT_2>(0, 4);
	while(count > 0)
	{
		if (typeCrtLine == elements_.front()->get_type())
		{	
			m_arCrtLine.push_back(elements_.front());
			elements_.pop_front(); 
		}
		if (typeLineFormat == elements_.front()->get_type())
		{
			CrtLine * crt_line = dynamic_cast<CrtLine *>(m_arCrtLine.back().get());
			crt_line->m_LineFormat = elements_.front();
		} 
		elements_.pop_front(); count--;
	}

	count = proc.repeated<DFTTEXT>(0, 2);
	while(count > 0)
	{
		m_arDFTTEXT.push_back(elements_.front());
		elements_.pop_front(); count--;
	}

	if (proc.optional<DataLabExtContents>())
	{
		m_DataLabExtContents = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<SS>())
	{
		m_SS = elements_.back();
		elements_.pop_back();
	}
	count = proc.repeated<SHAPEPROPS>(0, 4);
	while(count > 0)
	{
		m_arSHAPEPROPS.push_back(elements_.front());
		elements_.pop_front(); count--;
	}
	if (proc.optional<CrtMlFrt>()) // 
	{
		elements_.pop_back();
	}
	
	if (proc.optional<End>())	elements_.pop_back();
//-------------------------------------------------------------------------------------
	m_bIs3D = false;
	if (m_Chart3d)
	{
		m_bIs3D = true;
	}

	m_iChartType = CHART_TYPE_Bar;

	switch(m_ChartType->get_type())
	{
		case typeBar:		m_iChartType = CHART_TYPE_Bar;			break;
		case typeLine:		m_iChartType = CHART_TYPE_Line;			break;
		case typePie:		m_iChartType = CHART_TYPE_Pie;			break;
		case typeArea:		m_iChartType = CHART_TYPE_Area;			break;
		case typeSurf:		m_iChartType = CHART_TYPE_Surf;			break;		
		case typeRadar:		m_iChartType = CHART_TYPE_Radar;		break;
		case typeRadarArea:	m_iChartType = CHART_TYPE_RadarArea;	break;
		case typeBopPop:	m_iChartType = CHART_TYPE_BopPop;		break;		
		case typeScatter:	m_iChartType = CHART_TYPE_Scatter;		break;
		default:
			break;
	}
	
	return true;
}

std::wstring CRT::getOoxChartType()
{
	switch(m_ChartType->get_type())
	{
	case typeBar:		return (m_bIs3D ? L"c:bar3DChart"		: L"c:barChart");
	case typeLine:
	{
		if (m_bIs3D) return  L"c:line3DChart";

		DROPBAR* Dr1 = dynamic_cast<DROPBAR*>(m_DROPBAR[0].get());
		DROPBAR* Dr2 = dynamic_cast<DROPBAR*>(m_DROPBAR[1].get());
		if (Dr1 || Dr2)
		{
			DropBar  *dropBar = NULL;
			if		(Dr1) dropBar = dynamic_cast<DropBar*>(Dr1->m_DropBar.get()); 
			else if (Dr2) dropBar = dynamic_cast<DropBar*>(Dr2->m_DropBar.get());

			if ((dropBar) && (dropBar->pcGap != (short)1)) 
			{
				m_iChartType = CHART_TYPE_Stock;
				return L"c:stockChart";
			}
		}
		return L"c:lineChart";
	}
	case typePie:	
	{
		Pie * p = dynamic_cast<Pie *>(m_ChartType.get());
		if ( p->pcDonut > 0)
		{
			m_iChartType = CHART_TYPE_Doughnut;
						return L"c:doughnutChart";
		}
		else 			return (m_bIs3D ? L"c:pie3DChart"		: L"c:pieChart");
	}break;
	case typeArea:		return (m_bIs3D ? L"c:area3DChart"		: L"c:areaChart");
	case typeSurf:		
	{
		ChartFormat *format	= dynamic_cast<ChartFormat*>(m_ChartFormat.get());
		format->fVaried = true;
		
		Surf* surf = dynamic_cast<Surf *>(m_ChartType.get());
		return ((m_bIs3D && surf->f3DPhongShade == true) ? L"c:surface3DChart"	: L"c:surfaceChart");
	}break;
	case typeScatter:
	{
		Scatter * sc = dynamic_cast<Scatter *>(m_ChartType.get());
		if (sc->fBubbles)
		{
			m_iChartType = CHART_TYPE_Bubble;
			return L"c:bubbleChart";
		}
		else			return L"c:scatterChart";
	}break;
	case typeRadar:		return L"c:radarChart";
	case typeRadarArea:	return L"c:radarChart";
	case typeBopPop:	return L"c:ofPieChart";		
	}
	return L"c:barChart";

}
} // namespace XLS

