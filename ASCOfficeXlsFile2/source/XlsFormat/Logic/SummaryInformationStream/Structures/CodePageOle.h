#pragma once
#include "Property.h"

#include <Binary/BinSmartPointers.h>

namespace OLEPS
{

class CodePageOle : public Property
{
public:
    CodePageOle(const unsigned short value_type, XLS::CFStreamPtr stream);
	
	static const unsigned int Type = 0x0001;
	static const unsigned short DefaultCodePage = 1250;

	const unsigned short GetCodePage();


private:
	unsigned short code_page;
};

typedef boost::shared_ptr<CodePageOle> CodePagePropertyPtr;

} // namespace OLEPS
