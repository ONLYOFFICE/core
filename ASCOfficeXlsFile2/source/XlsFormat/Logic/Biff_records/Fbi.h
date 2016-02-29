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
	_UINT16 dmixBasis;
	_UINT16 dmiyBasis;
	_UINT16 twpHeightBasis;
	_UINT16 scab;
	FontIndex ifnt;


};

} // namespace XLS

