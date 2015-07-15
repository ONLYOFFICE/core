#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;
class CFStream;

class CellOffsetResender : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CellOffsetResender)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent); // stubbed to avoid generating unnecessary tags
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static void resend(CFStream& stream, const unsigned int data_place, const unsigned int data);

private:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag); // Stubbed to make class non-abstract

private:
	bool row_starts;
};

typedef boost::shared_ptr<CellOffsetResender> CellOffsetResenderPtr;

} // namespace XLS

