#include "stdafx.h"
#include "TxO.h"

namespace XLS
{;

TxO::TxO()
{
}


TxO::~TxO()
{
}


BaseObjectPtr TxO::clone()
{
	return BaseObjectPtr(new TxO(*this));
}


void TxO::writeFields(CFRecord& record)
{
#pragma message("####################### TxO record is not implemented")
	Log::error("TxO record is not implemented.");
	//record << some_value;
}


void TxO::readFields(CFRecord& record)
{
#pragma message("####################### TxO record is not implemented")
	Log::error("TxO record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

