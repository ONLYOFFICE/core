#include "stdafx.h"
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
	record.skipNBytes(record.getDataSize());
}

} // namespace XLS

