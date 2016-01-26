

#include <Binary/CFStream.h>
#include "PropertyFactory.h"
#include "CodePageOle.h"


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
	case 0x01://CodePage::Type:
		return PropertyPtr(new PropertyCodePage(value_type, stream));
	case 0x02://TITLE
		return PropertyPtr(new PropertyTitle(value_type, stream));
	case 0x03://SUBJECT
		return PropertyPtr(new PropertySubject(value_type, stream));
	case 0x04://AUTHOR
		return PropertyPtr(new PropertyAuthor(value_type, stream));
	case 0x05://KEYWORDS
		return PropertyPtr(new PropertyKeywords(value_type, stream));
	case 0x06://COMMENTS
		return PropertyPtr(new PropertyComments(value_type, stream));
	case 0x0C://CREATE_DTM
		return PropertyPtr(new PropertyDateCreate(value_type, stream));

		default:
			return PropertyPtr();
	}

}



} // namespace OLEPS
