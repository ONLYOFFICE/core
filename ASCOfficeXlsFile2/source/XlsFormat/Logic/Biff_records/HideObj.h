#pragma once

#include "BiffRecord.h"
#include "../Biff_structures/HideObjEnum.h"

namespace XLS
{;


// Logical representation of HideObj record in BIFF8
class HideObj: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(HideObj)
	BASE_OBJECT_DEFINE_CLASS_NAME(HideObj)
public:
	HideObj();
	~HideObj();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeHideObj;

//-----------------------------
	HideObjEnum hideObj;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(hideObj)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

