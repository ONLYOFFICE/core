#include "stdafx.h"
#include "LD.h"
#include <XLS_logic/Biff_records/Legend.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/Pos.h>
#include <XLS_logic/Biff_unions/ATTACHEDLABEL.h>
#include <XLS_logic/Biff_unions/FRAME.h>
#include <XLS_logic/Biff_records/CrtLayout12.h>
#include <XLS_logic/Biff_unions/TEXTPROPS.h>
#include <XLS_logic/Biff_unions/CRTMLFRT.h>
#include <XLS_logic/Biff_records/End.h>

namespace XLS
{;


LD::LD()
{
}


LD::~LD()
{
}


BaseObjectPtr LD::clone()
{
	return BaseObjectPtr(new LD(*this));
}


// LD = Legend Begin Pos ATTACHEDLABEL [FRAME] [CrtLayout12] [TEXTPROPS] [CRTMLFRT] CRTMLFRT
const bool LD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Legend>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	
	//proc.mandatory<Pos>();

	// fix
	proc.optional<Pos>();

	proc.mandatory<ATTACHEDLABEL>();
	proc.optional<FRAME>();
	proc.optional<CrtLayout12>();
	proc.optional<TEXTPROPS>();
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();
	return true;
}

} // namespace XLS

