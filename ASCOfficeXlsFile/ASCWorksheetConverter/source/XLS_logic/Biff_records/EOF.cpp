#include "stdafx.h"
#include "EOF.h"

namespace XLS
{;


EOF_T::EOF_T()
{
}


EOF_T::~EOF_T()
{
}


BaseObjectPtr EOF_T::clone()
{
	return BaseObjectPtr(new EOF_T(*this));
}


void EOF_T::writeFields(CFRecord& record)
{
	// No data in this record
}


void EOF_T::readFields(CFRecord& record)
{
	// No data in this record
}


} // namespace XLS

