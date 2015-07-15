#pragma once

#include "../BiffStructure.h"
#include "OfficeArtFOPTE.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtRGFOPTE : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtRGFOPTE)
public:
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	void SetOptionsNumber(const unsigned __int16 number);
	const unsigned __int16 GetOptionsNumber();

private:
	std::vector<OfficeArtFOPTEPtr> rgfopte;
	unsigned __int16 options_num;
};

typedef boost::shared_ptr<OfficeArtRGFOPTE> OfficeArtRGFOPTEPtr;


} // namespace XLS
