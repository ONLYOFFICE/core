#include "stdafx.h"
#include "CRN.h"
#include <XLS_logic/Biff_structures/SerAr.h>

namespace XLS
{;

CRN::CRN()
{
}


CRN::~CRN()
{
}


BaseObjectPtr CRN::clone()
{
	return BaseObjectPtr(new CRN(*this));
}


void CRN::writeFields(CFRecord& record)
{
#pragma message("####################### CRN record is not implemented")
	Log::error("CRN record is not implemented.");
	//record << some_value;
}


void CRN::readFields(CFRecord& record)
{
	record >> colLast >> colFirst >> row;
	for(int i = 0; i < colLast - colFirst + 1; ++i)
	{
		BYTE rec_type;
		record >> rec_type;
		SerArPtr ser(SerAr::createSerAr(rec_type));
		record >> *ser;
		crnOper.push_back(ser);
	}
}

} // namespace XLS

