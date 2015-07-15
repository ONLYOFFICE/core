#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of OBJECTS union of records 
class OBJECTS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OBJECTS)
public:
	OBJECTS(const bool is_inside_chart_sheet);
	~OBJECTS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	virtual const bool loadContentRead(BinReaderProcessor& proc);

	static const ElementType	type = typeOBJECTS;

	bool is_inside_chart_sheet_;
};

} // namespace XLS

