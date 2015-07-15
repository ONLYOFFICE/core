#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/FormatRun.h>

namespace XLS
{;

class Run: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Run)
public:
	BiffStructurePtr clone();

	Run();
	~Run();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:	
	FormatRun formatRun;
};

typedef boost::shared_ptr<Run> RunPtr;

} // namespace XLS