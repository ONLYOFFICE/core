
#include "SORTDATA12.h"
#include <Logic/Biff_records/SortData.h>
#include <Logic/Biff_records/ContinueFrt12.h>

namespace XLS
{


SORTDATA12::SORTDATA12()
{
}


SORTDATA12::~SORTDATA12()
{
}


BaseObjectPtr SORTDATA12::clone()
{
	return BaseObjectPtr(new SORTDATA12(*this));
}


// SORTDATA12 = SortData *ContinueFrt12
const bool SORTDATA12::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SortData>())
	{
		return false;
	}
	m_SortData = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<ContinueFrt12>(0, 0); // processed inside of SortData

	return true;
}

int SORTDATA12::serialize(std::wostream & stream)
{
	if (m_SortData == NULL) return 0;

	SortData *info = dynamic_cast<SortData*>(m_SortData.get());

	std::wstring ref = info->rfx;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"sortState")
		{	
			CP_XML_ATTR(L"ref", ref);
			if (info->fCol)				CP_XML_ATTR(L"columnSort",		1);
			if (info->fCaseSensitive)	CP_XML_ATTR(L"caseSensitive",	1);
			if (info->fAltMethod)		CP_XML_ATTR(L"sortMethod",		L"stroke");

			for (int i = 0 ; i < info->sortCond12Array.size(); i++)
			{
				SortCond12 * sortCond = dynamic_cast<SortCond12 *>(info->sortCond12Array[i].get());
				if (sortCond == NULL) continue;

				CP_XML_NODE(L"sortCondition")
				{
					CP_XML_ATTR(L"ref", sortCond->rfx.toString());
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

