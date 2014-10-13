#include "precompiled_xls.h"
#include "ExtSST.h"

namespace XLS
{;

ExtSST::ExtSST()
{
}


ExtSST::~ExtSST()
{
}


BaseObjectPtr ExtSST::clone()
{
	return BaseObjectPtr(new ExtSST(*this));
}


void ExtSST::writeFields(CFRecord& record)
{
	record << dsst;

	for(size_t i = 0; i < num_sets; ++i)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(unsigned int)/*ISSTInf::ib*/);
		record.registerDelayedDataReceiver(NULL, sizeof(unsigned __int16)/*ISSTInf::cbOffset*/);
		record.reserveNunBytes(2);
	}
}


void ExtSST::readFields(CFRecord& record)
{
	record >> dsst;
	while(!record.isEOF())
	{
		ISSTInfPtr element(new ISSTInf);
		record >> *element;
		rgISSTInf.push_back(element);
	}
}

} // namespace XLS

