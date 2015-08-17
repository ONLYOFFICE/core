
#include "Frame.h"

namespace XLS
{

Frame::Frame()
{
}


Frame::~Frame()
{
}


BaseObjectPtr Frame::clone()
{
	return BaseObjectPtr(new Frame(*this));
}


void Frame::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAutoSize);
	SETBIT(flags, 1, fAutoPosition);
	record << frt << flags;
}


void Frame::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> frt >> flags;
	fAutoSize = GETBIT(flags, 0);
	fAutoPosition = GETBIT(flags, 1);
}

} // namespace XLS

