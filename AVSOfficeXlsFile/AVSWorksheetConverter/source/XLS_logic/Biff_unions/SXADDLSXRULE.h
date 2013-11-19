#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLSXRULE union of records 
class SXADDLSXRULE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLSXRULE)
public:
	SXADDLSXRULE();
	~SXADDLSXRULE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

