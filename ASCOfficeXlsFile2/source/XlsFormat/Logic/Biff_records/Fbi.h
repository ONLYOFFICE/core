#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FontIndex.h>

namespace XLS
{;


// Logical representation of Fbi record in BIFF8
class Fbi: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Fbi)
	BASE_OBJECT_DEFINE_CLASS_NAME(Fbi)
public:
	Fbi();
	~Fbi();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD dmixBasis;
	BIFF_WORD dmiyBasis;
	BIFF_WORD twpHeightBasis;
	BIFF_WORD scab;
	FontIndex ifnt;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(dmixBasis)
		BO_ATTRIB_MARKUP_ATTRIB(dmiyBasis)
		BO_ATTRIB_MARKUP_ATTRIB(twpHeightBasis)
		BO_ATTRIB_MARKUP_ATTRIB(scab)
		BO_ATTRIB_MARKUP_ATTRIB(ifnt)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

