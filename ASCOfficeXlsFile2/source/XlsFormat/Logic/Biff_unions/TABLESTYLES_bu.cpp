
#include "TABLESTYLES.h"
#include <Logic/Biff_records/TableStyles.h>
#include <Logic/Biff_records/TableStyle.h>
#include <Logic/Biff_records/TableStyleElement.h>

namespace XLS
{


TABLESTYLES::TABLESTYLES()
{
}


TABLESTYLES::~TABLESTYLES()
{
}



class Parenthesis_TABLESTYLES_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_TABLESTYLES_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_TABLESTYLES_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<TableStyle>())
		{
			return false;
		}
		int count = proc.repeated<TableStyleElement>(0, 28);
		return true;
	};
};


BaseObjectPtr TABLESTYLES::clone()
{
	return BaseObjectPtr(new TABLESTYLES(*this));
}


// TABLESTYLES = TableStyles *(TableStyle *28TableStyleElement)

const bool TABLESTYLES::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<TableStyles>())
	{
		return false;
	}
	
	m_TableStyles = elements_.back();
	elements_.pop_back();
		
	int count = proc.repeated<Parenthesis_TABLESTYLES_1>(0, 0);
	while(!elements_.empty())
	{
		if (elements_.front()->get_type() == typeTableStyle)
		{
			_table_style new_style;
			new_style.style_ = elements_.front();
			m_arTableStyles.push_back(new_style);
		}
		else
		{
			if (m_arTableStyles.size() > 0)
				m_arTableStyles.back().elements_.push_back(elements_.front());
		}
		elements_.pop_back();
	}	
	return true;
}
int TABLESTYLES::serialize(std::wostream & stream)
{
	TableStyles * styles = dynamic_cast<TableStyles*>(m_TableStyles.get());
	if (!styles) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"tableStyles")
		{
			CP_XML_ATTR(L"count", m_arTableStyles.size());
			
			if (!styles->rgchDefPivotStyle.value().empty())
			{
				CP_XML_ATTR(L"defaultPivotStyle", styles->rgchDefPivotStyle.value());
			}
			if (!styles->rgchDefTableStyle.value().empty())
			{
				CP_XML_ATTR(L"defaultTableStyle", styles->rgchDefTableStyle.value());
			}

		}
	}
	return 0;
}
} // namespace XLS

