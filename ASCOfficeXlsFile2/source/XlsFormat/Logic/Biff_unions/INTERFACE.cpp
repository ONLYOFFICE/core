
#include "INTERFACE.h"
#include <Logic/Biff_records/InterfaceHdr.h>
#include <Logic/Biff_records/Mms.h>
#include <Logic/Biff_records/InterfaceEnd.h>

namespace XLS
{


INTERFACE_T::INTERFACE_T()
{
}


INTERFACE_T::~INTERFACE_T()
{
}


BaseObjectPtr INTERFACE_T::clone()
{
	return BaseObjectPtr(new INTERFACE_T(*this));
}


// INTERFACE = InterfaceHdr Mms InterfaceEnd
const bool INTERFACE_T::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<InterfaceHdr>())
	{
		return false;
	}
	proc.mandatory<Mms>();
	proc.mandatory<InterfaceEnd>();
	return true;
}


} // namespace XLS
