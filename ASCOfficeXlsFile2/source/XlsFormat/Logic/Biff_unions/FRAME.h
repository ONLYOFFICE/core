#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of FRAME union of records 
class FRAME: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FRAME)
public:
	FRAME();
	~FRAME();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
	
	int serialize(std::wostream & stream_);

	static const ElementType	type = typeFRAME;

	BaseObjectPtr m_Frame;
	BaseObjectPtr m_AreaFormat;
	BaseObjectPtr m_LineFormat;
	BaseObjectPtr m_GELFRAME;
	BaseObjectPtr m_SHAPEPROPS;

};

} // namespace XLS

