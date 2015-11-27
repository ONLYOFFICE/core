
#include "CRT.h"
#include <Logic/Biff_records/ChartFormat.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Bar.h>
#include <Logic/Biff_records/Line.h>
#include <Logic/Biff_records/BopPop.h>
#include <Logic/Biff_records/BopPopCustom.h>
#include <Logic/Biff_records/Pie.h>
#include <Logic/Biff_records/Area.h>
#include <Logic/Biff_records/Scatter.h>
#include <Logic/Biff_records/Radar.h>
#include <Logic/Biff_records/RadarArea.h>
#include <Logic/Biff_records/Surf.h>
#include <Logic/Biff_records/CrtLink.h>
#include <Logic/Biff_records/SeriesList.h>
#include <Logic/Biff_records/Chart3d.h>
#include <Logic/Biff_unions/LD.h>
#include <Logic/Biff_unions/DROPBAR.h>
#include <Logic/Biff_records/CrtLine.h>
#include <Logic/Biff_records/LineFormat.h>
#include <Logic/Biff_unions/DFTTEXT.h>
#include <Logic/Biff_records/DataLabExtContents.h>
#include <Logic/Biff_unions/SS.h>
#include <Logic/Biff_unions/SHAPEPROPS.h>
#include <Logic/Biff_records/End.h>
#include <Logic/Biff_records/ValueRange.h>
#include <Logic/Biff_records/Tick.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_records/AxisLine.h>
#include <Logic/Biff_records/LineFormat.h>
#include <Logic/Biff_records/ShapePropsStream.h>
#include <Logic/Biff_records/TextPropsStream.h>
#include <Logic/Biff_records/PlotArea.h>


namespace XLS
{


CRT::CRT()
{
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

	proc.mandatory<Begin>();				elements_.pop_back();
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
		if ("CrtLine" == elements_.front()->getClassName())
		{	
			m_arCrtLine.push_back(elements_.front());
		}
		else if ("LineFormat" == elements_.front()->getClassName())
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
	
	proc.mandatory<End>();					elements_.pop_back();
	return true;
}

} // namespace XLS

