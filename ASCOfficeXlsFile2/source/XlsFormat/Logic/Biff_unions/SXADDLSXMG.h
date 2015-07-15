#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLSXMG union of records 
class SXADDLSXMG: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLSXMG)
public:
	SXADDLSXMG();
	~SXADDLSXMG();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLSXMG;
	
};

} // namespace XLS

