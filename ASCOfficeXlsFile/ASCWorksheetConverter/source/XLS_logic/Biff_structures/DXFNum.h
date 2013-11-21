#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/DXFNumUsr.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class DXFNum : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFNum)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	void setIsUserDefined(const bool is_user_defined); 
	const bool getIsUserDefined() const; 

private:
	nullable<bool> is_user_defined_;
	DXFNumIFmt fmt_id;
	DXFNumUsr user_defined;

};

} // namespace XLS

