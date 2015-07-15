#include "precompiled_xls.h"
#include "FORMATTING.h"
#include <Logic/Biff_records/Font.h>
#include <Logic/Biff_records/Format.h>
#include <Logic/Biff_unions/XFS.h>
#include <Logic/Biff_records/DXF.h>
#include <Logic/Biff_unions/STYLES.h>
#include <Logic/Biff_records/Palette.h>
#include <Logic/Biff_records/ClrtClient.h>
#include <Logic/Biff_records/TableStyles.h>
#include <Logic/Biff_unions/TABLESTYLES.h>

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
	int count = 0;
	proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally  at least one Font is mandatory)
	count = proc.repeated<Format>(0, 218); // Originally: proc.repeated<Format>(8, 218);

	while(count > 0)
	{
		m_Formats.insert(m_Formats.begin(), elements_.back());
		elements_.pop_back();
		count--;

	}
	proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally Font follows by Format)
	
	if (proc.mandatory<XFS>())
	{
		m_XFS = elements_.back();
		elements_.pop_back();
	}
	proc.repeated<DXF>(0, 0);
	
	if (proc.optional<STYLES>())
	{
		m_Styles = elements_.back();
		elements_.pop_back();
	}
	proc.optional<TABLESTYLES>();
	proc.optional<Palette>();
	proc.optional<ClrtClient>();

	return true;
}

} // namespace XLS

