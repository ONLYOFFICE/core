#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;

class  MsoDrawing;
typedef boost::shared_ptr<MsoDrawing> MsoDrawingPtr;

// Logical representation of OBJ union of records 
class OBJ: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OBJ)
public:
	OBJ(MsoDrawingPtr mso_drawing);
	~OBJ();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	BaseObjectPtr			m_Obj;
	MsoDrawingPtr			m_MsoDrawing;

	static const ElementType	type = typeOBJ;
	
	MsoDrawingPtr mso_drawing_;
};

} // namespace XLS

