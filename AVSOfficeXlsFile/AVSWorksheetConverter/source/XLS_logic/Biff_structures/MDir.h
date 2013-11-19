#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class MDir : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDir)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	long imdt;
	WORD mdd;
};

typedef boost::shared_ptr<MDir> MDirPtr;

} // namespace XLS
