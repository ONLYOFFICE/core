
#include "Obj.h"
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtDgContainer.h>

namespace XLS
{;

Obj::Obj(MsoDrawingPtr mso_drawing) : mso_drawing_(mso_drawing)
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
		unsigned short ft = 0x06;
		unsigned short cb = 0x02;
		record << ft << cb; // reserved
		record.reserveNunBytes(2); // unused
	}
	if(0x08 == cmo.ot)
	{
		record << pictFormat << pictFlags;
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		// FtCbls
		unsigned short ft = 0x0A;
		unsigned short cb = 0x0C;
		record << ft << cb; // reserved
		record.reserveNunBytes(cb); // unused
	}
	if(0x0C == cmo.ot)
	{
		// FtRbo
		unsigned short ft = 0x0B;
		unsigned short cb = 0x06;
		record << ft << cb; // reserved
		record.reserveNunBytes(cb); // unused
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
		record.reserveNunBytes(4); // reserved
	}
}


void Obj::readFields(CFRecord& record)
{
	record >> cmo;
	size_t rdPtr = record.getRdPtr();
	if(0 == cmo.ot)
	{
		record.skipNunBytes(6); // Skip FtGmo (obsolete)
	}
	if(0x08 == cmo.ot) // picture !!!
	{
		record >> pictFormat;
		record >> pictFlags;
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		record.skipNunBytes(16); // Skip FtCbls (obsolete)
	}
	if(0x0C == cmo.ot)
	{
		record.skipNunBytes(10); // Skip FtRbo (obsolete)
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
		record.skipNunBytes(4); // reserved
	}

	if (continue_records.size() > 0)
	{
		std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

		ODRAW::OfficeArtDgContainer dg(ODRAW::OfficeArtRecord::CA_Sheet);

		while( !recs.empty() )
		{
			//dg.loadFields(*recs.front());
			mso_drawing_->storeRecordAndDecideProceeding(recs.front());
			recs.pop_front();
		}
		
	}
}

} // namespace XLS

