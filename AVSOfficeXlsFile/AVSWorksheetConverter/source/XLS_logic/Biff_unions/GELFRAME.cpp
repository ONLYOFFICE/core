#include "stdafx.h"
#include "GELFRAME.h"
#include <XLS_logic/Biff_records/GelFrame.h>
#include <XLS_logic/Biff_records/Continue.h>

#include <XLS_logic/Biff_unions/PICF.h>

namespace XLS
{;


GELFRAME::GELFRAME()
{
}


GELFRAME::~GELFRAME()
{
}


BaseObjectPtr GELFRAME::clone()
{
	return BaseObjectPtr(new GELFRAME(*this));
}


// GELFRAME = 1*2GelFrame *Continue [PICF]
const bool GELFRAME::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<GelFrame>())
	{
		return false;
	}
	proc.optional<GelFrame>();
	proc.repeated<Continue>(0, 0);
	proc.optional<PICF>();

	return true;
}

} // namespace XLS

