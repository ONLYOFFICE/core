#include "stdafx.h"
#include "PROTECTION.h"
#include <XLS_logic/Biff_records/WinProtect.h>
#include <XLS_logic/Biff_records/Protect.h>
#include <XLS_logic/Biff_records/Password.h>
#include <XLS_logic/Biff_records/Prot4Rev.h>
#include <XLS_logic/Biff_records/Prot4RevPass.h>

namespace XLS
{;


PROTECTION::PROTECTION()
{
}


PROTECTION::~PROTECTION()
{
}


BaseObjectPtr PROTECTION::clone()
{
	return BaseObjectPtr(new PROTECTION(*this));
}


// PROTECTION = WinProtect Protect Password Prot4Rev Prot4RevPass
const bool PROTECTION::loadContent(BinProcessor& proc)
{
// 	if(!proc.mandatory<WinProtect>())
// 	{
// 		return false;
// 	}
	proc.optional<WinProtect>(); // OpenOffice Calc stored files workaround (WinProtect is mandatory according to [MS-XLS])
	proc.optional<Protect>(); // OpenOffice Calc stored files workaround (Protect is mandatory according to [MS-XLS])
	proc.optional<Password>(); // OpenOffice Calc stored files workaround (Password is mandatory according to [MS-XLS])
	proc.optional<Prot4Rev>(); // OpenOffice Calc stored files workaround (Prot4Rev is mandatory according to [MS-XLS])
	proc.optional<Prot4RevPass>(); // OpenOffice Calc stored files workaround (Prot4RevPass is mandatory according to [MS-XLS])
	return true;
}

} // namespace XLS

