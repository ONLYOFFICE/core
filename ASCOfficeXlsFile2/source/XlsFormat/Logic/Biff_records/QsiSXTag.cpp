#include "precompiled_xls.h"
#include "QsiSXTag.h"

namespace XLS
{;

QsiSXTag::QsiSXTag()
{
}


QsiSXTag::~QsiSXTag()
{
}


BaseObjectPtr QsiSXTag::clone()
{
	return BaseObjectPtr(new QsiSXTag(*this));
}


void QsiSXTag::writeFields(CFRecord& record)
{
#pragma message("####################### QsiSXTag record is not implemented")
	Log::error("QsiSXTag record is not implemented.");
	//record << some_value;
}


void QsiSXTag::readFields(CFRecord& record)
{
#pragma message("####################### QsiSXTag record is not implemented")
	Log::error("QsiSXTag record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

