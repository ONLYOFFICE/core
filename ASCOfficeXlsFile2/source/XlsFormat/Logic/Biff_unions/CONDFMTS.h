#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of CONDFMTS union of records 
class CONDFMTS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CONDFMTS)
public:
	CONDFMTS();
	~CONDFMTS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeCONDFMTS;

	int serialize(std::wostream & stream);

	std::vector<BaseObjectPtr> m_arCONDFMT; //CONDFMT or CONDFMT12
	std::vector<BaseObjectPtr> m_arCFEx;
};

} // namespace XLS

