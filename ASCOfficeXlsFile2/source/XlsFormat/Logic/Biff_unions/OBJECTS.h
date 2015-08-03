#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;

class  MsoDrawing;
typedef boost::shared_ptr<MsoDrawing> MsoDrawingPtr;

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

	MsoDrawingPtr								m_MsoDrawing;
	MsoDrawingPtr								m_MsoDrawingObjects;
	
	std::vector<std::pair<BaseObjectPtr, int>>	m_OBJs;
	std::vector<std::pair<BaseObjectPtr, int>>	m_TEXTOBJECTs;
	std::vector<std::pair<BaseObjectPtr, int>>	m_CHARTs;
};

} // namespace XLS

