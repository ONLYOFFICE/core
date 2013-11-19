#include "stdafx.h"
#include "PROTECTION_COMMON.h"
#include <XLS_logic/Biff_records/Protect.h>
#include <XLS_logic/Biff_records/ScenarioProtect.h>
#include <XLS_logic/Biff_records/ObjProtect.h>
#include <XLS_logic/Biff_records/Password.h>

namespace XLS
{;


PROTECTION_COMMON::PROTECTION_COMMON()
{
}


PROTECTION_COMMON::~PROTECTION_COMMON()
{
}


BaseObjectPtr PROTECTION_COMMON::clone()
{
	return BaseObjectPtr(new PROTECTION_COMMON(*this));
}


// PROTECTION_COMMON = [Protect] [ScenarioProtect] [ObjProtect] [Password]
const bool PROTECTION_COMMON::loadContent(BinProcessor& proc)
{
	bool res1 = proc.optional<Protect>();
	bool res2 = proc.optional<ScenarioProtect>();
	bool res3 = proc.optional<ObjProtect>();
	bool res4 = proc.optional<Password>();

	return res1 || res2 || res3 || res4;
}

} // namespace XLS

