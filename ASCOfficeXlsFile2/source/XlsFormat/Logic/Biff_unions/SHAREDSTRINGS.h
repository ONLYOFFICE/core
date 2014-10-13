#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SHAREDSTRINGS union of records 
class SHAREDSTRINGS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SHAREDSTRINGS)
public:
	SHAREDSTRINGS(const unsigned __int16 code_page);
	~SHAREDSTRINGS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

private:
	unsigned __int16 code_page_;
};

} // namespace XLS

