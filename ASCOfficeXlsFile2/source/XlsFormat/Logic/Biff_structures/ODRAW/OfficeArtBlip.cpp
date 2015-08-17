
#include "OfficeArtBlip.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{

OfficeArtBlip::OfficeArtBlip(const unsigned short recType)
:	OfficeArtRecord(0, recType)
{
}

OfficeArtBlipPtr OfficeArtBlip::load_blip(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh_child;
	record >> rh_child;
	size_t child_beginning_ptr = record.getRdPtr();
	record.RollRdPtrBack(rh_child.size());

	OfficeArtBlipPtr blip_record;
	switch (rh_child.recType)
	{
		case BlipEMF:
			blip_record = OfficeArtBlipPtr(new OfficeArtBlipEMF);
			break;

		case BlipWMF:
			blip_record = OfficeArtBlipPtr(new OfficeArtBlipWMF);
			break;

		case BlipPICT:
			blip_record = OfficeArtBlipPtr(new OfficeArtBlipPICT);
			break;

		case BlipJPEG:
		case BlipJPEG_clone:
			blip_record = OfficeArtBlipPtr(new OfficeArtBlipJPEG);
			break;

		case BlipPNG:
			blip_record = OfficeArtBlipPtr(new OfficeArtBlipPNG);
			break;

		case BlipDIB:
			blip_record = OfficeArtBlipPtr(new OfficeArtBlipDIB);
			break;

		case BlipTIFF:
			blip_record = OfficeArtBlipPtr(new OfficeArtBlipTIFF);
			break;

	}
	if(blip_record)
	{
		record >> *blip_record;
	}
	return blip_record;
}


} // namespace ODRAW
