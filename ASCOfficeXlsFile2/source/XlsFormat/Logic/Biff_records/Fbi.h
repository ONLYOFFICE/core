#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FontIndex.h>

namespace XLS
{


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

	static const ElementType	type = typeFbi;

//-----------------------------
	BIFF_WORD dmixBasis;
	BIFF_WORD dmiyBasis;
	BIFF_WORD twpHeightBasis;
	BIFF_WORD scab;
	FontIndex ifnt;


};

} // namespace XLS

