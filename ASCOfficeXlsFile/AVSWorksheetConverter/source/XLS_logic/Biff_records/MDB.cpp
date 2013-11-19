#include "stdafx.h"
#include "MDB.h"

namespace XLS
{;

MDB::MDB()
{
}


MDB::~MDB()
{
}


BaseObjectPtr MDB::clone()
{
	return BaseObjectPtr(new MDB(*this));
}


void MDB::writeFields(CFRecord& record)
{
#pragma message("####################### MDB record is not implemented")
	Log::error("MDB record is not implemented.");
	//record << some_value;
}


void MDB::readFields(CFRecord& record)
{
	record.skipNBytes(12);
#pragma message("############################ frtHeader skipped here")
	while(!record.isEOF())
	{
		MDirPtr element(new MDir);
		record >> *element;
		rgmdir.push_back(element);
	}
}

} // namespace XLS

