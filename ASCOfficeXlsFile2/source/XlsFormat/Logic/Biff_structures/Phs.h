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
	unsigned short ifnt;
	struct Phs_Data
	{
		unsigned short phType : 2;
		unsigned short alcH : 2;
		unsigned short set_one : 2;
		unsigned short unused : 10;
	} data;
};
#pragma pack()

} // namespace XLS
