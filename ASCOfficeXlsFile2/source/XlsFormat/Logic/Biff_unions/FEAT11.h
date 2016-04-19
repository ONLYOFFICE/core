#pragma once

#include <Logic/CompositeObject.h>
#include <vector>

namespace XLS
{


// Logical representation of FEAT11 union of records 
class FEAT11: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FEAT11)
public:
	FEAT11();
	~FEAT11();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeFEAT11;

	BaseObjectPtr					m_FeatHdr11;

	struct _data
	{
		BaseObjectPtr					m_Feature; //11 or 12
		BaseObjectPtr					m_AutoFilter12;
		std::vector<BaseObjectPtr>		m_arList12;
		std::vector<BaseObjectPtr>		m_arList12_second;
		std::vector<BaseObjectPtr>		m_arAutoFilter12;
		BaseObjectPtr					m_SORTDATA12;
	};

	std::vector<_data>					m_arFEAT;
};

} // namespace XLS

