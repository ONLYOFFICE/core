#pragma once
#include "Property.h"

#include "Binary/BinSmartPointers.h"

namespace OLEPS
{;

class CodePage : public Property
{
public:
	CodePage(const unsigned __int16 value_type, XLS::CFStreamPtr stream);
	
	static const unsigned int Type = 0x0001;
	static const unsigned __int16 DefaultCodePage = 1250;

	const unsigned __int16 GetCodePage();


private:
	unsigned __int16 code_page;
};

typedef boost::shared_ptr<CodePage> CodePagePropertyPtr;

} // namespace OLEPS
