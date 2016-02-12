
#include "CFEx.h"

namespace XLS
{

CFEx::CFEx()
{
}


CFEx::~CFEx()
{
}


BaseObjectPtr CFEx::clone()
{
	return BaseObjectPtr(new CFEx(*this));
}


void CFEx::writeFields(CFRecord& record)
{

}


void CFEx::readFields(CFRecord& record)
{
	record >> frtRefHeaderU;
	record >> fIsCF12 >> nID;
	if(!fIsCF12)
	{
		record >> rgbContent;
	}
}


const unsigned short CFEx::getID() const
{
	return nID;
}

} // namespace XLS

