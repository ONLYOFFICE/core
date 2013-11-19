#include "stdafx.h"
#include "CHARTFOMATS.h"
#include <XLS_logic/Biff_records/Chart.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/Scl.h>
#include <XLS_logic/Biff_records/PlotGrowth.h>
#include <XLS_logic/Biff_records/ShtProps.h>
#include <XLS_logic/Biff_records/AxesUsed.h>
#include <XLS_logic/Biff_records/CrtLayout12A.h>
#include <XLS_logic/Biff_records/DataLabExt.h>
#include <XLS_logic/Biff_records/StartObject.h>
#include <XLS_logic/Biff_records/EndObject.h>
#include <XLS_logic/Biff_records/End.h>

#include <XLS_logic/Biff_unions/FONTLIST.h>
#include <XLS_logic/Biff_unions/FRAME.h>
#include <XLS_logic/Biff_unions/SERIESFORMAT.h>
#include <XLS_logic/Biff_unions/SS.h>
#include <XLS_logic/Biff_unions/DFTTEXT.h>
#include <XLS_logic/Biff_unions/AXISPARENT.h>
#include <XLS_logic/Biff_unions/DAT.h>
#include <XLS_logic/Biff_unions/ATTACHEDLABEL.h>
#include <XLS_logic/Biff_unions/CRTMLFRT.h>
#include <XLS_logic/Biff_unions/ATTACHEDLABEL.h>
#include <XLS_logic/Biff_unions/TEXTPROPS.h>

namespace XLS
{;


CHARTFOMATS::CHARTFOMATS()
{
}


CHARTFOMATS::~CHARTFOMATS()
{
}


//  ([DataLabExt StartObject] ATTACHEDLABEL [EndObject])
class Parenthesis_CHARTFOMATS_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CHARTFOMATS_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CHARTFOMATS_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(proc.optional<DataLabExt>())
		{
			proc.mandatory<StartObject>();
		}
		if(!proc.mandatory<ATTACHEDLABEL>())
		{
			return false;
		}
		proc.optional<EndObject>();
		return true;
	};
};


BaseObjectPtr CHARTFOMATS::clone()
{
	return BaseObjectPtr(new CHARTFOMATS(*this));
}


/*
CHARTFOMATS = Chart Begin *2FONTLIST Scl PlotGrowth [FRAME] *SERIESFORMAT *SS ShtProps *2DFTTEXT 
				AxesUsed 1*2AXISPARENT [CrtLayout12A] [DAT] *ATTACHEDLABEL [CRTMLFRT] 
				*([DataLabExt StartObject] ATTACHEDLABEL [EndObject]) [TEXTPROPS] *2CRTMLFRT End
*/
const bool CHARTFOMATS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Chart>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	proc.repeated<FONTLIST>(0, 2);
	
	// fix
	//proc.mandatory<Scl>();
	//proc.mandatory<PlotGrowth>();

	if (proc.optional<Scl>())
		proc.optional<PlotGrowth>();

	proc.optional<FRAME>();
	proc.repeated<SERIESFORMAT>(0, 0);
	proc.repeated<SS>(0, 0);
	proc.mandatory<ShtProps>();
	proc.repeated<DFTTEXT>(0, 2);
	proc.mandatory<AxesUsed>();
	proc.repeated<AXISPARENT>(1, 2);
	proc.optional<CrtLayout12A>();
	proc.optional<DAT>();
	proc.repeated<ATTACHEDLABEL>(0, 0);
	proc.optional<CRTMLFRT>();
	proc.repeated<Parenthesis_CHARTFOMATS_1>(0, 0);
	proc.optional<TEXTPROPS>();
	proc.repeated<CRTMLFRT>(0, 2);
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

