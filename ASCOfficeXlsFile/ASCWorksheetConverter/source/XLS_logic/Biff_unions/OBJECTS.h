#pragma once

#include <XLS_logic/CompositeObject.h>

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
	virtual const bool loadContentWrite(BinWriterProcessor& proc);

private:
	bool is_inside_chart_sheet_;
};

} // namespace XLS

