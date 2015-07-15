#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of IVAXIS union of records 
class IVAXIS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(IVAXIS)
public:
	IVAXIS();
	~IVAXIS();

	BaseObjectPtr clone();

	virtual const bool loadContentRead(BinReaderProcessor& proc);
	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeIVAXIS;

	unsigned int id;

};

} // namespace XLS

