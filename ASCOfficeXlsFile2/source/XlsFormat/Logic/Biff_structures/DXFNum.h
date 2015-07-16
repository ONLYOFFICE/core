#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/DXFNumUsr.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class DXFNum : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFNum)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	void setIsUserDefined(const bool is_user_defined); 
	const bool getIsUserDefined() const; 

private:
	_CP_OPT(bool)	is_user_defined_;
	DXFNumIFmt		fmt_id;
	DXFNumUsr		user_defined;

};

} // namespace XLS

