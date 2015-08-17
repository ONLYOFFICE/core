

#include <Binary/CFStream.h>
#include "PropertyFactory.h"
#include "CodePage.h"


namespace OLEPS
{

PropertyFactory::PropertyFactory()
{
}

PropertyPtr PropertyFactory::ReadProperty(const unsigned int prop_type, XLS::CFStreamPtr stream, const unsigned int property_offset)
{
	stream->seekFromBegin(property_offset);
	unsigned short value_type;
	*stream >> value_type;
	stream->seekFromCurForward(2); // Skip 2 reserved unsigned chars

	switch(prop_type)
	{
		case CodePage::Type:
			return PropertyPtr(new CodePage(value_type, stream));

		default:
			return PropertyPtr();
	}

}



} // namespace OLEPS
