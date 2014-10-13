#include "precompiled_xls.h"
#include "STYLES.h"
#include <Logic/Biff_records/Style.h>
#include <Logic/Biff_records/StyleExt.h>

namespace XLS
{;


STYLES::STYLES()
{
}


STYLES::~STYLES()
{
}


class Parenthesis_STYLES_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_STYLES_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_STYLES_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<Style>())
		{
			return false;
		}
		proc.optional<StyleExt>();
		return true;
	};
};

BaseObjectPtr STYLES::clone()
{
	return BaseObjectPtr(new STYLES(*this));
}


// STYLES = 1*(Style [StyleExt])
const bool STYLES::loadContent(BinProcessor& proc)
{
	return proc.repeated<Parenthesis_STYLES_1>(1, 0);
}

} // namespace XLS

