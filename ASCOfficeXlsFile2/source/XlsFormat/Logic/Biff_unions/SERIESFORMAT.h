#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SERIESFORMAT union of records 
class SERIESFORMAT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SERIESFORMAT)
public:
	SERIESFORMAT();
	~SERIESFORMAT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSERIESFORMAT;

	std::vector<BaseObjectPtr>	m_arSS;
	std::vector<BaseObjectPtr>	m_arAI;

	BaseObjectPtr				m_Series;
	
	std::vector<BaseObjectPtr>	m_arDopSeries;

	struct _series_ex
	{
		BaseObjectPtr	legendException;
		BaseObjectPtr	attachedLABEL;
		BaseObjectPtr	textPROPS;
	};
	std::vector<_series_ex>		m_SeriesEx;

//-----------------------------------------------------------------------

	
};
} // namespace XLS

