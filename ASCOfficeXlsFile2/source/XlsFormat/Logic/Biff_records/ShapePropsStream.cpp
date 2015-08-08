
#include "ShapePropsStream.h"

namespace XLS
{;

ShapePropsStream::ShapePropsStream()
{
}


ShapePropsStream::~ShapePropsStream()
{
}


BaseObjectPtr ShapePropsStream::clone()
{
	return BaseObjectPtr(new ShapePropsStream(*this));
}


void ShapePropsStream::writeFields(CFRecord& record)
{
#pragma message("####################### ShapePropsStream record is not implemented")
	Log::error("ShapePropsStream record is not implemented.");
	//record << some_value;
}


void ShapePropsStream::readFields(CFRecord& record)
{
	record >> frtHeader;
	record >> wObjContext;

	record.skipNunBytes(2);

	record >> dwChecksum;
	
	unsigned int	cb=0;
	record >> cb;

	if (cb > 0)
	{
		xml_ = std::string( record.getCurData<char>(), cb);
		record.skipNunBytes(cb);
	}
}

} // namespace XLS

