
#include "Note.h"

namespace XLS
{;

Note::Note()
{
}


Note::~Note()
{
}


BaseObjectPtr Note::clone()
{
	return BaseObjectPtr(new Note(*this));
}


void Note::writeFields(CFRecord& record)
{
#pragma message("####################### Note record is not implemented")
	Log::error("Note record is not implemented.");
	//record << some_value;
}


void Note::readFields(CFRecord& record)
{
// 	is_contained_in_Workbook_ = XMLSTUFF::isChildOf(parent_, "WorkbookStreamObject", true);
// 	if(is_contained_in_Workbook_)
// 	{
		record >> note_sh;
// 	}
// 	else
// 	{
// 		record >> note_rr;
// 	}
}

} // namespace XLS

