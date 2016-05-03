
#include "BoundSheet8.h"
#include <boost/lexical_cast.hpp>


namespace XLS
{

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
	ShortXLUnicodeString stName = name_;

	SETBITS(flags, 0, 1, hsState_num);
	SETBITS(flags, 8, 15, dt);
	record << flags << stName;
}


void BoundSheet8::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> lbPlyPos >> flags;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		ShortXLAnsiString stName;
		record >> stName;
		name_ = stName.value();
	}
	else
	{
		ShortXLUnicodeString stName;
		record >> stName;
		name_ = stName.value();
	}

	switch(GETBITS(flags, 0, 1))
	{
		case 0:
			hsState = std::wstring (L"visible");
			break;
		case 1:
			hsState = std::wstring (L"hidden");
			break;
		case 2:
			hsState = std::wstring (L"hidden");//(L"veryHidden");
			break;
	}
	if (name_.length() > 31)
	{
		name_ = name_.substr(0, 31);
		//cell_links_broken.xls
	}
	if (name_.empty())
	{//file(6).xls
		name_ = L"Sheet_" + boost::lexical_cast<std::wstring>(record.getGlobalWorkbookInfo()->current_sheet + 1);
	}
	
	record.getGlobalWorkbookInfo()->sheets_names.push_back(name_);
	record.getGlobalWorkbookInfo()->sheets_state.push_back(hsState);
	
	dt = GETBITS(flags, 8, 15);
}

} // namespace XLS

