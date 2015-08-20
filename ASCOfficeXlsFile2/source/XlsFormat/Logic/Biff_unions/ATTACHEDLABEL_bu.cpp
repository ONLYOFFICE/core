
#include "ATTACHEDLABEL.h"
#include <Logic/Biff_records/Text.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Pos.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_records/AlRuns.h>
#include <Logic/Biff_records/ObjectLink.h>
#include <Logic/Biff_records/DataLabExtContents.h>
#include <Logic/Biff_records/CrtLayout12.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_unions/AI.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_unions/TEXTPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>

namespace XLS
{


ATTACHEDLABEL::ATTACHEDLABEL()
{
}


ATTACHEDLABEL::~ATTACHEDLABEL()
{
}


BaseObjectPtr ATTACHEDLABEL::clone()
{
	return BaseObjectPtr(new ATTACHEDLABEL(*this));
}


/*
ATTACHEDLABEL = Text Begin Pos [FontX] [AlRuns] AI [FRAME] [ObjectLink] [DataLabExtContents] 
				[CrtLayout12] [TEXTPROPS] [CRTMLFRT] End
*/
const bool ATTACHEDLABEL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Text>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	//proc.mandatory<Pos>();

	// fix
	proc.optional<Pos>();

	proc.optional<FontX>();
	proc.optional<AlRuns>();
	proc.mandatory<AI>();
	proc.optional<FRAME>();
	proc.optional<ObjectLink>();
	proc.optional<DataLabExtContents>();
	proc.optional<CrtLayout12>();
	proc.optional<TEXTPROPS>();
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

