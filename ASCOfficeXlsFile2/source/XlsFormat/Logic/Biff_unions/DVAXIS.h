#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DVAXIS union of records 
class DVAXIS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DVAXIS)
public:
	DVAXIS();
	~DVAXIS();

	BaseObjectPtr clone();

	virtual const bool loadContentRead(BinReaderProcessor& proc);
	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeDVAXIS;

	unsigned int id;

};

} // namespace XLS

