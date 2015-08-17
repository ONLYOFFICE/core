
#include "SHFMLA_SET.h"
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{


SHFMLA_SET::SHFMLA_SET(std::vector<CellRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
}


SHFMLA_SET::~SHFMLA_SET()
{
}


BaseObjectPtr SHFMLA_SET::clone()
{
	return BaseObjectPtr(new SHFMLA_SET(*this));
}


const bool SHFMLA_SET::loadContent(BinProcessor& proc)
{
	return true;
}

//void SHFMLA_SET::toXML(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	size_t counter = 0;
//	for(std::vector<CellRef>::const_iterator it = shared_formulas_locations_ref_.begin(), itEnd = shared_formulas_locations_ref_.end(); it != itEnd; ++it)
//	{
//		MSXML2::IXMLDOMElementPtr shrfmla_tag = XMLSTUFF::createElement(L"shfmla", own_tag);
//		shrfmla_tag->setAttribute(L"si", counter++);
//		shrfmla_tag->setAttribute(L"ref", it->toString().c_str());
//		shrfmla_tag->setAttribute(L"row", it->getRow());
//		shrfmla_tag->setAttribute(L"col", it->getColumn());
//	}
//}


} // namespace XLS

