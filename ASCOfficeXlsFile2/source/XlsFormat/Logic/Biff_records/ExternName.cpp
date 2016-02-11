
#include "ExternName.h"

namespace XLS
{

ExternName::ExternName(const unsigned short supporting_link_type)
:	supbook_cch(supporting_link_type)
{
}


ExternName::~ExternName()
{
}


BaseObjectPtr ExternName::clone()
{
	return BaseObjectPtr(new ExternName(*this));
}


void ExternName::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fBuiltIn);
	SETBIT(flags, 1, fWantAdvise);
	SETBIT(flags, 2, fWantPict);
	SETBIT(flags, 3, fOle);
	SETBIT(flags, 4, fOleLink);
	SETBITS(flags, 5, 14, cf);
	SETBIT(flags, 15, fIcon);
	record << flags;

	body->store(record);
}


void ExternName::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fBuiltIn = GETBIT(flags, 0);
	fWantAdvise = GETBIT(flags, 1);
	fWantPict = GETBIT(flags, 2);
	fOle = GETBIT(flags, 3);
	fOleLink = GETBIT(flags, 4);
	cf = GETBITS(flags, 5, 14);
	fIcon = GETBIT(flags, 15);

	if(0x3A01 == supbook_cch) // UDF reference on a XLL or COM add-in.
	{
		body = BiffStructurePtr(new AddinUdf);
	}
	else
	{
#pragma message("####################### ExternName record is not implemented")
		Log::error("ExternName record is not implemented.");

		if(fOle && !fOleLink) // DDE data item
		{
			body = BiffStructurePtr(new ExternDdeLinkNoOper);
		}
		if(!fOle && fOleLink) // DDE data item
		{
			body = BiffStructurePtr(new ExternOleDdeLink);
		}
		// Nu i kak ya dolzhen opredelit', DDE eto ili OLE?!!!
		// V Mikrosofte vse ebanutye - pust' sami parsyat, debily
		if(!fOle && !fOleLink)
		{
			body = BiffStructurePtr(new ExternDocName);
		}
		// This fills in the gaps between AddinUdfs if the body is not AddinUdf. The simplest way to maintain indexing from my point of view.
	}
	body->load(record);

	if(0x3A01 != supbook_cch)
	{
		std::wstring name; 
		if(!fOle && !fOleLink)
		{
			ExternDocName* n = dynamic_cast<ExternDocName*>(body.get());

			if (n->ixals > 0)
			{
				//from SupBook
			}
			else
			{
				name = n->nameDefinition.getAssembledFormula();
				if (name.empty())
					name = n->extName.value();
			}
		}
		if(fOle && !fOleLink) // DDE data item
		{
			ExternDdeLinkNoOper* n = dynamic_cast<ExternDdeLinkNoOper*>(body.get());
			name = n->linkName.value();
		}
		if(!fOle && fOleLink)
		{
			ExternOleDdeLink* n = dynamic_cast<ExternOleDdeLink*>(body.get());
			name = n->linkName.value();
		}
		record.getGlobalWorkbookInfo()->arExternalNames.push_back(name);
	}
}

} // namespace XLS

