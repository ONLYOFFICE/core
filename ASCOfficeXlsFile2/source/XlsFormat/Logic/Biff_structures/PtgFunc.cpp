#include "precompiled_xls.h"
#include "PtgFunc.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgFunc::PtgFunc()
{
}


PtgFunc::PtgFunc(const unsigned __int16 func_index, const unsigned char data_type)
:	Ptg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	iftab(func_index)
{
}


BiffStructurePtr PtgFunc::clone()
{
	return BiffStructurePtr(new PtgFunc(*this));
}

//
//void PtgFunc::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	iftab.toXML(xml_tag);
//}


void PtgFunc::storeFields(CFRecord& record)
{
	record << iftab;
}


void PtgFunc::loadFields(CFRecord& record)
{
	record >> iftab;
}


void PtgFunc::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	long num_params = iftab.getParamsNum();
	std::wstring arguments;
	if(num_params)
	{
		arguments += ptg_stack.top();
		ptg_stack.pop();
		for(long i = 0; i < num_params - 1; ++i)
		{
			arguments = ptg_stack.top() + L',' + arguments;
			ptg_stack.pop();
		}
	}
	ptg_stack.push(iftab.getFuncName() + L'(' + arguments + L')');

}


// static
PtgPtr PtgFunc::create(const std::wstring& word, const unsigned char data_type)
{
	unsigned __int16 func_index = Ftab_Cetab::recognizeFunction(word);
	if(0xFFFF != func_index)
	{
		Ftab_Cetab tab(func_index);
		if(!tab.isMacro() && -1 != tab.getParamsNum())
		{
			return PtgPtr(new PtgFunc(func_index, data_type));
		}
	}
	return PtgPtr();
}


const long PtgFunc::getParametersNum() const
{
	return iftab.getParamsNum();
}


} // namespace XLS

