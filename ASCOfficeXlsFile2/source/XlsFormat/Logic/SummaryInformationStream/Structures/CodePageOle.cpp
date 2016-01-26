
#include "CodePageOle.h"
#include <Binary/CFStream.h>
#include <Auxiliary/HelpFunc.h>

namespace OLEPS
{

PropertyCodePage::PropertyCodePage(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_I2);
	*stream >> code_page;
}
PropertyTitle::PropertyTitle(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_LPSTR);
	_UINT32 size;
	*stream >> size;

	if (size > 0)
	{
		char *s = new char[size];
		stream->read(s,size);
		title  = STR::toStdWString(s, size, 0);
		delete []s;
	}
}
PropertySubject::PropertySubject(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_LPSTR);
	_UINT32 size;
	*stream >> size;

	if (size > 0)
	{
		char *s = new char[size];
		stream->read(s,size);
		subject  = STR::toStdWString(s, size, 0);
		delete []s;
	}
}
PropertyAuthor::PropertyAuthor(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_LPSTR);
	_UINT32 size;
	*stream >> size;

	if (size > 0)
	{
		char *s = new char[size];
		stream->read(s, size);
		author  = STR::toStdWString(std::string(s,size), 0);
		delete []s;
	}
}
PropertyKeywords::PropertyKeywords(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_LPSTR);
	_UINT32 size;
	*stream >> size;

	if (size > 0)
	{
		char *s = new char[size];
		stream->read(s,size);
		keywords = STR::toStdWString(std::string(s,size), 0);
		delete []s;
	}
}

PropertyComments::PropertyComments(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_LPSTR);
	_UINT32 size;
	*stream >> size;

	if (size > 0)
	{
		char *s = new char[size];
		stream->read(s,size);
		comments = STR::toStdWString(s, size, 0);
		delete []s;
	}
}

PropertyDateCreate::PropertyDateCreate(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_FILETIME);
	_UINT32 dwLowDateTime, dwHighDateTime;
	*stream >> dwLowDateTime >> dwHighDateTime;

}
} // namespace OLEPS
