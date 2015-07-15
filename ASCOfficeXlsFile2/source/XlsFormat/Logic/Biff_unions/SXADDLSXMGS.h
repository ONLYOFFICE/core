#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLSXMGS union of records 
class SXADDLSXMGS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLSXMGS)
public:
	SXADDLSXMGS();
	~SXADDLSXMGS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLSXMGS;
	
};

} // namespace XLS

