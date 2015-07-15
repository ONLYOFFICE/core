#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtRefHeaderU.h>
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/TableFeatureType.h>

namespace XLS
{;


// Logical representation of Feature11 record in BIFF8
class Feature11: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Feature11)
	BASE_OBJECT_DEFINE_CLASS_NAME(Feature11)
public:
	Feature11();
	~Feature11();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFeature11;



public:
	FrtRefHeaderU frtRefHeaderU;
	BIFF_WORD isf;
	BIFF_WORD cref2;
	BIFF_DWORD cbFeatData;
	BiffStructurePtrVector refs2;
	BIFF_BSTR sqref;

	TableFeatureType rgbFeat;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(frtRefHeaderU)
		//BO_ATTRIB_MARKUP_ATTRIB(isf)
		//BO_ATTRIB_MARKUP_ATTRIB(cref2)
		//BO_ATTRIB_MARKUP_ATTRIB(cbFeatData)
		//BO_ATTRIB_MARKUP_ATTRIB(sqref)
		//BO_ATTRIB_MARKUP_VECTOR_COMPLEX(refs2, Ref8U)
		//BO_ATTRIB_MARKUP_COMPLEX(rgbFeat)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

