#include "precompiled_xls.h"
#include "GUIDTypeLib.h"

namespace XLS
{;

GUIDTypeLib::GUIDTypeLib()
{
}


GUIDTypeLib::~GUIDTypeLib()
{
}


BaseObjectPtr GUIDTypeLib::clone()
{
	return BaseObjectPtr(new GUIDTypeLib(*this));
}


void GUIDTypeLib::writeFields(CFRecord& record)
{
#pragma message("####################### GUIDTypeLib record is not implemented")
	Log::error("GUIDTypeLib record is not implemented.");
	//record << some_value;
}


void GUIDTypeLib::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	_GUID_ guid_num;
	record >> guid_num;
	guid = STR::guid2bstr(guid_num);
}

} // namespace XLS

