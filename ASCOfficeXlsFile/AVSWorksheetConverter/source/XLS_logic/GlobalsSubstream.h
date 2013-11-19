#pragma once

#include "CompositeObject.h"

namespace XLS
{;

class CFStreamCacheReader;
class BOF;

// Logical representation of Globals substream 
class GlobalsSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GlobalsSubstream)
public:
	GlobalsSubstream(const WORD code_page);
	~GlobalsSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

private:
	WORD code_page_;
};

} // namespace XLS
