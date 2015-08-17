#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/SerAr.h>

namespace XLS
{


// Logical representation of CRN record in BIFF8
class CRN: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CRN)
	BASE_OBJECT_DEFINE_CLASS_NAME(CRN)
public:
	CRN();
	~CRN();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCRN;

//-----------------------------
	BIFF_BYTE colLast;
	BIFF_BYTE colFirst;
	BIFF_WORD row;
	BiffStructurePtrVector crnOper;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(colLast)
		//BO_ATTRIB_MARKUP_ATTRIB(colFirst)
		//BO_ATTRIB_MARKUP_ATTRIB(row)
		////BO_ATTRIB_MARKUP_VECTOR_COMPLEX(crnOper, SerAr) // Temporarily commented to avoid compilation errors

	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

