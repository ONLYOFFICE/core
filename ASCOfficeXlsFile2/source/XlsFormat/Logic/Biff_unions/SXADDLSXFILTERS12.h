#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLSXFILTERS12 union of records 
class SXADDLSXFILTERS12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLSXFILTERS12)
public:
	SXADDLSXFILTERS12();
	~SXADDLSXFILTERS12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

