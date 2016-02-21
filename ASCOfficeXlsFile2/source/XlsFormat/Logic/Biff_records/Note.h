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

	NoteSh note_sh;
// 	NoteRR note_rr;


};

} // namespace XLS

