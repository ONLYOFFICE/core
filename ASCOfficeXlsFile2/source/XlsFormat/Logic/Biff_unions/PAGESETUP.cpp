#include "precompiled_xls.h"
#include "PAGESETUP.h"
#include <Logic/Biff_records/Header.h>
#include <Logic/Biff_records/Footer.h>
#include <Logic/Biff_records/HCenter.h>
#include <Logic/Biff_records/VCenter.h>
#include <Logic/Biff_records/LeftMargin.h>
#include <Logic/Biff_records/RightMargin.h>
#include <Logic/Biff_records/TopMargin.h>
#include <Logic/Biff_records/BottomMargin.h>
#include <Logic/Biff_records/Pls.h>
#include <Logic/Biff_records/Continue.h>
#include <Logic/Biff_records/Setup.h>
#include <Logic/Biff_records/HeaderFooter.h> // Moved from WorksheetSubstream

namespace XLS
{;


PAGESETUP::PAGESETUP()
{
}


PAGESETUP::~PAGESETUP()
{
}

class Parenthesis_PAGESETUP_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_PAGESETUP_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_PAGESETUP_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		//todooo ????
		//if(!proc.mandatory(Pls(proc.getParent())))
		{
			return false;
		}
		//proc.repeated<Continue>(0, 0);
		//return true;
	};
};



BaseObjectPtr PAGESETUP::clone()
{
	return BaseObjectPtr(new PAGESETUP(*this));
}


// PAGESETUP = Header Footer HCenter VCenter [LeftMargin] [RightMargin] [TopMargin] [BottomMargin] [Pls *Continue] Setup
const bool PAGESETUP::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Header>())
	{
		return false;
	}
	proc.mandatory<Footer>();
	proc.mandatory<HCenter>();
	proc.mandatory<VCenter>();
	proc.optional<BottomMargin>(); // OpenOffice Calc stored files workaround
	proc.optional<TopMargin>(); // OpenOffice Calc stored files workaround
	proc.optional<LeftMargin>();
	proc.optional<RightMargin>();
	proc.optional<TopMargin>();
	proc.optional<BottomMargin>();
	proc.optional<Parenthesis_PAGESETUP_1>();
	//proc.optional(Pls(proc.getParent())); // OpenOffice Calc stored files workaround
	//todooo ?????
	proc.mandatory<Setup>();
	proc.optional<HeaderFooter>(); // Moved from WorksheetSubstream


	return true;
}

} // namespace XLS

