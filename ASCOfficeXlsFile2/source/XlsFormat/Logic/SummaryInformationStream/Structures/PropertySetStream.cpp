
#include "PropertySetStream.h"
#include "Binary/CFStream.h"

#include "Auxiliary/HelpFunc.h"

namespace OLEPS
{;

PropertySetStream::PropertySetStream(XLS::CFStreamPtr stream)
{
	unsigned short	ByteOrder;
	unsigned short	Version;
	unsigned int	SystemIdentifier;
	unsigned int	NumPropertySets;
	_GUID_			Clsid;
	
	*stream >> ByteOrder >> Version >> SystemIdentifier >> Clsid >> NumPropertySets;
	
	std::vector<unsigned int> property_sets_offsets;
	for(unsigned int i = 0; i < NumPropertySets; ++i)
	{
		_GUID_ FMTID;
		unsigned int Offset;
		*stream >> FMTID >> Offset;
		property_sets_offsets.push_back(Offset);
	}

	for(unsigned int i = 0; i < NumPropertySets; ++i)
	{
		property_sets.push_back(PropertySetPtr(new PropertySet(stream, property_sets_offsets[i])));
	}
}

const unsigned short PropertySetStream::GetCodePage()
{
	return 0 != property_sets.size() ? property_sets[0]->GetCodePage() : 0;
}

} // namespace OLEPS
