#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of FORMATTING union of records 
class FORMATTING: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FORMATTING)
public:
	FORMATTING();
	~FORMATTING();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeFORMATTING;

	BaseObjectPtr				m_Styles;
	BaseObjectPtr				m_XFS;
	std::vector<BaseObjectPtr>	m_Formats;
	std::vector<BaseObjectPtr>	m_Fonts;
	BaseObjectPtr				m_Palette;
};

} // namespace XLS

