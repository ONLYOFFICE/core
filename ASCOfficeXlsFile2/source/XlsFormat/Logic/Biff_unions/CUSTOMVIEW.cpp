#include "precompiled_xls.h"
#include "CUSTOMVIEW.h"
#include <Logic/Biff_records/UserSViewBegin.h>
#include <Logic/Biff_records/Selection.h>
#include <Logic/Biff_records/HorizontalPageBreaks.h>
#include <Logic/Biff_records/VerticalPageBreaks.h>
#include <Logic/Biff_records/Header.h>
#include <Logic/Biff_records/Footer.h>
#include <Logic/Biff_records/HCenter.h>
#include <Logic/Biff_records/VCenter.h>
#include <Logic/Biff_records/LeftMargin.h>
#include <Logic/Biff_records/RightMargin.h>
#include <Logic/Biff_records/TopMargin.h>
#include <Logic/Biff_records/BottomMargin.h>
#include <Logic/Biff_records/Pls.h>
#include <Logic/Biff_records/Setup.h>
#include <Logic/Biff_records/PrintSize.h>
#include <Logic/Biff_records/HeaderFooter.h>
#include <Logic/Biff_records/UserSViewEnd.h>
#include <Logic/Biff_unions/AUTOFILTER.h>

namespace XLS
{;


CUSTOMVIEW::CUSTOMVIEW()
{
}


CUSTOMVIEW::~CUSTOMVIEW()
{
}


BaseObjectPtr CUSTOMVIEW::clone()
{
	return BaseObjectPtr(new CUSTOMVIEW(*this));
}


/*
CUSTOMVIEW = UserSViewBegin *Selection [HorizontalPageBreaks] [VerticalPageBreaks] [Header] 
			[Footer] [HCenter] [VCenter] [LeftMargin] [RightMargin] [TopMargin] [BottomMargin] [Pls] [Setup] 
			[PrintSize] [HeaderFooter] [AUTOFILTER] UserSViewEnd
*/
const bool CUSTOMVIEW::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<UserSViewBegin>())
	{
		return false;
	}
	proc.repeated<Selection>(0, 2);
	proc.optional<HorizontalPageBreaks>();
	proc.optional<VerticalPageBreaks>();
	proc.optional<Header>();
	proc.optional<Footer>();
	proc.optional<HCenter>();
	proc.optional<VCenter>();
	proc.optional<LeftMargin>();
	proc.optional<RightMargin>();
	proc.optional<TopMargin>();
	proc.optional<BottomMargin>();
	//proc.optional(Pls(proc.getParent()));//??????
	proc.optional<Setup>();
	proc.optional<PrintSize>();
	proc.optional<HeaderFooter>();
	proc.optional<AUTOFILTER>();
	proc.mandatory<UserSViewEnd>();

	return true;
}

} // namespace XLS

