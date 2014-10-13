#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLCONDFMTS union of records 
class SXADDLCONDFMTS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLCONDFMTS)
public:
	SXADDLCONDFMTS();
	~SXADDLCONDFMTS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

