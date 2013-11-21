#include "stdafx.h"
#include "SS.h"
#include <XLS_logic/Biff_records/DataFormat.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/Chart3DBarShape.h>
#include <XLS_logic/Biff_records/LineFormat.h>
#include <XLS_logic/Biff_records/AreaFormat.h>
#include <XLS_logic/Biff_records/PieFormat.h>
#include <XLS_logic/Biff_records/SerFmt.h>
#include <XLS_logic/Biff_records/MarkerFormat.h>
#include <XLS_logic/Biff_records/AttachedLabel.h>
#include <XLS_logic/Biff_records/End.h>

#include <XLS_logic/Biff_unions/GELFRAME.h>
#include <XLS_logic/Biff_unions/SHAPEPROPS.h>
#include <XLS_logic/Biff_unions/CRTMLFRT.h>

namespace XLS
{;


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

