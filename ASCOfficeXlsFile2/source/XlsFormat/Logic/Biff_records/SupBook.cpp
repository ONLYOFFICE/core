#include "precompiled_xls.h"
#include "SupBook.h"

namespace XLS
{;

SupBook::SupBook()
{
}


SupBook::~SupBook()
{
}


BaseObjectPtr SupBook::clone()
{
	return BaseObjectPtr(new SupBook(*this));
}


void SupBook::writeFields(CFRecord& record)
{
	record << ctab << cch;
	if(0x0001 <= cch && 0x00ff >= cch)
	{
		virtPath.setSize(cch);
		record << virtPath;
		//if(virtPath.isConformToVirtPath() && !virtPath.isConformToOleLink())
		{
			record << rgst;
		}
	}
}


void SupBook::readFields(CFRecord& record)
{
	record >> ctab >> cch;
	if(0x0001 <= cch && 0x00ff >= cch)
	{
		virtPath.setSize(cch);
		record >> virtPath;
		virtPath.EscapeUrlW();
		//if(virtPath.isConformToVirtPath() && !virtPath.isConformToOleLink())
		{
			record >> rgst;
		}
	}
}


const unsigned __int16 SupBook::getSupportingLinkType() const
{
	return cch;
}


} // namespace XLS

