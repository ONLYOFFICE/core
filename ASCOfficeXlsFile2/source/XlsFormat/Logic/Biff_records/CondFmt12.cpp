
#include "CondFmt12.h"

namespace XLS
{

CondFmt12::CondFmt12()
{
}


CondFmt12::~CondFmt12()
{
}


BaseObjectPtr CondFmt12::clone()
{
	return BaseObjectPtr(new CondFmt12(*this));
}


void CondFmt12::writeFields(CFRecord& record)
{

}


void CondFmt12::readFields(CFRecord& record)
{
	record >> frtRefHeaderU;
	record >> mainCF;
}


const CellRef CondFmt12::getLocation() const
{
	return mainCF.getLocation();
}


} // namespace XLS

