#include "precompiled_xls.h"
#include "SUPBOOK.h"
#include <Logic/Biff_records/SupBook.h>
#include <Logic/Biff_records/ExternName.h>
#include <Logic/Biff_records/ExternSheet.h>
#include <Logic/Biff_records/Continue.h>
#include <Logic/Biff_records/XCT.h>
#include <Logic/Biff_records/CRN.h>

namespace XLS
{;


SUPBOOK::SUPBOOK()
{
}


SUPBOOK::~SUPBOOK()
{
}

class Parenthesis_SUPBOOK_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SUPBOOK_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SUPBOOK_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<XCT>())
		{
			return false;
		}
		proc.repeated<CRN>(0, 0);
		return true;
	};
};



BaseObjectPtr SUPBOOK::clone()
{
	return BaseObjectPtr(new SUPBOOK(*this));
}


// SUPBOOK = SupBook [*ExternName *(XCT *CRN)] [ExternSheet] *Continue
const bool SUPBOOK::loadContent(BinProcessor& proc)
{
	SupBook supbook;
	if(!proc.mandatory(supbook))
	{
		return false;
	}
	/*  The above is equal to:

	if(!proc.mandatory<SupBook>())
	{
	return false;
	}
	*/
	for(;/*infinity*/;)
	{
		ExternName extern_name(supbook.getSupportingLinkType());
		if(!proc.optional(extern_name))
		{
			break;
		}
	}
	//proc.repeated<ExternName>(0, 0);
	

	proc.repeated<Parenthesis_SUPBOOK_1>(0, 0);
	proc.optional<ExternSheet>();
	//proc.repeated<Continue>(0, 0);
	return true;
}

} // namespace XLS

