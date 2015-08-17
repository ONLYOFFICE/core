
#include "LD.h"
#include <Logic/Biff_records/Legend.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Pos.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_records/CrtLayout12.h>
#include <Logic/Biff_unions/TEXTPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


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

