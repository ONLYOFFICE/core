#pragma once

#include "BiffRecord.h"

namespace XLS
{;


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
private:
	BIFF_BOOL fNoSaveSup;
	BIFF_BOOL fHasEnvelope;
	BIFF_BOOL fEnvelopeVisible;
	BIFF_BOOL fEnvelopeInitDone;
	BIFF_BSTR grUpdateLinks;
	BIFF_BOOL fHideBorderUnselLists;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fNoSaveSup)
		BO_ATTRIB_MARKUP_ATTRIB(fHasEnvelope)
		BO_ATTRIB_MARKUP_ATTRIB(fEnvelopeVisible)
		BO_ATTRIB_MARKUP_ATTRIB(fEnvelopeInitDone)
		BO_ATTRIB_MARKUP_ATTRIB(grUpdateLinks)
		BO_ATTRIB_MARKUP_ATTRIB(fHideBorderUnselLists)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

