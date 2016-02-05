#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of WINDOW union of records 
class WINDOW: public CompositeObject
{
public:
	static const ElementType	type = typeWINDOW;
	
	BASE_OBJECT_DEFINE_CLASS_NAME(WINDOW);
	
	WINDOW();
	~WINDOW();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	int serialize(std::wostream & stream);

	std::vector<BaseObjectPtr>	m_arSelection;
	BaseObjectPtr				m_Scl;
	BaseObjectPtr				m_PLV;
	BaseObjectPtr				m_Window2;
	BaseObjectPtr				m_Pane;

	
};

} // namespace XLS

