#include "StdAfx.h"
#include "PropertySet.h"
#include "XLS_bin\CFStream.h"
#include "PropertyFactory.h"
#include "CodePage.h"


namespace OLEPS
{;

PropertySet::PropertySet(XLS::CFStreamPtr stream, const DWORD property_set_offset)
{
	stream->seekFromBegin(property_set_offset);

	DWORD Size;
	*stream >> Size;
	DWORD NumProperties;
	*stream >> NumProperties;
	
	std::vector<PropertyIdentifierAndOffset> prop_offsets;
	for(DWORD i = 0; i < NumProperties; ++i)
	{
		PropertyIdentifierAndOffset prop_offset;
		*stream >> prop_offset;
		prop_offsets.push_back(prop_offset);
	}

	code_page = CodePage::DefaultCodePage;
	for(DWORD i = 0; i < NumProperties; ++i)
	{
		PropertyPtr next_property = PropertyFactory::ReadProperty(prop_offsets[i].PropertyIdentifier, stream, property_set_offset + prop_offsets[i].Offset);
		if(next_property) // Skip the property if the corresponding class is not implemented
		{
			properties.push_back(next_property);

			CodePagePropertyPtr code_page_property = boost::dynamic_pointer_cast<CodePage>(next_property);
			if(code_page_property)
			{
				code_page = code_page_property->GetCodePage();
			}
		}
	}

}

const WORD PropertySet::GetCodePage()
{
	return code_page;
}

} // namespace OLEPS
