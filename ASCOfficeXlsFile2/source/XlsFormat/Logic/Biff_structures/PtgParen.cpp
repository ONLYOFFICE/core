
#include "PtgParen.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgParen::PtgParen()
:	Ptg(fixed_id),
	num_parameters(0)
{
}


BiffStructurePtr PtgParen::clone()
{
	return BiffStructurePtr(new PtgParen(*this));
}


void PtgParen::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	ptg_stack.top() = L'(' + ptg_stack.top() + L')';
}


void PtgParen::incrementParametersNum()
{
	++num_parameters;
}


void PtgParen::decrementParametersNum()
{
	--num_parameters;
}


const size_t PtgParen::getParametersNum() const
{
	return num_parameters;
}


} // namespace XLS

