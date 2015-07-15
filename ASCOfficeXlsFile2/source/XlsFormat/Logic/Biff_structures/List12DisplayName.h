#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>


namespace XLS
{;

class List12DisplayName: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(List12DisplayName)
public:
	BiffStructurePtr clone();

	List12DisplayName();
	~List12DisplayName();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	XLNameUnicodeString stListName;
	XLUnicodeString stListComment;
};

typedef boost::shared_ptr<List12DisplayName> List12DisplayNamePtr;

} // namespace XLS