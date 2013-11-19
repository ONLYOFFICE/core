#include "stdafx.h"
#include "TEXTPROPS.h"
#include <XLS_logic/Biff_records/RichTextStream.h>
#include <XLS_logic/Biff_records/TextPropsStream.h>
#include <XLS_logic/Biff_records/ContinueFrt12.h>

//#include <XLS_logic/Biff_unions/PAGESETUP.h>

namespace XLS
{;


TEXTPROPS::TEXTPROPS()
{
}


TEXTPROPS::~TEXTPROPS()
{
}


BaseObjectPtr TEXTPROPS::clone()
{
	return BaseObjectPtr(new TEXTPROPS(*this));
}


// TEXTPROPS = (RichTextStream / TextPropsStream) *ContinueFrt12
const bool TEXTPROPS::loadContent(BinProcessor& proc)
{
	if(!proc.optional<RichTextStream>() && !proc.optional<TextPropsStream>())
	{
		return false;
	}
	proc.repeated<ContinueFrt12>(0, 0);

	return true;
}

} // namespace XLS

