
#include "ExternSheet.h"
#include <Logic/Biff_structures/XTI.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

ExternSheet::ExternSheet()
{
}


ExternSheet::~ExternSheet()
{
}


BaseObjectPtr ExternSheet::clone()
{
	return BaseObjectPtr(new ExternSheet(*this));
}


void ExternSheet::writeFields(CFRecord& record)
{
	cXTI = rgXTI.size();
	record << cXTI;
	for(BiffStructurePtrVector::const_iterator it = rgXTI.begin(), itEnd = rgXTI.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void ExternSheet::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char type;
		//record.skipNunBytes(record.getDataSize() - record.getRdPtr());
		ShortXLAnsiString stName;
		record >> type >> stName;

		std::wstring	name	= stName.value();
		//int				type	= stName.value().substr(0, 1).c_str()[0];
		if (!name.empty())
			record.getGlobalWorkbookInfo()->arExternalNames.push_back(name);

	}
	else
	{
		record >> cXTI;
		for(int i = 0; i < cXTI; ++i)
		{
			XTIPtr xti(new XTI);
			record >> *xti;
			rgXTI.push_back(xti);
		}
	}
}

} // namespace XLS

