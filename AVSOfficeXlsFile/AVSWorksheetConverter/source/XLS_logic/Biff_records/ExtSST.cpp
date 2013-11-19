#include "stdafx.h"
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
		record.registerDelayedDataReceiver(NULL, sizeof(DWORD)/*ISSTInf::ib*/);
		record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*ISSTInf::cbOffset*/);
		record.reserveNBytes(2);
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

