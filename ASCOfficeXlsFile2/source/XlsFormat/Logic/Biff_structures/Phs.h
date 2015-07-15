#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

#pragma pack(1)
class Phs : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Phs)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 ifnt;
	struct Phs_Data
	{
		unsigned __int16 phType : 2;
		unsigned __int16 alcH : 2;
		unsigned __int16 set_one : 2;
		unsigned __int16 unused : 10;
	} data;
};
#pragma pack()

} // namespace XLS
