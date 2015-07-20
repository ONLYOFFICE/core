
#include "XFExt.h"

namespace XLS
{;

XFExt::XFExt()
{
}


XFExt::~XFExt()
{
}


BaseObjectPtr XFExt::clone()
{
	return BaseObjectPtr(new XFExt(*this));
}


void XFExt::writeFields(CFRecord& record)
{
#pragma message("####################### XFExt record is not implemented")
	Log::error("XFExt record is not implemented.");
	//record << some_value;
}


void XFExt::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record.skipNunBytes(2); // reserved
	record >> ixfe;
	record.skipNunBytes(2); // reserved
	record >> cexts;
	while(!record.isEOF())
	{
		ExtPropPtr element(new ExtProp);
		record >> *element;
		rgExt.push_back(element);
	}
}

} // namespace XLS

