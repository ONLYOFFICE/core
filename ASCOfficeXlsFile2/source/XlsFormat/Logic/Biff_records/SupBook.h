#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of SupBook record in BIFF8
class SupBook: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SupBook)
	BASE_OBJECT_DEFINE_CLASS_NAME(SupBook)
public:
	SupBook();
	~SupBook();

	const unsigned __int16 getSupportingLinkType() const;

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSupBook;

//-----------------------------
	BIFF_WORD ctab;
	BIFF_WORD cch;
	XLUnicodeStringNoCch virtPath;
	XLUnicodeString rgst;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(ctab)
		//BO_ATTRIB_MARKUP_ATTRIB(cch)
		//BO_ATTRIB_MARKUP_ATTRIB_NAME(virtPath/*.getEscapedW()*/, L"virtPath")
		//BO_ATTRIB_MARKUP_ATTRIB(rgst)
	//BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<SupBook> SupBookPtr;

} // namespace XLS

