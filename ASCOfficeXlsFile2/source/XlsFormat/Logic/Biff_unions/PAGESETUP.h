#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PAGESETUP union of records 
class PAGESETUP: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PAGESETUP)
public:
	PAGESETUP();
	~PAGESETUP();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

