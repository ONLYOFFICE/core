#pragma once
#include "Property.h"

#include <Binary/BinSmartPointers.h>

namespace OLEPS
{

class PropertyCodePage : public Property
{
public:
    PropertyCodePage(const unsigned short value_type, XLS::CFStreamPtr stream);
	
	static const unsigned int Type = 0x0001;
	static const unsigned short DefaultCodePage = 1250;

	unsigned short code_page;
};
typedef boost::shared_ptr<PropertyCodePage> PropertyCodePagePtr;
//-----------------------------------------------------------------------------------------
class PropertyTitle : public Property
{
public:
    PropertyTitle(const unsigned short value_type, XLS::CFStreamPtr stream);
	
	static const unsigned int Type = 0x0002;

	std::wstring title;
};
typedef boost::shared_ptr<PropertyTitle> PropertyTitlePtr;
//-----------------------------------------------------------------------------------------
class PropertySubject : public Property
{
public:
    PropertySubject(const unsigned short value_type, XLS::CFStreamPtr stream);
	
	static const unsigned int Type = 0x0003;

	std::wstring subject;
};
typedef boost::shared_ptr<PropertySubject> PropertySubjectPtr;
//-----------------------------------------------------------------------------------------
class PropertyAuthor : public Property
{
public:
    PropertyAuthor(const unsigned short value_type, XLS::CFStreamPtr stream);
	static const unsigned int Type = 0x0004;
	std::wstring author;
};
typedef boost::shared_ptr<PropertyAuthor> PropertyAuthorPtr;
//-----------------------------------------------------------------------------------------
class PropertyKeywords : public Property
{
public:
    PropertyKeywords(const unsigned short value_type, XLS::CFStreamPtr stream);
	static const unsigned int Type = 0x0005;
	std::wstring keywords;
};
typedef boost::shared_ptr<PropertyKeywords> PropertyKeywordsPtr;
//-----------------------------------------------------------------------------------------
class PropertyComments : public Property
{
public:
    PropertyComments(const unsigned short value_type, XLS::CFStreamPtr stream);
	static const unsigned int Type = 0x0006;
	std::wstring comments;
};
typedef boost::shared_ptr<PropertyComments> PropertyCommentsPtr;
//-----------------------------------------------------------------------------------------
class PropertyDateCreate : public Property
{
public:
    PropertyDateCreate(const unsigned short value_type, XLS::CFStreamPtr stream);
	static const unsigned int Type = 0x000c;
	std::wstring dateCreate;
};
typedef boost::shared_ptr<PropertyDateCreate> PropertyDateCreatesPtr;
} // namespace OLEPS
