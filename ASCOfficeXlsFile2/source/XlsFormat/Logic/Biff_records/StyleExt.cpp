
#include "StyleExt.h"

#include <simple_xml_writer.h>

namespace XLS
{

StyleExt::StyleExt()
{
}


StyleExt::~StyleExt()
{
}

BaseObjectPtr StyleExt::clone()
{
	return BaseObjectPtr(new StyleExt(*this));
}


void StyleExt::writeFields(CFRecord& record)
{
#pragma message("####################### StyleExt record is not implemented")
	Log::error("StyleExt record is not implemented.");
	//record << some_value;
}


void StyleExt::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	unsigned char flags;
	record >> flags;
	fBuiltIn = GETBIT(flags, 0);
	fHidden = GETBIT(flags, 1);
	fCustom = GETBIT(flags, 2);

	record >> iCategory>> builtInData >> stName >> xfProps;
}

int StyleExt::serialize(std::wostream & stream)
{

	return 0;
}

} // namespace XLS

