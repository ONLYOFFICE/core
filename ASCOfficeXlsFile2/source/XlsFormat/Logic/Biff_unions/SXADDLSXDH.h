#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLSXDH union of records 
class SXADDLSXDH: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLSXDH)
public:
	SXADDLSXDH();
	~SXADDLSXDH();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

