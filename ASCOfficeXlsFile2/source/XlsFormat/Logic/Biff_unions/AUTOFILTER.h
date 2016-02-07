#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of AUTOFILTER union of records 
class AUTOFILTER: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AUTOFILTER)
public:
	AUTOFILTER();
	~AUTOFILTER();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	int serialize(std::wostream & stream);

	BaseObjectPtr								m_AutoFilterInfo;	
	std::vector<BaseObjectPtr>					m_arFilters;
	std::map<int, std::vector<BaseObjectPtr>>	m_mapFilters12;

	std::vector<BaseObjectPtr>					m_arSORTDATA12;

	GlobalWorkbookInfoPtr		pGlobalWorkbookInfoPtr;

};

} // namespace XLS

