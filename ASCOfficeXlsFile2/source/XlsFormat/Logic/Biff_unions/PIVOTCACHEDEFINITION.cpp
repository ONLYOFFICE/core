
#include "PIVOTCACHEDEFINITION.h"
#include <Logic/Biff_records/SXStreamID.h>
#include <Logic/Biff_records/SXVS.h>
#include <Logic/Biff_unions/SXSRC.h>
#include <Logic/Biff_unions/SXADDLCACHE.h>

namespace XLS
{


PIVOTCACHEDEFINITION::PIVOTCACHEDEFINITION()
{
}


PIVOTCACHEDEFINITION::~PIVOTCACHEDEFINITION()
{
}


BaseObjectPtr PIVOTCACHEDEFINITION::clone()
{
	return BaseObjectPtr(new PIVOTCACHEDEFINITION(*this));
}


// PIVOTCACHEDEFINITION = SXStreamID SXVS [SXSRC] [SXADDLCACHE]
const bool PIVOTCACHEDEFINITION::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXStreamID>())
	{
		return false;
	}
#pragma message("####################### PIVOTCACHEDEFINITION union is not implemented")
	Log::info("PIVOTCACHEDEFINITION union is not implemented!!!!!!!!!!!!!!!");
	proc.mandatory<SXVS>();
	proc.optional<SXSRC>();
	proc.optional<SXADDLCACHE>();
	return true;
}

} // namespace XLS

