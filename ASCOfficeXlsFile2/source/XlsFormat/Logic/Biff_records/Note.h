#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/NoteSh.h>
#include <Logic/Biff_structures/NoteRR.h>

namespace XLS
{


// Logical representation of Note record in BIFF8
class Note: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Note)
	BASE_OBJECT_DEFINE_CLASS_NAME(Note)
public:
	Note();
	~Note();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeNote;

//-----------------------------
// 	BIFF_BOOL is_contained_in_Workbook_;
	
	NoteSh note_sh;
// 	NoteRR note_rr;

public:
	//BO_ATTRIB_MARKUP_BEGIN
// 		if(is_contained_in_Workbook_)
// 		{
			//BO_ATTRIB_MARKUP_COMPLEX(note_sh)
// 		}
// 		else
// 		{
// 			//BO_ATTRIB_MARKUP_COMPLEX(note_rr)
// 		}
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

