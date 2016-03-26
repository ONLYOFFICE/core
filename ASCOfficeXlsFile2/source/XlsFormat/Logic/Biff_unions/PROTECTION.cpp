
#include "PROTECTION.h"
#include <Logic/Biff_records/WinProtect.h>
#include <Logic/Biff_records/Protect.h>
#include <Logic/Biff_records/Password.h>
#include <Logic/Biff_records/Prot4Rev.h>
#include <Logic/Biff_records/Prot4RevPass.h>

namespace XLS
{


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
	proc.optional<WinProtect>(); 
	proc.optional<Protect>(); 
	proc.optional<Password>(); 
	proc.optional<Prot4Rev>(); 
	proc.optional<Prot4RevPass>(); 
	return true;
}

} // namespace XLS

