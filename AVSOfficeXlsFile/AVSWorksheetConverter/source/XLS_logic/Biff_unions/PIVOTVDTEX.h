#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTVDTEX union of records 
class PIVOTVDTEX: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTVDTEX)
public:
	PIVOTVDTEX();
	~PIVOTVDTEX();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

