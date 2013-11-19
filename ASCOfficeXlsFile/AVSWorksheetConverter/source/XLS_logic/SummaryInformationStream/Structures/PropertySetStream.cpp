#include "StdAfx.h"
#include "PropertySetStream.h"
#include "XLS_bin\CFStream.h"


namespace OLEPS
{;

PropertySetStream::PropertySetStream(XLS::CFStreamPtr stream)
{
	WORD ByteOrder;
	WORD Version;
	DWORD SystemIdentifier;
	CLSID clsid;
	DWORD NumPropertySets;
	*stream >> ByteOrder >> Version >> SystemIdentifier >> clsid >> NumPropertySets;
	
	std::vector<DWORD> property_sets_offsets;
	for(DWORD i = 0; i < NumPropertySets; ++i)
	{
		GUID FMTID;
		DWORD Offset;
		*stream >> FMTID >> Offset;
		property_sets_offsets.push_back(Offset);
	}

	for(DWORD i = 0; i < NumPropertySets; ++i)
	{
		property_sets.push_back(PropertySetPtr(new PropertySet(stream, property_sets_offsets[i])));
	}
}

const WORD PropertySetStream::GetCodePage()
{
	return 0 != property_sets.size() ? property_sets[0]->GetCodePage() : 0;
}

} // namespace OLEPS
