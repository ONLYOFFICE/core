#include "precompiled_xls.h"
#include "BuiltInFnGroupCount.h"

namespace XLS
{;

BuiltInFnGroupCount::BuiltInFnGroupCount()
{
}


BuiltInFnGroupCount::~BuiltInFnGroupCount()
{
}


BaseObjectPtr BuiltInFnGroupCount::clone()
{
	return BaseObjectPtr(new BuiltInFnGroupCount(*this));
}


void BuiltInFnGroupCount::writeFields(CFRecord& record)
{
	record << count;
}


void BuiltInFnGroupCount::readFields(CFRecord& record)
{
	record >> count;
}

} // namespace XLS

