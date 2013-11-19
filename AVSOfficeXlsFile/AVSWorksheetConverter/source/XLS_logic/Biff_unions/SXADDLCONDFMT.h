#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLCONDFMT union of records 
class SXADDLCONDFMT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLCONDFMT)
public:
	SXADDLCONDFMT();
	~SXADDLCONDFMT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

