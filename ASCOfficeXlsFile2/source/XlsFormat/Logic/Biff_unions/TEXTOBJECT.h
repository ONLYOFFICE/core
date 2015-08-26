#pragma once

#include <Logic/CompositeObject.h>
#include <Logic/Biff_records/MsoDrawing.h>

namespace XLS
{


// Logical representation of TEXTOBJECT union of records 
class TEXTOBJECT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(TEXTOBJECT)
public:
	TEXTOBJECT(MsoDrawingPtr mso_drawing) : mso_drawing_(mso_drawing)
	{
	}
	~TEXTOBJECT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeTEXTOBJECT;
	
	BaseObjectPtr		m_TxO;
	MsoDrawingPtr		mso_drawing_;
};

} // namespace XLS

