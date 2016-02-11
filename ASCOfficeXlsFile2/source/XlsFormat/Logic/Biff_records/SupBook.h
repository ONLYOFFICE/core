#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of SupBook record in BIFF8
class SupBook: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SupBook)
	BASE_OBJECT_DEFINE_CLASS_NAME(SupBook)
public:
	SupBook();
	~SupBook();

	const unsigned short getSupportingLinkType() const;

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSupBook;

//-----------------------------
	_UINT16					ctab;
	_UINT16					cch;

	XLUnicodeStringNoCch	virtPath;
	XLUnicodeString			rgst;

};

typedef boost::shared_ptr<SupBook> SupBookPtr;

} // namespace XLS

