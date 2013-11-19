#include "stdafx.h"
#include "Pos.h"

namespace XLS
{;

Pos::Pos()
{
}


Pos::~Pos()
{
}


BaseObjectPtr Pos::clone()
{
	return BaseObjectPtr(new Pos(*this));
}


void Pos::writeFields(CFRecord& record)
{
	record << mdTopLt << mdBotRt;
	record << x1;
	record.reserveNBytes(2); // unused1
	record << y1;
	record.reserveNBytes(2); // unused2
	record << x2;
	record.reserveNBytes(2); // unused3
	record << y2;
	record.reserveNBytes(2); // unused4
}


void Pos::readFields(CFRecord& record)
{
	record >> mdTopLt >> mdBotRt;
	record >> x1;
	record.skipNBytes(2); // unused1
	record >> y1;
	record.skipNBytes(2); // unused2
	record >> x2;
	record.skipNBytes(2); // unused3
	record >> y2;
	record.skipNBytes(2); // unused4
}

} // namespace XLS

