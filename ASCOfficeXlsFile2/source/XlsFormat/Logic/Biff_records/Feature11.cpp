
#include "Feature11.h"

namespace XLS
{;

Feature11::Feature11()
{
}


Feature11::~Feature11()
{
}


BaseObjectPtr Feature11::clone()
{
	return BaseObjectPtr(new Feature11(*this));
}


void Feature11::writeFields(CFRecord& record)
{
}


void Feature11::readFields(CFRecord& record)
{
	record >> frtRefHeaderU;
	record >> isf;
	record.skipNunBytes(1);		// reserved1
	record.skipNunBytes(4);		// reserved2
	record >> cref2;
	record >> cbFeatData;
	record.skipNunBytes(2);		// reserved3

	unsigned short _isf = isf;
	unsigned short _cref2 = cref2;
	unsigned int _cbFeatData = cbFeatData;	

	std::wstring  sqref_str;
	for (int i = 0; i < cref2 ; ++i)
	{
		Ref8U reff;
		record >> reff;
		refs2.push_back(BiffStructurePtr(new Ref8U(reff)));
		sqref_str += std::wstring (reff.toString().c_str()) + ((i == cref2 - 1) ? L"" : L" ");
	}
	sqref = sqref_str;	

	record >> rgbFeat;	
}

} // namespace XLS

