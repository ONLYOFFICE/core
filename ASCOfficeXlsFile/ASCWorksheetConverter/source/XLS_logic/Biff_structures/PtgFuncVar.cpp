#include "stdafx.h"
#include "PtgFuncVar.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgFuncVar::PtgFuncVar()
{
}


PtgFuncVar::PtgFuncVar(const WORD func_index, const BYTE data_type)
:	Ptg(fixed_id | (static_cast<BYTE>(data_type) << 5)),
	tab(func_index)
{
}


BiffStructurePtr PtgFuncVar::clone()
{
	return BiffStructurePtr(new PtgFuncVar(*this));
}


void PtgFuncVar::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"cparams", cparams);
	xml_tag->setAttribute(L"fCeFunc", fCeFunc);
	tab.toXML(xml_tag);
}


void PtgFuncVar::storeFields(CFRecord& record)
{
	record << cparams;
	record.storeAnyData(tab);
}


void PtgFuncVar::loadFields(CFRecord& record)
{
	record >> cparams;
	record.loadAnyData(tab);
	fCeFunc = tab.isMacro();
}


void PtgFuncVar::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	std::wstring arguments;
	std::wstring func_name = tab.getFuncName();

	BYTE nparams = cparams; // To let us change the value temporarily
	if(nparams && 0xFF == tab.getIndex()) // user-defined function
	{
		--nparams;
	}
	int sz = ptg_stack.size();

	if(nparams)
	{
		arguments += ptg_stack.top();
		ptg_stack.pop();
		for(BYTE i = 0; i < nparams - 1; ++i)
		{
			arguments = ptg_stack.top() + L',' + arguments;
			ptg_stack.pop();
		}
	}
	if(0xFF == tab.getIndex()) // user-defined function
	{
		func_name = ptg_stack.top();
		ptg_stack.pop();
	}
	ptg_stack.push(func_name + L'(' + arguments + L')');
}


// static
PtgPtr PtgFuncVar::create(const std::wstring& word, const BYTE data_type)
{
	WORD func_index = Ftab_Cetab::recognizeFunction(word);
	if(0xFFFF != func_index)
	{
		Ftab_Cetab tab(func_index);
		if(tab.isMacro() || -1 == tab.getParamsNum())
		{
			return PtgPtr(new PtgFuncVar(func_index, data_type));
		}
	}
	return PtgPtr();
}


void PtgFuncVar::setParamsNum(const BYTE num)
{
	cparams = num;
	if(0xFF == tab.getIndex())
	{
		++cparams;
	}
}


} // namespace XLS

