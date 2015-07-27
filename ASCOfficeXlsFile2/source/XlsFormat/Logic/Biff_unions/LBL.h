#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of LBL union of records 
class LBL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(LBL)
public:
	LBL();
	~LBL();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	int serialize(std::wostream & stream);
	
	static const ElementType	type = typeLBL;

	BaseObjectPtr m_Lbl;

};

} // namespace XLS

