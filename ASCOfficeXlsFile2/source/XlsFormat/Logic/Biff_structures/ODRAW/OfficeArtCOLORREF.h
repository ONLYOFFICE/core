#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{

class OfficeArtCOLORREF : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtCOLORREF)
public:
	OfficeArtCOLORREF();
    OfficeArtCOLORREF(const int raw_data);
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtCOLORREF;

    virtual void load(XLS::CFRecord& record) {}
    virtual void store(XLS::CFRecord& record) {}

	unsigned char red;
	unsigned char green;
	unsigned char blue;

	bool fPaletteIndex;
	bool fPaletteRGB;
	bool fSystemRGB;
	bool fSchemeIndex;
	bool fSysIndex;

	std::wstring	colorRGB;
	short			index;

};

typedef boost::shared_ptr<OfficeArtCOLORREF> OfficeArtCOLORREFPtr;


} // namespace XLS
