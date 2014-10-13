#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtCOLORREF : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtCOLORREF)
public:
	OfficeArtCOLORREF();
	OfficeArtCOLORREF(const long raw_data);
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
private:
	virtual void load(XLS::CFRecord& record) {};
	virtual void store(XLS::CFRecord& record) {};

public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;

	bool fPaletteIndex;
	bool fPaletteRGB;
	bool fSystemRGB;
	bool fSchemeIndex;
	bool fSysIndex;

};

typedef boost::shared_ptr<OfficeArtCOLORREF> OfficeArtCOLORREFPtr;


} // namespace XLS
