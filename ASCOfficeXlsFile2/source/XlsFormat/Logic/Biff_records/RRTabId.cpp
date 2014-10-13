#include "precompiled_xls.h"
#include "RRTabId.h"

namespace XLS
{;

RRTabId::RRTabId()
{
}


RRTabId::~RRTabId()
{
}


BaseObjectPtr RRTabId::clone()
{
	return BaseObjectPtr(new RRTabId(*this));
}


void RRTabId::writeFields(CFRecord& record)
{
	for(BiffStructurePtrVector::iterator it = sheet_ids.begin(), itEnd = sheet_ids.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void RRTabId::readFields(CFRecord& record)
{
	while(!record.isEOF())
	{
		SheetIdPtr element(new SheetId);
		record >> *element;
		sheet_ids.push_back(element);
	}
}

} // namespace XLS

