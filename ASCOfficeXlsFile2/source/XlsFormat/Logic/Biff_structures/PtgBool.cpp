#include "precompiled_xls.h"
#include "PtgBool.h"
#include <Binary/CFRecord.h>
//#include <Exception/FormulaParsingError.h>

namespace XLS
{;


PtgBool::PtgBool()
{
}


PtgBool::PtgBool(const std::wstring& word)
:	OperandPtg(fixed_id)
{
	std::wstring up_word = boost::algorithm::to_upper_copy(word);
	if(L"TRUE" == up_word)
	{
		boolean_ = true;
	}
	else if(L"FALSE" == up_word)
	{
		boolean_ = false;
	}
	else
	{
		throw;// EXCEPT::LE::FormulaParsingError("Wrong boolean format.", __FUNCTION__);
	}

}


BiffStructurePtr PtgBool::clone()
{
	return BiffStructurePtr(new PtgBool(*this));
}


//void PtgBool::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"boolean", boolean_);
//}
//

void PtgBool::storeFields(CFRecord& record)
{
	record << boolean_;
}


void PtgBool::loadFields(CFRecord& record)
{
	record >> boolean_;
}


void PtgBool::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	ptg_stack.push(boolean_ ? L"TRUE" : L"FALSE");
}


} // namespace XLS

