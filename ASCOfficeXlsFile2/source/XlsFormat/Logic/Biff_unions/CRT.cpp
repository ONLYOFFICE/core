
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
{;


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

	proc.mandatory<Begin>();	
	proc.mandatory<Parenthesis_CRT_1>();
	
	//proc.mandatory<CrtLink>();

	// fix
	proc.optional<CrtLink>();

	proc.optional<SeriesList>();
	proc.optional<Chart3d>();
	proc.optional<LD>();
	if(proc.optional<DROPBAR>())
	{
		proc.mandatory<DROPBAR>();
	}
	proc.repeated<Parenthesis_CRT_2>(0, 4);
	proc.repeated<DFTTEXT>(0, 2);
	proc.optional<DataLabExtContents>();
	proc.optional<SS>();
	proc.repeated<SHAPEPROPS>(0, 4);
	proc.mandatory<End>();


	return true;
}

} // namespace XLS

