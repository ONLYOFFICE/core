
#include "SUPBOOK.h"
#include <Logic/Biff_records/SupBook.h>
#include <Logic/Biff_records/ExternName.h>
#include <Logic/Biff_records/ExternSheet.h>
#include <Logic/Biff_records/Continue.h>
#include <Logic/Biff_records/XCT.h>
#include <Logic/Biff_records/CRN.h>

namespace XLS
{


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
	m_SupBook = elements_.back();
	elements_.pop_back();

	if (supbook.cch != 0x0401  && supbook.cch != 0x3A01 )
	{
		proc.getGlobalWorkbookInfo()->arExternalNames.clear();
	}

	while(true)
	{
		ExternName extern_name(supbook.getSupportingLinkType());
		if(!proc.optional(extern_name))
		{
			break;
		}
	}

	int count = proc.repeated<Parenthesis_SUPBOOK_1>(0, 0);
	
	if (proc.optional<ExternSheet>())
	{
		m_ExternSheet = elements_.back();
		elements_.pop_back();
	}
	//proc.repeated<Continue>(0, 0);
	return true;
}

} // namespace XLS

