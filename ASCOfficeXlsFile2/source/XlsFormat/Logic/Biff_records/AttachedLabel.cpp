//
#include "AttachedLabel.h"

namespace XLS
{

AttachedLabel::AttachedLabel()
{
}


AttachedLabel::~AttachedLabel()
{
}


BaseObjectPtr AttachedLabel::clone()
{
	return BaseObjectPtr(new AttachedLabel(*this));
}


void AttachedLabel::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fShowValue);
	SETBIT(flags, 1, fShowPercent);
	SETBIT(flags, 2, fShowLabelAndPerc);
	SETBIT(flags, 4, fShowLabel);
	SETBIT(flags, 5, fShowBubbleSizes);
	SETBIT(flags, 6, fShowSeriesName);
	record << flags;
}


void AttachedLabel::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fShowValue = GETBIT(flags, 0);
	fShowPercent = GETBIT(flags, 1);
	fShowLabelAndPerc = GETBIT(flags, 2);
	fShowLabel = GETBIT(flags, 4);
	fShowBubbleSizes = GETBIT(flags, 5);
	fShowSeriesName = GETBIT(flags, 6);
}

} // namespace XLS

