
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


SS::SS()
{
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
	proc.mandatory<Begin>();
	proc.optional<Chart3DBarShape>();
	if(proc.optional<LineFormat>())
	{
		//proc.mandatory<AreaFormat>();
		//proc.mandatory<PieFormat>();

		// fix
		proc.optional<AreaFormat>();
		proc.optional<PieFormat>();
	}
	proc.optional<SerFmt>();
	proc.optional<GELFRAME>();
	proc.optional<MarkerFormat>();
	proc.optional<AttachedLabel>();
	proc.repeated<SHAPEPROPS>(0, 2);
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

