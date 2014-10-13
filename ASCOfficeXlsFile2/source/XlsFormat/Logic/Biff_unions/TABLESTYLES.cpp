#include "precompiled_xls.h"
#include "TABLESTYLES.h"
#include <Logic/Biff_records/TableStyles.h>
#include <Logic/Biff_records/TableStyle.h>
#include <Logic/Biff_records/TableStyleElement.h>

namespace XLS
{;


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
		proc.repeated<TableStyleElement>(0, 28);
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
	proc.repeated<Parenthesis_TABLESTYLES_1>(0, 0);
	return true;
}

} // namespace XLS

