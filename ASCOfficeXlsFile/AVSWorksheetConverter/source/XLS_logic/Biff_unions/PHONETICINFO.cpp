#include "stdafx.h"
#include "PHONETICINFO.h"
#include <XLS_logic/Biff_records/PhoneticInfo.h>
#include <XLS_logic/Biff_records/Continue.h>

namespace XLS
{;


PHONETICINFO::PHONETICINFO()
{
}


PHONETICINFO::~PHONETICINFO()
{
}


BaseObjectPtr PHONETICINFO::clone()
{
	return BaseObjectPtr(new PHONETICINFO(*this));
}


// PHONETICINFO = PhoneticInfo *Continue
const bool PHONETICINFO::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<PhoneticInfo>())
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

