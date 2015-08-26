#pragma once

#include <Logic/CompositeObject.h>
#include <Logic/Biff_records/MsoDrawing.h>

namespace XLS
{


// Logical representation of CHART union of records 
class CHART: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CHART)
public:
	CHART(MsoDrawingPtr mso_drawing) : mso_drawing_(mso_drawing)
	{
	}
	~CHART();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeCHART;
	
	MsoDrawingPtr mso_drawing_;
};

} // namespace XLS

