#pragma once

#include "Ptg.h"
#include "Cetab.h"
#include "Ftab_Cetab.h"

namespace XLS
{;

class CFRecord;

class PtgFuncVar : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgFuncVar)
	PtgFuncVar(const WORD func_index, const BYTE data_type);
public:
	PtgFuncVar();
	BiffStructurePtr clone();

	static PtgPtr create(const std::wstring& word, const BYTE data_type);

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

	void setParamsNum(const BYTE num);

	static const WORD fixed_id = 0x02;
private:
	BYTE cparams;
	Ftab_Cetab tab;
	bool fCeFunc;
};

typedef boost::shared_ptr<PtgFuncVar> PtgFuncVarPtr;


} // namespace XLS

