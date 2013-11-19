#include "stdafx.h"
#include "FORMATTING.h"
#include <XLS_logic/Biff_records/Font.h>
#include <XLS_logic/Biff_records/Format.h>
#include <XLS_logic/Biff_unions/XFS.h>
#include <XLS_logic/Biff_records/DXF.h>
#include <XLS_logic/Biff_unions/STYLES.h>
#include <XLS_logic/Biff_records/Palette.h>
#include <XLS_logic/Biff_records/ClrtClient.h>
#include <XLS_logic/Biff_records/TableStyles.h>
#include <XLS_logic/Biff_unions/TABLESTYLES.h>

namespace XLS
{;


FORMATTING::FORMATTING()
{
}


FORMATTING::~FORMATTING()
{
}


BaseObjectPtr FORMATTING::clone()
{
	return BaseObjectPtr(new FORMATTING(*this));
}


//FORMATTING = 1*510Font 8*218Format XFS *DXF STYLES [TABLESTYLES] [Palette] [ClrtClient]
const bool FORMATTING::loadContent(BinProcessor& proc)
{
	proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally  at least one Font is mandatory)
	proc.repeated<Format>(0, 218); // Originally: proc.repeated<Format>(8, 218);
	proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally Font follows by Format)
	proc.mandatory<XFS>();
	proc.repeated<DXF>(0, 0);
	proc.mandatory<STYLES>();
	proc.optional<TABLESTYLES>();
	proc.optional<Palette>();
	proc.optional<ClrtClient>();

	return true;
}

} // namespace XLS

