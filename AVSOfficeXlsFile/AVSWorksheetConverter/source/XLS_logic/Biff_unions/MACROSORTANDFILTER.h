#pragma once

#include <XLS_logic/CompositeObject.h>

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

};

} // namespace XLS

