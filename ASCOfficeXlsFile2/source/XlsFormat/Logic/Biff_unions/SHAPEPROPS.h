#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SHAPEPROPS union of records 
class SHAPEPROPS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SHAPEPROPS)
public:
	SHAPEPROPS();
	~SHAPEPROPS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	int serialize(std::wostream & stream_);

	static const ElementType	type = typeSHAPEPROPS;

	BaseObjectPtr m_ShapePropsStream;
	
};

} // namespace XLS

