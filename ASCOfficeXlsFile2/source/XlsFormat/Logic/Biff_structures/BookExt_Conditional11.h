#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class BookExt_Conditional11 : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(BookExt_Conditional11)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool fBuggedUserAboutSolution;
	bool fShowInkAnnotation;
};

} // namespace XLS

