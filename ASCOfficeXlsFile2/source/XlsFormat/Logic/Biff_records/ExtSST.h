#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/ISSTInf.h>

namespace XLS
{;


// Logical representation of ExtSST record in BIFF8
class ExtSST: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ExtSST)
	BASE_OBJECT_DEFINE_CLASS_NAME(ExtSST)
public:
	ExtSST();
	~ExtSST();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD dsst;
	BiffStructurePtrVector rgISSTInf;
	BackwardOnlyParam<unsigned int> num_sets;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(dsst)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgISSTInf, ISSTInf);
	BO_ATTRIB_MARKUP_ATTRIB(num_sets)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

