#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SXADDLSXFILTER12 union of records 
class SXADDLSXFILTER12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLSXFILTER12)
public:
	SXADDLSXFILTER12();
	~SXADDLSXFILTER12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLSXFILTER12;
	
};

} // namespace XLS

