#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of TABLESTYLES union of records 
class TABLESTYLES: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(TABLESTYLES)
public:
	TABLESTYLES();
	~TABLESTYLES();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeTABLESTYLES;

	int serialize(std::wostream & stream);
//----------------------------------------------------------------------------------
	BaseObjectPtr				m_TableStyles;

	struct _table_style
	{
		BaseObjectPtr				style_;
		std::vector<BaseObjectPtr>	elements_;
	};
	std::vector<_table_style>		m_arTableStyles;
	
};

} // namespace XLS

