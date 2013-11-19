#pragma once

#include "../BiffStructTagged.h"
#include "OfficeArtFOPTE.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtRGFOPTE : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtRGFOPTE)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	void SetOptionsNumber(const WORD number);
	const WORD GetOptionsNumber();

private:
	std::vector<OfficeArtFOPTEPtr> rgfopte;
	WORD options_num;
};

typedef boost::shared_ptr<OfficeArtRGFOPTE> OfficeArtRGFOPTEPtr;


} // namespace XLS
