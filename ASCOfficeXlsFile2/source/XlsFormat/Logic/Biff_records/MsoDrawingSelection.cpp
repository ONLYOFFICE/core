#include "precompiled_xls.h"
#include "MsoDrawingSelection.h"

namespace XLS
{;

MsoDrawingSelection::MsoDrawingSelection()
{
}


MsoDrawingSelection::~MsoDrawingSelection()
{
}


BaseObjectPtr MsoDrawingSelection::clone()
{
	return BaseObjectPtr(new MsoDrawingSelection(*this));
}


void MsoDrawingSelection::writeFields(CFRecord& record)
{
	Log::error("MsoDrawingSelection record is not implemented.");
}


void MsoDrawingSelection::readFields(CFRecord& record)
{
	record.skipNunBytes(record.getDataSize());
}

} // namespace XLS

