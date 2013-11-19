#include "StdAfx.h"
#include "XLS_bin\CFStream.h"
#include "PropertyFactory.h"
#include "CodePage.h"


namespace OLEPS
{;

PropertyFactory::PropertyFactory()
{
}

PropertyPtr PropertyFactory::ReadProperty(const DWORD prop_type, XLS::CFStreamPtr stream, const DWORD property_offset)
{
	stream->seekFromBegin(property_offset);
	WORD value_type;
	*stream >> value_type;
	stream->seekFromCurForward(2); // Skip 2 reserved bytes

	switch(prop_type)
	{
		case CodePage::Type:
			return PropertyPtr(new CodePage(value_type, stream));

		default:
			return PropertyPtr();
	}

}



} // namespace OLEPS
