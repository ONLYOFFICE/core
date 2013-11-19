#pragma once

#include "../BiffStructTagged.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtCOLORREF : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtCOLORREF)
public:
	OfficeArtCOLORREF();
	OfficeArtCOLORREF(const long raw_data);
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
private:
	virtual void load(XLS::CFRecord& record) {};
	virtual void store(XLS::CFRecord& record) {};

public:
	BYTE red;
	BYTE green;
	BYTE blue;

	bool fPaletteIndex;
	bool fPaletteRGB;
	bool fSystemRGB;
	bool fSchemeIndex;
	bool fSysIndex;

};

typedef boost::shared_ptr<OfficeArtCOLORREF> OfficeArtCOLORREFPtr;


} // namespace XLS
