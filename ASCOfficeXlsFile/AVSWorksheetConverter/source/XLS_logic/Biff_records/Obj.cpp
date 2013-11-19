#include "stdafx.h"
#include "Obj.h"

namespace XLS
{;

Obj::Obj()
{
}


Obj::~Obj()
{
}


BaseObjectPtr Obj::clone()
{
	return BaseObjectPtr(new Obj(*this));
}


void Obj::writeFields(CFRecord& record)
{
	record << cmo;
	if(0 == cmo.ot)
	{
		// FtGmo
		WORD ft = 0x06;
		WORD cb = 0x02;
		record << ft << cb; // reserved
		record.reserveNBytes(2); // unused
	}
	if(0x08 == cmo.ot)
	{
		record << pictFormat << pictFlags;
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		// FtCbls
		WORD ft = 0x0A;
		WORD cb = 0x0C;
		record << ft << cb; // reserved
		record.reserveNBytes(cb); // unused
	}
	if(0x0C == cmo.ot)
	{
		// FtRbo
		WORD ft = 0x0B;
		WORD cb = 0x06;
		record << ft << cb; // reserved
		record.reserveNBytes(cb); // unused
	}
	if(0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		record << sbs;
	}
	if(0x19 == cmo.ot)
	{
		record << nts;
	}
	if(false) // TODO: Find out the condition
	{
		macro.store(record);
	}
	if(0x08 != cmo.ot)
	{
		pictFmla.store(record, pictFlags);
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot || 0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		linkFmla.store(record, cmo.ot);
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		checkBox.store(record);
	}
	if(0x0C == cmo.ot)
	{
		radioButton.store(record);
	}
	if(0x0D == cmo.ot)
	{
		edit.store(record);
	}
	if(0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		list.store(record, cmo.ot);
	}
	if(0x13 == cmo.ot)
	{
		gbo.store(record);
	}
	if(0x12 != cmo.ot && 0x14 != cmo.ot)
	{
		record.reserveNBytes(4); // reserved
	}
}


void Obj::readFields(CFRecord& record)
{
	record >> cmo;
	size_t rdPtr = record.getRdPtr();
	if(0 == cmo.ot)
	{
		record.skipNBytes(6); // Skip FtGmo (obsolete)
	}
	if(0x08 == cmo.ot) // picture !!!
	{
		record >> pictFormat;
		record >> pictFlags;
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		record.skipNBytes(16); // Skip FtCbls (obsolete)
	}
	if(0x0C == cmo.ot)
	{
		record.skipNBytes(10); // Skip FtRbo (obsolete)
	}
	if(0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		record >> sbs;
	}
	if(0x19 == cmo.ot)
	{
		record >> nts;
	}
	if(false) // TODO: Find out the condition
	{
		macro.load(record);
	}
	if(0x08 == cmo.ot)
	{
		//pictFmla.load(record, pictFlags);  // picture !!! This field MUST NOT exist unless cmo.ot is equal to 0x08.
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot || 0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot /*|| 0x14 == cmo.ot*/)
	{
		linkFmla.load(record);
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		checkBox.load(record);
	}
	if(0x0C == cmo.ot)
	{
		radioButton.load(record);
	}
	if(0x0D == cmo.ot)
	{
		edit.load(record);
	}
	if(0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		list.load(record, cmo.ot);
	}
	if(0x13 == cmo.ot)
	{
		gbo.load(record);
	}
	if(0x12 != cmo.ot && 0x14 != cmo.ot)
	{
		record.skipNBytes(4); // reserved
	}
}

} // namespace XLS

