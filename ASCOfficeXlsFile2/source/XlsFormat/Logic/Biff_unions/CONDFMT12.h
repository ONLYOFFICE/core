#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of CONDFMT12 union of records 
class CONDFMT12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CONDFMT12)
public:
	CONDFMT12();
	~CONDFMT12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeCONDFMT12;

	int serialize(std::wostream & stream);

	BaseObjectPtr				m_CondFmt12;
	std::vector<BaseObjectPtr>	m_arCF12;
};

} // namespace XLS

