#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SORTANDFILTER union of records 
class SORTANDFILTER: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SORTANDFILTER)
public:
	SORTANDFILTER();
	~SORTANDFILTER();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	int serialize(std::wostream & stream);

	static const ElementType	type = typeSORTANDFILTER;

	BaseObjectPtr				m_Sort;
	BaseObjectPtr				m_SORTDATA12;
	BaseObjectPtr				m_FilterMode;
	BaseObjectPtr				m_DropDownObjIds;
	BaseObjectPtr				m_AUTOFILTER;	
};

} // namespace XLS

