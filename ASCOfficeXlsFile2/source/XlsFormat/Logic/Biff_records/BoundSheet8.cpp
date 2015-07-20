
#include "BoundSheet8.h"
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{;

BoundSheet8::BoundSheet8()
{
}


BoundSheet8::~BoundSheet8()
{
}


BaseObjectPtr BoundSheet8::clone()
{
	return BaseObjectPtr(new BoundSheet8(*this));
}

void BoundSheet8::writeFields(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned int));

	unsigned short flags = 0;
	unsigned char hsState_num;
	if(hsState == std::wstring (L"visible"))
	{
		hsState_num = 0;
	}
	else if(hsState == std::wstring (L"hidden"))
	{
		hsState_num = 1;
	}
	else if(hsState == std::wstring (L"veryHidden"))
	{
		hsState_num = 3;
	}
	else
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"hsState", record.getTypeString().c_str(), hsState);
	}
	SETBITS(flags, 0, 1, hsState_num);
	SETBITS(flags, 8, 15, dt);
	record << flags << stName;
}


void BoundSheet8::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> lbPlyPos >> flags >> stName;

	record.getGlobalWorkbookInfo()->sheets_names.push_back(stName);

	switch(GETBITS(flags, 0, 1))
	{
		case 0:
			hsState = std::wstring (L"visible");
			break;
		case 1:
			hsState = std::wstring (L"hidden");
			break;
		case 2:
			hsState = std::wstring (L"veryHidden");
			break;
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported value of hsState.", record.getTypeString());
	}
	dt = GETBITS(flags, 8, 15);
}

} // namespace XLS

