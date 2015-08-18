

#include "PropertySet.h"
#include <Binary/CFStream.h>
#include "PropertyFactory.h"
#include "CodePageOle.h"


namespace OLEPS
{

PropertySet::PropertySet(XLS::CFStreamPtr stream, const unsigned int property_set_offset)
{
	stream->seekFromBegin(property_set_offset);

	unsigned int Size;
	*stream >> Size;
	unsigned int NumProperties;
	*stream >> NumProperties;
	
	std::vector<PropertyIdentifierAndOffset> prop_offsets;
	for(unsigned int i = 0; i < NumProperties; ++i)
	{
		PropertyIdentifierAndOffset prop_offset;
		*stream >> prop_offset;
		prop_offsets.push_back(prop_offset);
	}

    code_page = CodePageOle::DefaultCodePage;
	for(unsigned int i = 0; i < NumProperties; ++i)
	{
		PropertyPtr next_property = PropertyFactory::ReadProperty(prop_offsets[i].PropertyIdentifier, stream, property_set_offset + prop_offsets[i].Offset);
		if(next_property) // Skip the property if the corresponding class is not implemented
		{
			properties.push_back(next_property);

            CodePagePropertyPtr code_page_property = boost::dynamic_pointer_cast<CodePageOle>(next_property);
			if(code_page_property)
			{
				code_page = code_page_property->GetCodePage();
			}
		}
	}

}

const unsigned short PropertySet::GetCodePage()
{
	return code_page;
}

} // namespace OLEPS
