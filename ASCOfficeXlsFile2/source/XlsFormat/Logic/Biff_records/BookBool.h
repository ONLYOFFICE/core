#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of BookBool record in BIFF8
class BookBool: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BookBool)
	BASE_OBJECT_DEFINE_CLASS_NAME(BookBool)
public:
	BookBool();
	~BookBool();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBookBool;

//-----------------------------
	bool fNoSaveSup;
	bool fHasEnvelope;
	bool fEnvelopeVisible;
	bool fEnvelopeInitDone;
	bool fHideBorderUnselLists;

	BIFF_BSTR grUpdateLinks;
};

} // namespace XLS

