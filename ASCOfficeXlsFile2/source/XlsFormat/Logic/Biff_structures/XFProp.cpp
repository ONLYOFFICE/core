
#include "XFProp.h"
#include <Binary/CFRecord.h>
#include "XFPropColor.h"
#include "XFPropGradient.h"
#include "XFPropGradientStop.h"
#include "XFPropBorder.h"
#include "BiffString.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;


BiffStructurePtr XFProp::clone()
{
	return BiffStructurePtr(new XFProp(*this));
}


void XFProp::store(CFRecord& record)
{
#pragma message("####################### XFProp record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFProp record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFProp::load(CFRecord& record)
{
	record >> xfPropType >> cb;

	//record.skipNunBytes(cb - sizeof(cb) - sizeof(xfPropType));
	switch(xfPropType)
	{
		case 0x0000:
			xfPropDataBlob.reset(new BIFF_BYTE(0, L"value"));
			break;
		case 0x0001:
		case 0x0002:
		case 0x0005:
			xfPropDataBlob.reset(new XFPropColor);
			break;
		case 0x0003:
			xfPropDataBlob.reset(new XFPropGradient);
			break;
		case 0x0004:
			xfPropDataBlob.reset(new XFPropGradientStop);
			break;
		case 0x0006:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x000B:
		case 0x000C:
			xfPropDataBlob.reset(new XFPropBorder);
			break;
		case 0x000D:
		case 0x000E:
		case 0x000F:
		case 0x0010:
		case 0x0011: // XFPropTextRotation
		case 0x0013: // ReadingOrder
		case 0x0014:
		case 0x0015:
		case 0x0016:
		case 0x0017:
		case 0x001C:
		case 0x001D:
		case 0x001E:
		case 0x001F:
		case 0x0020:
		case 0x0021:
		case 0x0022:
		case 0x0023:
		case 0x0025:
		case 0x002B:
		case 0x002C:
			xfPropDataBlob.reset(new BIFF_BYTE(0, L"value"));
			break;
		case 0x0018:
			{
				boost::shared_ptr<LPWideString> str(new LPWideString);
				str->setName(L"value");
				record >> *str;
				xfPropDataBlob = str;
				return;
			}
		case 0x0019:
		case 0x001A:
		case 0x001B:
		case 0x0029:
		case 0x002A:
			xfPropDataBlob.reset(new BIFF_WORD(0, L"value"));
			break;
		case 0x0024:
			xfPropDataBlob.reset(new BIFF_DWORD(0, L"value"));
			break;
		case 0x0026:
			{
				boost::shared_ptr<LPWideString> str(new LPWideString);
				str->setName(L"value");
				record >> *str;
				xfPropDataBlob = str;
				return;
			}
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of XFProp.", record.getTypeString());
	}
	record >> *xfPropDataBlob;
}


} // namespace XLS
