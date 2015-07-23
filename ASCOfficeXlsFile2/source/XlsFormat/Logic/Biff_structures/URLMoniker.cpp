
#include "URLMoniker.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{;


URLMoniker::URLMoniker()
{
}


URLMoniker::URLMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr URLMoniker::clone()
{
	return XLS::BiffStructurePtr(new URLMoniker(*this));
}

void URLMoniker::store(XLS::CFRecord& record)
{
	unsigned int length = (url.length() + 1) * 2;
	record << length << url;
}


void URLMoniker::load(XLS::CFRecord& record)
{
	unsigned int length;
	record >> length;
	try
	{
		record >> url;	
	}catch(...)
	{
	}

	if(!record.isEOF())
	{
		_GUID_ guid;
		record >> guid >> serialVersion >> uriFlags;
		serialGUID = STR::guid2bstr(guid);
	}
}


} // namespace OSHARED

