#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of DROPBAR union of records 
class DROPBAR: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DROPBAR)
public:
	DROPBAR();
	~DROPBAR();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeDROPBAR;

	int serialize(std::wostream & _stream);

	BaseObjectPtr m_DropBar;
	BaseObjectPtr m_LineFormat;
	BaseObjectPtr m_AreaFormat;
	BaseObjectPtr m_GELFRAME;
	BaseObjectPtr m_SHAPEPROPS;
};

} // namespace XLS

