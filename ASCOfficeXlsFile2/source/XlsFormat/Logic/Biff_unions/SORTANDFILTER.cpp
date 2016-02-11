
#include "SORTANDFILTER.h"
#include <Logic/Biff_records/Sort.h>
#include <Logic/Biff_unions/SORTDATA12.h>
#include <Logic/Biff_records/FilterMode.h>
#include <Logic/Biff_records/DropDownObjIds.h>
#include <Logic/Biff_unions/AUTOFILTER.h>

namespace XLS
{


SORTANDFILTER::SORTANDFILTER()
{
}


SORTANDFILTER::~SORTANDFILTER()
{
}


BaseObjectPtr SORTANDFILTER::clone()
{
	return BaseObjectPtr(new SORTANDFILTER(*this));
}


// SORTANDFILTER = [Sort] [SORTDATA12] [FilterMode] [DropDownObjIds] [AUTOFILTER]
const bool SORTANDFILTER::loadContent(BinProcessor& proc)
{
	bool res1 = proc.optional<Sort>();
	if (res1)
	{
		m_Sort = elements_.back();
		elements_.pop_back();
	}		
	bool res2 = proc.optional<SORTDATA12>();
	if (res2)
	{
		m_SORTDATA12 = elements_.back();
		elements_.pop_back();
	}
	bool res3 = proc.optional<FilterMode>();
	if (res3)
	{
		m_FilterMode = elements_.back();
		elements_.pop_back();
	}
	bool res4 = proc.optional<DropDownObjIds>();
	if (res3)
	{
		m_DropDownObjIds = elements_.back();
		elements_.pop_back();
	}
	bool res5 = proc.optional<AUTOFILTER>();
	if (res5)
	{
		m_AUTOFILTER = elements_.back();
		elements_.pop_back();
	}
	return res1 || res2 || res3 || res4 || res5;
}

int SORTANDFILTER::serialize(std::wostream & stream)
{
	if (m_AUTOFILTER)
	{
		m_AUTOFILTER->serialize(stream);
	}
	if (m_SORTDATA12)
	{
		m_SORTDATA12->serialize(stream);
	}
	return 0;
}

} // namespace XLS

