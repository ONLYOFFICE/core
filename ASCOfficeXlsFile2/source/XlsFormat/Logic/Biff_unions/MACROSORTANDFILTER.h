#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of MACROSORTANDFILTER union of records 
class MACROSORTANDFILTER: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MACROSORTANDFILTER)
public:
	MACROSORTANDFILTER();
	~MACROSORTANDFILTER();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeMACROSORTANDFILTER;
};

} // namespace XLS

