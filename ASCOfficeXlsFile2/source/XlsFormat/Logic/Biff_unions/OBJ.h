#pragma once

#include <Logic/CompositeObject.h>
#include <Logic/Biff_records/MsoDrawing.h>

namespace XLS
{


// Logical representation of OBJ union of records 
class OBJ: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OBJ)
public:
	OBJ(MsoDrawingPtr mso_drawing) : mso_drawing_(mso_drawing)
	{
	}
	~OBJ();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	BaseObjectPtr m_Obj;

	static const ElementType	type = typeOBJ;
	
	MsoDrawingPtr mso_drawing_;
};

} // namespace XLS

