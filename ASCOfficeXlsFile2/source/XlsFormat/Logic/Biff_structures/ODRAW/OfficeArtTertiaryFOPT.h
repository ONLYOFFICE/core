#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtRGFOPTE.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtTertiaryFOPT : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtTertiaryFOPT)
public:
	OfficeArtTertiaryFOPT();
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned short GetInstanceToStore();

private:
	OfficeArtRGFOPTE fopt;

};

typedef boost::shared_ptr<OfficeArtTertiaryFOPT> OfficeArtTertiaryFOPTPtr;


} // namespace XLS
